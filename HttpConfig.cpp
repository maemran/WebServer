#include "HttpConfig.hpp"
HttpConfig::HttpConfig()
{
    addServer(ServerConfig());
}

HttpConfig::HttpConfig(const HttpConfig& other)
    : servers(other.servers)
{
}

HttpConfig::~HttpConfig()
{
}

HttpConfig& HttpConfig::operator=(const HttpConfig& other)
{
    if (this != &other)
    {
        servers = other.servers;
    }
    return *this;
}

const std::vector<ServerConfig>& HttpConfig::getServers() const
{
    return servers;
}
void HttpConfig:: addServer(const ServerConfig& /*server*/)
{
    ServerConfig copy;
    copy.setListenIp("127.0.0.1");
    copy.setListenPort(8080);
    servers.push_back(copy);

    ServerConfig copy1;
    copy1.setListenIp("127.0.0.2");
    copy1.setListenPort(9090);
    servers.push_back(copy1);

    ServerConfig copy2;
    copy2.setListenIp("127.0.0.3");
    copy2.setListenPort(5050);
    servers.push_back(copy2);

    ServerConfig copy3;
    copy2.setListenIp("");
    copy2.setListenPort(9090);
    servers.push_back(copy2);

}