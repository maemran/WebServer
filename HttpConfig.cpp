/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 07:05:07 by maemran           #+#    #+#             */
/*   Updated: 2026/02/24 07:06:49 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConfig.hpp"

HttpConfig::HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& other)
{
    *this = other;
}

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

HttpConfig::~HttpConfig() {}

const std::string& HttpConfig::getRoot() const
{
    return root;
}

const std::string& HttpConfig::getIndex() const
{
    return index;
}

const bool HttpConfig::getAutoindex() const
{
    return autoindex;
}

const size_t HttpConfig::getMaxBodySize() const
{
    return max_body_size;
}

const std::map<int, std::string>& HttpConfig::getErrorPages() const
{
    return error_pages;
}

const std::vector<std::string>& HttpConfig::getMethods() const
{
    return allowed_methods;
}

const std::vector<ServerConfig>& HttpConfig::getServers() const
{
    return servers;
}

void HttpConfig::setRoot(const std::string& root)
{
    this->root = root;
}

void HttpConfig::setIndex(const std::string& index)
{
    this->index = index;
}

void HttpConfig::setAutoindex(bool autoindex)
{
    this->autoindex = autoindex;
}

void HttpConfig::setMaxBodySize(size_t size)
{
    this->max_body_size = size;
}

void HttpConfig::addErrorPage(int code, const std::string& page)
{
    error_pages[code] = page;
}

void HttpConfig::addAllowedMethod(const std::string& method)
{
    allowed_methods.push_back(method);
}

void HttpConfig::addServer(const ServerConfig& server)
{
    servers.push_back(server);
}


