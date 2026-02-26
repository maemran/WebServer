#include "config/ServerConfig.hpp"

// Default constructor
ServerConfig::ServerConfig()
    : listen_ip("0.0.0.0"), listen_port(80), root(""), index(""), autoindex(false), max_body_size(0)
{}

// Copy constructor
ServerConfig::ServerConfig(const ServerConfig& other)
    : listen_ip(other.listen_ip),
      listen_port(other.listen_port),
      root(other.root),
      index(other.index),
      autoindex(other.autoindex),
      error_pages(other.error_pages),
      allowed_methods(other.allowed_methods),
      max_body_size(other.max_body_size),
      locations(other.locations)
{}

// Assignment operator
ServerConfig& ServerConfig::operator=(const ServerConfig& other)
{
    if (this != &other)
    {
        listen_ip = other.listen_ip;
        listen_port = other.listen_port;
        root = other.root;
        index = other.index;
        autoindex = other.autoindex;
        error_pages = other.error_pages;
        allowed_methods = other.allowed_methods;
        max_body_size = other.max_body_size;
        locations = other.locations;
    }
    return *this;
}

// Destructor
ServerConfig::~ServerConfig() {}

// Getters
const std::string& ServerConfig::getListenIp() const { return listen_ip; }
int ServerConfig::getListenPort() const { return listen_port; }
const std::string& ServerConfig::getRoot() const { return root; }
const std::string& ServerConfig::getIndex() const { return index; }
bool ServerConfig::getAutoindex() const { return autoindex; }
const std::vector<std::string>& ServerConfig::getMethods() const { return allowed_methods; }
const std::map<int, std::string>& ServerConfig::getErrorPages() const { return error_pages; }
size_t ServerConfig::getMaxBodySize() const { return max_body_size; }
const std::vector<LocationConfig>& ServerConfig::getLocations() const { return locations; }

// Setters
void ServerConfig::setListenIp(const std::string& ip) { listen_ip = ip; }
void ServerConfig::setListenPort(int port) { listen_port = port; }
void ServerConfig::setRoot(const std::string& r) { root = r; }
void ServerConfig::setIndex(const std::string& i) { index = i; }
void ServerConfig::setAutoindex(bool a) { autoindex = a; }
void ServerConfig::addAllowedMethod(const std::string& method) { allowed_methods.push_back(method); }
void ServerConfig::addErrorPage(int code, const std::string& page) { error_pages[code] = page; }
void ServerConfig::setMaxBodySize(size_t size) { max_body_size = size; }
void ServerConfig::addLocation(const LocationConfig& location) { locations.push_back(location); }