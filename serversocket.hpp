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

    public:
        ServerSocket(const HttpConfig& config);
        ~ServerSocket();
        ServerSocket(const ServerSocket& other);
        ServerSocket&	operator=(const ServerSocket& other);
        void start();
        void run();
};

#endif