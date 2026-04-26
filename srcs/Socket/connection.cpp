#include "connection.hpp"

Connection::Connection(int fd, const std::string& cIp, int cPort,const std::string& sIp, int sPort, size_t sIndex)
    : clientFd(fd), clientIp(cIp), clientPort(cPort),
      serverIp(sIp), serverPort(sPort), serverIndex(sIndex),
      bytesSent(0), state(READING), lastActivity(time(NULL))
{
}

Connection::~Connection()
{
}

Connection::Connection(const Connection& other)
    : clientFd(other.clientFd), clientIp(other.clientIp),
      clientPort(other.clientPort), serverIp(other.serverIp),
      serverPort(other.serverPort), serverIndex(other.serverIndex),
      buffer(other.buffer), lastRequest(other.lastRequest),
      writeBuffer(other.writeBuffer), bytesSent(other.bytesSent),
      state(other.state), lastActivity(other.lastActivity)
{
}

Connection& Connection::operator=(const Connection& other)
{
    if (this != &other)
    {
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
        lastActivity = other.lastActivity;
    }
    return *this;
}

int Connection::getClientFd() const
{
        return clientFd; 
}
std::string Connection::getClientIp() const {
        return clientIp; 
}
int Connection::getClientPort() const
{
         return clientPort;
}
std::string Connection::getServerIp() const {
         return serverIp;
}
int Connection::getServerPort() const { 
        return serverPort; 
}
size_t Connection::getServerIndex() const 
{
         return serverIndex;
}


//  new 
void Connection::append(const char* data, size_t size)
{
    buffer.append(data, size);
}

std::string& Connection::getBuffer()
{
    return buffer;
}

const std::string& Connection::getRequest() const
{
    return lastRequest;
}

void Connection::setResponse(const std::string& res)
{
    writeBuffer = res;
    bytesSent = 0;
}
std::string& Connection::getWriteBuffer()
{
    return writeBuffer;
}
size_t& Connection::getBytesSent()
{
    return bytesSent;
}

// bool Connection::isRequestComplete()
// {
//     size_t headerEnd = buffer.find("\r\n\r\n");
//     if (headerEnd == std::string::npos)
//         return false;

//     std::string method = getMethod();

//     size_t bodyStart = headerEnd + 4;

//     size_t contentLength = 0;
//     size_t pos = buffer.find("Content-Length:");

//     if (pos != std::string::npos)
//     {
//         size_t end = buffer.find("\r\n", pos);
//         std::string value = buffer.substr(pos + 15, end - (pos + 15));
//         contentLength = std::atoi(value.c_str());
//     }

//     // 🔥 HERE IS YOUR POST CHECK
//     if (method == "POST" && contentLength == 0)
//     {
//         std::cout << "Invalid POST: missing Content-Length\n";
//         return false; // or handle error later
//     }

//     size_t bodySize = buffer.size() - bodyStart;

//     return bodySize >= contentLength;
// }

bool Connection::isRequestComplete()
{
    size_t headerEnd = buffer.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        return false;

    size_t pos = buffer.find("Content-Length:");
    if (pos == std::string::npos || pos > headerEnd)
        return true;

    size_t end = buffer.find("\r\n", pos);
    std::string value = buffer.substr(pos + 15, end - (pos + 15));
    size_t contentLength = std::atoi(value.c_str());

    size_t bodySize = buffer.size() - (headerEnd + 4);
    return bodySize >= contentLength;
}

std::string Connection::extractRequest()
{
    size_t headerEnd = buffer.find("\r\n\r\n");
    size_t bodyStart = headerEnd + 4;

    size_t contentLength = 0;

    size_t pos = buffer.find("Content-Length:");
    if (pos != std::string::npos)
    {
        size_t end = buffer.find("\r\n", pos);
        std::string value = buffer.substr(pos + 15, end - (pos + 15));
        contentLength = std::atoi(value.c_str());
    }

    size_t totalSize = bodyStart + contentLength;

    lastRequest = buffer.substr(0, totalSize);

    return lastRequest;
}

void Connection::consumeRequest()
{
    size_t headerEnd = buffer.find("\r\n\r\n");
    size_t bodyStart = headerEnd + 4;

    size_t contentLength = 0;

    size_t pos = buffer.find("Content-Length:");
    if (pos != std::string::npos)
    {
        size_t end = buffer.find("\r\n", pos);
        std::string value = buffer.substr(pos + 15, end - (pos + 15));
        contentLength = std::atoi(value.c_str());
    }

    size_t totalSize = bodyStart + contentLength;

    buffer.erase(0, totalSize);
}

State Connection::getState() const
{
    return state;
}

void Connection::setState(State s)
{
    state = s;
}

void Connection::updateActivity()
{
    lastActivity = time(NULL);
}

time_t Connection::getLastActivity() const
{
    return lastActivity;
}