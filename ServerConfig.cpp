#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : listen_ip(""), listen_port(0)
{
}

ServerConfig::ServerConfig(const ServerConfig& other)
    : listen_ip(other.listen_ip), listen_port(other.listen_port)
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other)
{
    if (this != &other)
    {
        listen_ip = other.listen_ip;
        listen_port = other.listen_port;
    }
    return *this;
}

const std::string& ServerConfig::getListenIp() const
{
    return listen_ip;
}

int ServerConfig::getListenPort() const
{
    return listen_port;
}

void ServerConfig::setListenIp(const std::string& ip)
{
    listen_ip = ip;
}

void ServerConfig::setListenPort(int port)
{
    listen_port = port;
}