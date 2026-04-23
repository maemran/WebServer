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
#include "connection.hpp"

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
    
    struct epoll_event events[128]; // events[0] → fd 4 is ready events[1] → fd 8 is ready events[2] → fd 10 is ready

    while (true)
    {
        int nfds = epoll_wait(epollFd, events, 128, -1); //Tell me which sockets are ready for something
        //nfds is the numbers of ready sockets
        if (nfds < 0)
            continue;

        for (int i = 0; i < nfds; i++)
        {
            int fd = events[i].data.fd;

            if (isServerFd(fd)) // Because the action is different if server it should go to accept , if clent --> send data recv()
                handleServerEvent(epollFd, fd);
            else
                handleClientEvent(epollFd, fd);
        }
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

    if (!conn->isRequestComplete())
        return;

    std::string request = conn->extractRequest();
    conn->consumeRequest();
    // std::cout << "FULL REQUEST:\n" << request << std::endl;

    std::string responseStr;
    try
    {
        HttpRequest httpRequest;
        httpRequest.requestHandler(request);

        HttpResponse httpResponse(httpRequest, config, conn->getServerIndex());
        httpResponse.responseHandler();

        responseStr = httpResponse.getResponse();
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

    conn->setResponse(responseStr);
    conn->setState(SENDING);

    struct epoll_event ev;
    ev.events = EPOLLOUT;
    ev.data.fd = clientFd;
    epoll_ctl(epollFd, EPOLL_CTL_MOD, clientFd, &ev);
}

void ServerSocket::handleSending(int epollFd, Connection* conn)
{
    int clientFd = conn->getClientFd();
    std::string& wb = conn->getWriteBuffer();
    size_t& sent = conn->getBytesSent();

    int bytes = send(clientFd, wb.c_str() + sent, wb.size() - sent, 0);
    if (bytes > 0)
        sent += bytes;

    if (sent >= wb.size())
    {
        conn->setState(DONE);
        closeConnection(epollFd, clientFd);
    }
    else if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
    {
        closeConnection(epollFd, clientFd);
    }
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

const std::vector<Connection>& ServerSocket::getConnections() const
{
    return connections;
}