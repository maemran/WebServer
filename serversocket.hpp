#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include "ServerConfig.hpp"
#include "HttpConfig.hpp"
class ServerSocket
{
    private:
        std::vector<int> serverFds;
        std::vector<ServerConfig> servers;
        std :: string acceptedIp;
        int acceptedport;
    public:
        ServerSocket(const HttpConfig& config);
        ~ServerSocket();
        ServerSocket(const ServerSocket& other);
        ServerSocket&	operator=(const ServerSocket& other);
        void start();
        void  run();
        int   createEpoll();
        void  registerServerSockets(int epollFd);
        void  handleServerEvent(int epollFd, int serverFd);
        void  handleClientEvent(int epollFd, int clientFd);
        bool  isServerFd(int fd);
        std :: string getAcceptedIp(std :: string acceptedIp);
        int getAcceptedPort(int acceptedPort);
};

#endif