/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 07:07:26 by maemran           #+#    #+#             */
/*   Updated: 2026/02/24 07:08:39 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig& other)
{
    *this = other;
}

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

ServerConfig::~ServerConfig() {}

const std::string& ServerConfig::getListenIp() const
{
    return listen_ip;
}

const int ServerConfig::getListenPort() const
{
    return listen_port;
}

const std::string& ServerConfig::getRoot() const
{
    return root;
}

const std::string& ServerConfig::getIndex() const
{
    return index;
}

const bool ServerConfig::getAutoindex() const
{
    return autoindex;
}

const std::vector<std::string>& ServerConfig::getMethods() const
{
    return allowed_methods;
}

const std::map<int, std::string>& ServerConfig::getErrorPages() const
{
    return error_pages;
}

const size_t ServerConfig::getMaxBodySize() const
{
    return max_body_size;
}

const std::vector<LocationConfig>& ServerConfig::getLocations() const
{
    return locations;
}

void ServerConfig::setListenIp(const std::string& ip)
{
    listen_ip = ip;
}

void ServerConfig::setListenPort(int port)
{
    listen_port = port;
}

void ServerConfig::setRoot(const std::string& root)
{
    this->root = root;
}

void ServerConfig::setIndex(const std::string& index)
{
    this->index = index;
}

void ServerConfig::setAutoindex(bool autoindex)
{
    this->autoindex = autoindex;
}

void ServerConfig::addAllowedMethod(const std::string& method)
{
    allowed_methods.push_back(method);
}

void ServerConfig::addErrorPage(int code, const std::string& page)
{
    error_pages[code] = page;
}

void ServerConfig::setMaxBodySize(size_t size)
{
    max_body_size = size;
}

void ServerConfig::addLocation(const LocationConfig& location)
{
    locations.push_back(location);
}

