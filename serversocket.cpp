#include "serversocket.hpp"
#include "HttpConfig.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

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

void ServerSocket::start()
{
    for (size_t i = 0; i < servers.size(); i++)
    {
      
        serverFds[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFds[i] < 0)
            throw std::runtime_error("Socket creation failed");
        
        sockaddr_in address;
        std::memset(&address, 0, sizeof(address)); // Because sockaddr_in may contain garbage values We reset everything to 0.
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(servers[i].getListenIp().c_str());//inet_addr() → converts text IP → binary format
        address.sin_port = htons(servers[i].getListenPort()); // network use big endian for storing the bytes but my os use the little indian so we convert it 


        if (bind(serverFds[i], (sockaddr*)&address, sizeof(address)) < 0)
            throw std::runtime_error("Bind failed");

        if (listen(serverFds[i], 127) < 0)
            throw std::runtime_error("Listen failed");

        std::cout << "Listening on "
                  << servers[i].getListenIp()
                  << ":"
                  << servers[i].getListenPort()
                  << " (fd : " << serverFds[i]
                  << " )"
                  << std::endl;
    }
}

void ServerSocket::run()
{
    // Phase 3: Runtime loop
    while (true)
    {
        for (size_t i = 0; i < serverFds.size(); i++)
        {
            int clientFd = accept(serverFds[i], NULL, NULL);
            if (clientFd < 0)
                continue;

            std::cout << "Client connected on port "
                      << servers[i].getListenPort()
                      << std::endl;

            const char* response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 13\r\n"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "Hello, world!";

            send(clientFd, response, strlen(response), 0);
            close(clientFd);
        }
    }
}