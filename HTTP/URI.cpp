/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:52:32 by maemran           #+#    #+#             */
/*   Updated: 2026/02/24 06:53:09 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"

URI::URI() {}

URI::URI(const std::string& uri)
{
    this->uri = uri;
}

URI::URI(const URI& other)
{
    this->uri = other.uri;
    this->scheme = other.scheme;
    this->host = other.host;
    this->path = other.path;
    this->port = other.port;
}

URI& URI::operator=(const URI& other)
{
    if (this != &other)
    {
        this->uri = other.uri;
        this->scheme = other.scheme;
        this->host = other.host;
        this->path = other.path;
        this->port = other.port;
    }
    return *this;
}

URI::~URI() {}

const std::string& URI::getUri() const
{
    return this->uri;
}

const std::string& URI::getScheme() const
{
    return this->scheme;
}

const std::string& URI::getHost() const
{
    return this->host;
}

const std::string& URI::getPath() const
{
    return this->path;
}

unsigned int URI::getPort() const
{
    return this->port;
}

void URI::setUri(const std::string& uri)
{
    this->uri = uri;
}

void URI::setScheme(const std::string& scheme)
{
    this->scheme = scheme;
}

void URI::setHost(const std::string& host)
{
    this->host = host;
}

void URI::setPath(const std::string& path)
{
    this->path = path;
}

void URI::setPort(unsigned int port)
{
    this->port = port;
}
