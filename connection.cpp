#include "connection.hpp"

Connection::Connection(int fd, const std::string& cIp, int cPort,const std::string& sIp, int sPort, size_t sIndex)
    : clientFd(fd), clientIp(cIp), clientPort(cPort),
      serverIp(sIp), serverPort(sPort), serverIndex(sIndex)
{
}

Connection::~Connection()
{
}

Connection::Connection(const Connection& other)
    : clientFd(other.clientFd), clientIp(other.clientIp), 
      clientPort(other.clientPort), serverIp(other.serverIp),
      serverPort(other.serverPort), serverIndex(other.serverIndex)
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