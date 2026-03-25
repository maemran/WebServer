/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:18:49 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/04 15:45:02 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/HttpConfig.hpp"

HttpConfig::HttpConfig() : autoindex(false), client_max_body_size(0) {}
HttpConfig::~HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& other) { *this = other; }

HttpConfig& HttpConfig::operator=(const HttpConfig& other)
{
    if (this != &other)
    {
        root = other.root;
        autoindex = other.autoindex;
        client_max_body_size = other.client_max_body_size;
        error_pages = other.error_pages;
        allowed_methods = other.allowed_methods;
        servers = other.servers;
    }
    return *this;
}

/* ========================================= */
/*               SETTERS                     */
/* ========================================= */

void HttpConfig::setRoot(const std::string& r)
{
    root = r;
}


void HttpConfig::setAutoindex(bool value)
{
    autoindex = value;
}

void HttpConfig::setMaxBodySize(size_t size)
{
    client_max_body_size = size;
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

/* ========================================= */
/*               GETTERS                     */
/* ========================================= */

const std::string& HttpConfig::getRoot() const
{
    return root;
}


bool HttpConfig::getAutoindex() const
{
    return autoindex;
}

size_t HttpConfig::getMaxBodySize() const
{
    return client_max_body_size;
}

const std::map<int, std::string>&
HttpConfig::getErrorPages() const
{
    return error_pages;
}

const std::vector<std::string>&
HttpConfig::getMethods() const
{
    return allowed_methods;
}

const std::vector<ServerConfig>&
HttpConfig::getServers() const
{
    return servers;
}

std::vector<ServerConfig>&
HttpConfig::getServers()
{
    return servers;
}

void HttpConfig::addIndexFile(const std::string& file)
{
    indexFiles.push_back(file);
}

const std::vector<std::string>& HttpConfig::getIndexFiles() const
{
    return indexFiles;
}