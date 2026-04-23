/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:19:06 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/23 23:03:28 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
ServerConfig::ServerConfig()
    : listen_ip("0.0.0.0"), listen_port(80), root(""), autoindex(false), client_max_body_size(0)
{}

ServerConfig::ServerConfig(const ServerConfig& other)
    : listen_ip(other.listen_ip),
      listen_port(other.listen_port),
      root(other.root),
      autoindex(other.autoindex),
      error_page(other.error_page),
      allowed_methods(other.allowed_methods),
      client_max_body_size(other.client_max_body_size),
      locations(other.locations)
{}

ServerConfig& ServerConfig::operator=(const ServerConfig& other)
{
    if (this != &other)
    {
        listen_ip = other.listen_ip;
        listen_port = other.listen_port;
        root = other.root;
        autoindex = other.autoindex;
        error_page = other.error_page;
        allowed_methods = other.allowed_methods;
        client_max_body_size = other.client_max_body_size;
        locations = other.locations;
    }
    return *this;
}

ServerConfig::~ServerConfig() {}
const std::string& ServerConfig::getListenIp() const { return listen_ip; }
int ServerConfig::getListenPort() const { return listen_port; }
const std::string& ServerConfig::getRoot() const { return root; }
bool ServerConfig::getAutoindex() const { return autoindex; }
const std::vector<std::string>& ServerConfig::getMethods() const { return allowed_methods; }
const std::map<int, std::string>& ServerConfig::getErrorPages() const { return error_page; }
size_t ServerConfig::getMaxBodySize() const { return client_max_body_size; }
const std::vector<LocationConfig>& ServerConfig::getLocations() const { return locations; }
std::vector<LocationConfig>& ServerConfig::getLocations() {return this->locations;}
void ServerConfig::setListenIp(const std::string& ip) { listen_ip = ip; }
void ServerConfig::setListenPort(int port) { listen_port = port; }
void ServerConfig::setRoot(const std::string& r) { root = r; }
void ServerConfig::setAutoindex(bool a) { autoindex = a; }
void ServerConfig::addAllowedMethod(const std::string& method) { allowed_methods.push_back(method); }
void ServerConfig::addErrorPage(int code, const std::string& page) { error_page[code] = page; }
void ServerConfig::setMaxBodySize(size_t size) { client_max_body_size = size; }
void ServerConfig::addLocation(const LocationConfig& location) { locations.push_back(location); }
void ServerConfig::addIndexFile(const std::string& file){indexFiles.push_back(file);}
const std::vector<std::string>& ServerConfig::getIndexFiles() const{return indexFiles;}

void ServerConfig::resolveInheritance()
{
    for (size_t i = 0; i < locations.size(); i++)
    {
        if (locations[i].getRoot().empty())
            locations[i].setRoot(root);

        if (locations[i].getIndexFiles().empty())
            locations[i].addIndexFile(indexFiles.front());
    }
}