#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include "ServerConfig.hpp"
#include "HttpConfig.hpp"
#include "connection.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
class ServerSocket
{
    private:
        std::vector<int> serverFds;
        std::vector<ServerConfig> servers;
        HttpConfig config;
        std :: string acceptedIp;
        int acceptedport;

        std::vector<Connection> connections;
        Connection* getConnectionByFd(int fd);
        void removeConnection(int fd);
        size_t getServerIndexByFd(int serverFd);
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
        void  handleReading(int epollFd, Connection* conn);
        void  handleSending(int epollFd, Connection* conn);
        void  closeConnection(int epollFd, int clientFd);
        bool  isServerFd(int fd);
        std :: string getAcceptedIp(std :: string acceptedIp);
        int getAcceptedPort(int acceptedPort);

        const std::vector<Connection>& getConnections() const;
};

#endif