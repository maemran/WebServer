/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:52:32 by maemran           #+#    #+#             */
/*   Updated: 2026/03/04 15:43:12 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"
#include <iostream>

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
}

URI& URI::operator=(const URI& other)
{
    if (this != &other)
    {
        this->uri = other.uri;
        this->scheme = other.scheme;
        this->host = other.host;
        this->path = other.path;
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

URI::badURIException::badURIException(const char *errMessage)
{
	this->errMessage = const_cast<char *>(errMessage);
}

const char* URI::badURIException::what() const throw()
{
	return errMessage;
}

void	URI::pathWithSpacesCheck()
{
	for (int i = 0; i < (int)uri.length(); i++)
	{
		if (uri[i] == ' ')
			throw	badURIException("URL contain spaces");
	}
}

void     URI::uriCheck()
{
	if (uri == "")
		throw badURIException("Empty URL");
	pathWithSpacesCheck();
	if (uri[0] != '/')
	{
		/* http:///webwhiteboard.com:9090///var/ */
		if(uri.find(":///") != std::string::npos
			|| uri.find("///") != std::string::npos)
			throw badURIException("Multi forword slash");
		/* Scheme check */
		if (uri.find("http://") == std::string::npos
			|| uri[0] != 'h')
			throw badURIException("Scheme Error");
	}
}

//void    uriParser(const std::string& request);
