#include "connection.hpp"
#include <unistd.h>
#include <cstdlib>
#include <ctime>

Connection::Connection(int fd, const std::string& cIp, int cPort,
                       const std::string& sIp, int sPort, size_t sIndex)
    : clientFd(fd), clientIp(cIp), clientPort(cPort),
      serverIp(sIp), serverPort(sPort), serverIndex(sIndex),
      bytesSent(0), state(READING), headerParsed(false),
      contentLength(0), bodyBytesReceived(0), bodyFilePath(""),
      bodyFileFd(-1), sendFileFd(-1), lastActivity(std::time(NULL))
{
}

Connection::~Connection()
{
    if (bodyFileFd >= 0)
        close(bodyFileFd);
    if (sendFileFd >= 0)
        close(sendFileFd);
}

Connection::Connection(const Connection& other)
    : clientFd(other.clientFd), clientIp(other.clientIp),
      clientPort(other.clientPort), serverIp(other.serverIp),
      serverPort(other.serverPort), serverIndex(other.serverIndex),
      buffer(other.buffer), lastRequest(other.lastRequest),
      writeBuffer(other.writeBuffer), bytesSent(other.bytesSent),
      state(other.state), headerParsed(other.headerParsed),
      contentLength(other.contentLength),
      bodyBytesReceived(other.bodyBytesReceived),
      bodyFilePath(other.bodyFilePath),
      bodyFileFd(-1),   // never share a file descriptor across copies
      sendFileFd(-1),
      lastActivity(other.lastActivity)
{
}

Connection& Connection::operator=(const Connection& other)
{
    if (this != &other)
    {
        if (bodyFileFd >= 0)
            close(bodyFileFd);
        if (sendFileFd >= 0)
            close(sendFileFd);
        clientFd = other.clientFd;
        clientIp = other.clientIp;
        clientPort = other.clientPort;
        serverIp = other.serverIp;
        serverPort = other.serverPort;
        serverIndex = other.serverIndex;
        buffer = other.buffer;
        lastRequest = other.lastRequest;
        writeBuffer = other.writeBuffer;
        bytesSent = other.bytesSent;
        state = other.state;
        headerParsed = other.headerParsed;
        contentLength = other.contentLength;
        bodyBytesReceived = other.bodyBytesReceived;
        bodyFilePath = other.bodyFilePath;
        bodyFileFd = -1;
        sendFileFd = -1;
        lastActivity = other.lastActivity;
    }
    return *this;
}

int Connection::getClientFd() const { return clientFd; }
std::string Connection::getClientIp() const { return clientIp; }
int Connection::getClientPort() const { return clientPort; }
std::string Connection::getServerIp() const { return serverIp; }
int Connection::getServerPort() const { return serverPort; }
size_t Connection::getServerIndex() const { return serverIndex; }

const std::string& Connection::getBodyFilePath() const { return bodyFilePath; }
time_t Connection::getLastActivity() const { return lastActivity; }
void Connection::updateLastActivity() { lastActivity = std::time(NULL); }

// FIX: previously all incoming bytes (headers + entire body) were appended into
// buffer as a single std::string. For a 1GB upload this meant 1GB lived in memory
// and isRequestComplete() returned false until all of it arrived, starving other
// clients waiting for their first recv().
//
// New behaviour:
//   - While headers are incomplete, bytes go into buffer as before.
//   - The moment \r\n\r\n is found we parse Content-Length, open a temp file via
//     mkstemp(), flush any body bytes already sitting in buffer to that file, then
//     erase them from buffer so buffer holds ONLY the header section.
//   - Every subsequent recv() chunk is written straight to the file — never copied
//     into memory — so memory usage stays flat regardless of upload size.
void Connection::append(const char* data, size_t size)
{
    if (!headerParsed)
    {
        buffer.append(data, size);

        size_t headerEnd = buffer.find("\r\n\r\n");
        if (headerEnd == std::string::npos)
            return;

        headerParsed = true;

        size_t clPos = buffer.find("Content-Length:");
        if (clPos != std::string::npos && clPos < headerEnd)
        {
            size_t lineEnd = buffer.find("\r\n", clPos);
            std::string val = buffer.substr(clPos + 15, lineEnd - (clPos + 15));
            size_t firstChar = val.find_first_not_of(" \t");
            if (firstChar != std::string::npos)
                val = val.substr(firstChar);
            contentLength = static_cast<size_t>(std::atoi(val.c_str()));
        }

        if (contentLength > 0)
        {
            char tpl[] = "/tmp/webserv_body_XXXXXX";
            bodyFileFd = mkstemp(tpl);
            bodyFilePath = tpl;

            size_t bodyStart = headerEnd + 4;
            if (bodyStart < buffer.size())
            {
                size_t inBuffer = buffer.size() - bodyStart;
                ssize_t written = write(bodyFileFd, buffer.c_str() + bodyStart, inBuffer);
                if (written > 0)
                    bodyBytesReceived += static_cast<size_t>(written);
            }
            buffer.erase(bodyStart); // keep only headers in memory
        }
        return;
    }

    // headers already parsed — stream body bytes directly to disk
    if (bodyFileFd >= 0)
    {
        ssize_t written = write(bodyFileFd, data, size);
        if (written > 0)
            bodyBytesReceived += static_cast<size_t>(written);
    }
}

// FIX: previously checked buffer.size() >= full request size, which required the
// entire body to be in memory. Now checks the file byte counter instead.
bool Connection::isRequestComplete()
{
    if (!headerParsed)
        return false;
    if (contentLength == 0)
        return true; // no body (GET, HEAD, DELETE)
    return bodyBytesReceived >= contentLength;
}

// FIX: previously returned headers + full body as one string (caused a second
// copy of the body in memory). Now returns headers only — body is in bodyFilePath.
std::string Connection::extractRequest()
{
    lastRequest = buffer;
    return lastRequest;
}

void Connection::consumeRequest()
{
    buffer.clear();
    headerParsed = false;
    contentLength = 0;
    bodyBytesReceived = 0;
    if (bodyFileFd >= 0)
    {
        close(bodyFileFd);
        bodyFileFd = -1;
    }
    // bodyFilePath kept intentionally — caller still needs to read the file
}

std::string& Connection::getBuffer() { return buffer; }
const std::string& Connection::getRequest() const { return lastRequest; }

void Connection::setResponse(const std::string& res)
{
    writeBuffer = res;
    bytesSent = 0;
}

std::string& Connection::getWriteBuffer() { return writeBuffer; }
size_t& Connection::getBytesSent() { return bytesSent; }

State Connection::getState() const { return state; }
void Connection::setState(State s) { state = s; }

// sendFileFd: open file descriptor used by handleSending() to stream a static
// file in chunks without ever loading it into memory (see handleSending fix).
int Connection::getSendFileFd() const { return sendFileFd; }
void Connection::setSendFileFd(int fd) { sendFileFd = fd; }

void Connection::closeSendFileFd()
{
    if (sendFileFd >= 0)
    {
        close(sendFileFd);
        sendFileFd = -1;
    }
}
