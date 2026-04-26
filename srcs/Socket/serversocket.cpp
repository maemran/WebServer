/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serversocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 01:30:51 by asaadeh           #+#    #+#             */
/*   Updated: 2026/04/23 18:31:33 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serversocket.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <fcntl.h>
#include "connection.hpp"

static const int CONNECTION_TIMEOUT_SECS = 20;

ServerSocket::ServerSocket(const HttpConfig& config)
{
    this->config = config;//important
    servers = config.getServers();
    serverFds.resize(servers.size(), -1);
}

ServerSocket::~ServerSocket()
{
    for (size_t i = 0; i < serverFds.size(); i++)
    {
        if (serverFds[i] != -1)
            close(serverFds[i]);
    }
}
void ServerSocket::start()
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        serverFds[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFds[i] < 0)
            throw std::runtime_error("Socket creation failed");
        
        int opt = 1;
        setsockopt(serverFds[i], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        // Make server socket non-blocking
        int flags = fcntl(serverFds[i], F_GETFL, 0); // this flag read the current configration from the socket
        fcntl(serverFds[i], F_SETFL, flags | O_NONBLOCK);// the default socket is blocking , here i change propereties of fd to be non blocking 
        

        
        sockaddr_in address;
        std::memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(servers[i].getListenIp().c_str());
        address.sin_port = htons(servers[i].getListenPort());

        if (bind(serverFds[i], (sockaddr*)&address, sizeof(address)) < 0)
            throw std::runtime_error("Bind failed");

        if (listen(serverFds[i], 127) < 0)
            throw std::runtime_error("Listen failed");

        std::cout << "Listening on "
                  << servers[i].getListenIp()
                  << ":"
                  << servers[i].getListenPort()
                  << " (fd : " << serverFds[i] << ")"
                  << std::endl;
    }
}
void ServerSocket::run()
{
    int epollFd = createEpoll();
    registerServerSockets(epollFd);

    struct epoll_event events[128];

    while (true)
    {
        // FIX: timeout changed from -1 (block forever) to 5000ms so that
        // checkTimeouts() fires periodically even when there is no traffic.
        int nfds = epoll_wait(epollFd, events, 128, 5000);
        if (nfds < 0 && errno != EINTR)
            continue;

        for (int i = 0; i < nfds; i++)
        {
            int fd = events[i].data.fd;

            if (isServerFd(fd))
                handleServerEvent(epollFd, fd);
            else
                handleClientEvent(epollFd, fd);
        }
        // FIX: close connections that have been idle for CONNECTION_TIMEOUT_SECS
        checkTimeouts(epollFd);
    }
}
int ServerSocket::createEpoll()
{
    int epollFd = epoll_create1(0);
    if (epollFd < 0)
        throw std::runtime_error("Failed to create epoll instance");

    std::cout << "Epoll FD: " << epollFd << std::endl;
    return epollFd;
}
void ServerSocket::registerServerSockets(int epollFd)
{
    for (size_t i = 0; i < serverFds.size(); i++)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = serverFds[i];

        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFds[i], &ev) < 0)
            throw std::runtime_error("Failed to add server fd to epoll");
    }
}
bool ServerSocket::isServerFd(int fd)
{
    return std::find(serverFds.begin(), serverFds.end(), fd) != serverFds.end();
}

void ServerSocket::handleServerEvent(int epollFd, int serverFd)
{
    while (true)
    {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        int clientFd = accept(serverFd, (sockaddr*)&clientAddr, &clientLen);
        if (clientFd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            else
                throw std::runtime_error("accept failed");
        }

        // Get client IP and port
        std::string clientIp = inet_ntoa(clientAddr.sin_addr);
        int clientPort = ntohs(clientAddr.sin_port);

        // Get server IP and port
        sockaddr_in localAddr;
        socklen_t len = sizeof(localAddr);
        getsockname(clientFd, (sockaddr*)&localAddr, &len);
        
        acceptedIp = inet_ntoa(localAddr.sin_addr);
        acceptedport = ntohs(localAddr.sin_port);

       
        size_t serverIndex = getServerIndexByFd(serverFd);

        std::cout << "Client " << clientIp << ":" << clientPort
                  << " connected to server[" << serverIndex << "] "
                  << acceptedIp << ":" << acceptedport
                  << " (fd: " << clientFd << ")" << std::endl;

        
        connections.push_back(Connection(clientFd, clientIp, clientPort,
                                        acceptedIp, acceptedport, serverIndex));

        // Make client non-blocking
        int flags = fcntl(clientFd, F_GETFL, 0);
        fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);

        // Add client to epoll
        struct epoll_event clientEv;
        clientEv.events = EPOLLIN;
        clientEv.data.fd = clientFd;
       if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &clientEv) < 0)
            throw std::runtime_error("epoll_ctl client add failed");
    }
}

void ServerSocket::closeConnection(int epollFd, int clientFd)
{
    epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, 0);
    close(clientFd);
    removeConnection(clientFd);
}

// FIX: previously this function called responseHandler() which read the entire
// request body and file body into memory before sending a single byte.
// Now:
//   - conn->append() streams the upload body to a temp file (see connection.cpp)
//   - responseBodyFile holds the path of the file to serve (not its content)
//   - conn->setSendFileFd() passes the open fd to handleSending() for chunked sending
void ServerSocket::handleReading(int epollFd, Connection* conn)
{
    int clientFd = conn->getClientFd();
    char buffer[4096];
    int bytes = recv(clientFd, buffer, sizeof(buffer), 0);

    if (bytes <= 0)
    {
        closeConnection(epollFd, clientFd);
        return;
    }

    conn->append(buffer, bytes);
    conn->updateLastActivity();

    if (!conn->isRequestComplete())
        return;

    std::string request = conn->extractRequest();  // headers only
    std::string bodyFilePath = conn->getBodyFilePath(); // upload body on disk
    conn->consumeRequest();

    std::string responseStr;
    std::string responseBodyFile;
    try
    {
        HttpRequest httpRequest;
        httpRequest.requestHandler(request);
        httpRequest.setBodyFilePath(bodyFilePath);

        HttpResponse httpResponse(httpRequest, config, conn->getServerIndex());
        httpResponse.responseHandler();

        responseStr = httpResponse.getResponse();
        responseBodyFile = httpResponse.getResponseBodyFilePath(); // empty for non-file responses
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error handling request: " << e.what() << std::endl;
        responseStr =
            "HTTP/1.0 500 Internal Server Error\r\n"
            "Content-Length: 21\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Internal Server Error";
    }

    if (!bodyFilePath.empty())
        std::remove(bodyFilePath.c_str()); // delete upload temp file after response is built

    conn->setResponse(responseStr); // writeBuffer holds headers only for file responses

    // FIX: open the file to serve — handleSending() streams it without loading into memory
    if (!responseBodyFile.empty())
    {
        int fd = open(responseBodyFile.c_str(), O_RDONLY);
        if (fd >= 0)
            conn->setSendFileFd(fd);
    }

    conn->setState(SENDING);

    struct epoll_event ev;
    ev.events = EPOLLOUT;
    ev.data.fd = clientFd;
    epoll_ctl(epollFd, EPOLL_CTL_MOD, clientFd, &ev);
}

// FIX: previously sent the entire writeBuffer (which contained the whole file body)
// in one shot. For a 1GB video this meant 1GB sat in memory and the event loop was
// occupied until the entire transfer finished, blocking all other clients.
//
// New two-phase design:
//   Phase 1 — send HTTP headers from writeBuffer (a few hundred bytes, instant).
//   Phase 2 — read 64KB from sendFileFd and send it, then RETURN to epoll_wait.
//             The next EPOLLOUT event reads the next 64KB, and so on.
//             Between every 64KB chunk, epoll can serve other clients normally.
void ServerSocket::handleSending(int epollFd, Connection* conn)
{
    int clientFd = conn->getClientFd();
    std::string& wb = conn->getWriteBuffer();
    size_t& sent = conn->getBytesSent();

    // Phase 1: flush HTTP headers
    if (sent < wb.size())
    {
        int bytes = send(clientFd, wb.c_str() + sent, wb.size() - sent, 0);
        if (bytes > 0)
            sent += bytes;
        else if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
            closeConnection(epollFd, clientFd);
        return;
    }

    // Phase 2: stream file body one chunk at a time
    int fileFd = conn->getSendFileFd();
    if (fileFd >= 0)
    {
        char chunk[65536];
        ssize_t bytesRead = read(fileFd, chunk, sizeof(chunk));
        if (bytesRead > 0)
        {
            send(clientFd, chunk, static_cast<size_t>(bytesRead), 0);
            return; // come back next EPOLLOUT for the next chunk
        }
        conn->closeSendFileFd();
    }

    conn->setState(DONE);
    closeConnection(epollFd, clientFd);
}

void ServerSocket::handleClientEvent(int epollFd, int clientFd)
{
    Connection* conn = getConnectionByFd(clientFd);
    if (!conn)
        return;

    if (conn->getState() == READING)
        handleReading(epollFd, conn);
    else if (conn->getState() == SENDING)
        handleSending(epollFd, conn);
}

Connection* ServerSocket::getConnectionByFd(int fd)
{
    for (size_t i = 0; i < connections.size(); i++)
    {
        if (connections[i].getClientFd() == fd)
            return &connections[i];
    }
    return NULL;
}

void ServerSocket::removeConnection(int fd)
{
    for (size_t i = 0; i < connections.size(); i++)
    {
        if (connections[i].getClientFd() == fd)
        {
            connections.erase(connections.begin() + i);//erase take iterator not index
            break;
        }
    }
}

size_t ServerSocket::getServerIndexByFd(int serverFd)
{
    for (size_t i = 0; i < serverFds.size(); i++)
    {
        if (serverFds[i] == serverFd)
            return i;
    }
    return 0;
}

// FIX: added timeout handling — connections idle for CONNECTION_TIMEOUT_SECS (20s)
// are closed automatically. Called after every epoll_wait cycle.
// Don't increment i when erasing — closeConnection() removes the entry from the
// vector so the next element shifts into position i.
void ServerSocket::checkTimeouts(int epollFd)
{
    time_t now = std::time(NULL);
    for (size_t i = 0; i < connections.size(); )
    {
        if (now - connections[i].getLastActivity() > CONNECTION_TIMEOUT_SECS)
        {
            std::cout << "Closing idle connection fd=" << connections[i].getClientFd() << std::endl;
            closeConnection(epollFd, connections[i].getClientFd());
        }
        else
            i++;
    }
}

const std::vector<Connection>& ServerSocket::getConnections() const
{
    return connections;
}