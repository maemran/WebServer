#include "config/HttpConfig.hpp"

HttpConfig::HttpConfig() : autoindex(false), max_body_size(0) {}
HttpConfig::~HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& other) { *this = other; }

HttpConfig& HttpConfig::operator=(const HttpConfig& other)
{
    if (this != &other)
    {
        root = other.root;
        index = other.index;
        autoindex = other.autoindex;
        max_body_size = other.max_body_size;
        error_pages = other.error_pages;
        allowed_methods = other.allowed_methods;
        servers = other.servers;
    }
    return *this;
}

void HttpConfig::addServer(const ServerConfig& server)
{
    servers.push_back(server);
}

void HttpConfig::setMaxBodySize(size_t size)
{
    max_body_size = size;
}

const std::vector<ServerConfig>& HttpConfig::getServers() const
{
    return servers;
}