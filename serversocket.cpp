/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serversocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadeh <asaadeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 01:30:51 by asaadeh           #+#    #+#             */
/*   Updated: 2026/03/07 02:58:48 by asaadeh          ###   ########.fr       */
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

ServerSocket::ServerSocket(const HttpConfig& config)
{
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
 std :: string ServerSocket :: getAcceptedIp(std :: string acceptedIp)
 {
    this->acceptedIp = acceptedIp;
 }
 int ServerSocket :: getAcceptedPort(int acceptedPort)
 {
    return this->acceptedport = acceptedPort;
 }
void ServerSocket::start()
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        serverFds[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFds[i] < 0)
            throw std::runtime_error("Socket creation failed");

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

// void ServerSocket::run()
// {
//     // Create epoll instance
//     int epollFd = epoll_create1(0);
//     std :: cout << "the fd of epoll is : "<<epollFd << std :: endl;
//     if (epollFd < 0)
//         throw std::runtime_error("Failed to create epoll instance");

//     // Map server fd → port for printing
//     std::map<int, int> serverFdToPort;
//     for (size_t i = 0; i < serverFds.size(); i++)
//         serverFdToPort[serverFds[i]] = servers[i].getListenPort();

//     // Register all server sockets with epoll
//     for (size_t i = 0; i < serverFds.size(); i++)
//     {
//         struct epoll_event ev;
//         ev.events = EPOLLIN; // ready to read → incoming connection
//         ev.data.fd = serverFds[i];

//         if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFds[i], &ev) < 0)
//             throw std::runtime_error("Failed to add server fd to epoll");
//     }

//     struct epoll_event events[128]; // max 128 events per loop

//     while (true)
//     {
//         int nfds = epoll_wait(epollFd, events, 128, -1); // wait indefinitely
//         if (nfds < 0)
//             continue;

//         for (int i = 0; i < nfds; i++)
//         {
//             int fd = events[i].data.fd;

//             // Check if fd is a server socket (incoming connection)
//             if (std::find(serverFds.begin(), serverFds.end(), fd) != serverFds.end())
//             {
//                 // Accept all pending clients
//                 while (true)
//                 {
//                     int clientFd = accept(fd, 0, 0);
//                     if (clientFd < 0)
//                     {
//                         if (errno == EAGAIN || errno == EWOULDBLOCK)
//                             break; // no more clients pending
//                         else
//                             throw std::runtime_error("accept failed");
//                     }

//                     // Make client socket non-blocking
//                     int flags = fcntl(clientFd, F_GETFL, 0);
//                     fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);

//                     // Add clientFd to epoll to monitor for data
//                     struct epoll_event clientEv;
//                     clientEv.events = EPOLLIN;
//                     clientEv.data.fd = clientFd;
//                     epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &clientEv);

//                     // Correct port printing
//                     int serverPort = serverFdToPort[fd];
//                     std::cout << "Client connected on port "
//                               << serverPort
//                               << " (fd: " << clientFd << ")"
//                               << std::endl;
//                 }
//             }
//             else
//             {
//                 // Client socket ready to read
//                 char buffer[4096];
//                 int bytes = recv(fd, buffer, sizeof(buffer), 0);
//                 if (bytes <= 0)
//                 {
//                     // Client disconnected
//                     epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, 0);
//                     close(fd);
//                 }
//                 else
//                 {
//                     // Hard-coded HTTP response
//                     const char* response =
//                         "HTTP/1.1 200 OK\r\n"
//                         "Content-Length: 13\r\n"
//                         "Content-Type: text/plain\r\n"
//                         "\r\n"
//                         "Hello, world!";

//                     send(fd, response, strlen(response), 0);
//                     epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, 0);
//                     close(fd);
//                 }
//             }
//         }
//     }
// }

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
        int clientFd = accept(serverFd, 0, 0);
        if (clientFd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            else
                throw std::runtime_error("accept failed");
        }
        //----------------------------------------------------
        sockaddr_in localAddr;
        socklen_t len = sizeof(localAddr); //How big is the structure where I will write the address?

        getsockname(clientFd, (sockaddr*)&localAddr, &len);// it fill the localAddr

        acceptedIp = inet_ntoa(localAddr.sin_addr); // it stored in binary format so we convert it to string
        acceptedport = ntohs(localAddr.sin_port); // network --> text;

        std::cout << "Client connected to server "
                << acceptedIp << ":" << acceptedport
                << " (fd: " << clientFd << ")"
                << std::endl;

        //-------------------------------------------------------------
        // Make client non-blocking
        int flags = fcntl(clientFd, F_GETFL, 0);
        fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);

        // Add client to epoll
        struct epoll_event clientEv;
        clientEv.events = EPOLLIN;
        clientEv.data.fd = clientFd;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &clientEv);

        std::cout << "New client fd: " << clientFd << std::endl;
    }
}
void ServerSocket::handleClientEvent(int epollFd, int clientFd)
{
    char buffer[4096];
    int bytes = recv(clientFd, buffer, sizeof(buffer), 0);

    if (bytes <= 0)
    {
        epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, 0);
        close(clientFd);
        return;
    }

    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 13\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello, world!";

    send(clientFd, response, strlen(response), 0);

    epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, 0);
    close(clientFd);
}