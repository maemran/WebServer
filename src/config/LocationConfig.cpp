#include "config/LocationConfig.hpp"

// Default constructor
LocationConfig::LocationConfig()
    : path(""), root(""), index(""), autoindex(false), max_body_size(0)
{}

// Copy constructor
LocationConfig::LocationConfig(const LocationConfig& other)
    : path(other.path),
      root(other.root),
      index(other.index),
      autoindex(other.autoindex),
      error_pages(other.error_pages),
      redirections(other.redirections),
      allowed_methods(other.allowed_methods),
      max_body_size(other.max_body_size)
{}

// Assignment operator
LocationConfig& LocationConfig::operator=(const LocationConfig& other)
{
    if (this != &other)
    {
        path = other.path;
        root = other.root;
        index = other.index;
        autoindex = other.autoindex;
        error_pages = other.error_pages;
        redirections = other.redirections;
        allowed_methods = other.allowed_methods;
        max_body_size = other.max_body_size;
    }
    return *this;
}

// Destructor
LocationConfig::~LocationConfig() {}

// Getters
const std::string& LocationConfig::getPath() const { return path; }
const std::string& LocationConfig::getRoot() const { return root; }
const std::string& LocationConfig::getIndex() const { return index; }
bool LocationConfig::getAutoindex() const { return autoindex; }
const std::vector<std::string>& LocationConfig::getMethods() const { return allowed_methods; }
const std::map<int, std::string>& LocationConfig::getErrorPages() const { return error_pages; }
const std::map<int, std::string>& LocationConfig::getRedirections() const { return redirections; }
size_t LocationConfig::getMaxBodySize() const { return max_body_size; }

// Setters
void LocationConfig::setPath(const std::string& p) { path = p; }
void LocationConfig::setRoot(const std::string& r) { root = r; }
void LocationConfig::setIndex(const std::string& i) { index = i; }
void LocationConfig::setAutoindex(bool a) { autoindex = a; }
void LocationConfig::addAllowedMethod(const std::string& method) { allowed_methods.push_back(method); }
void LocationConfig::addErrorPage(int code, const std::string& page) { error_pages[code] = page; }
void LocationConfig::addRedirection(int code, const std::string& url) { redirections[code] = url; }
void LocationConfig::setMaxBodySize(size_t size) { max_body_size = size; }