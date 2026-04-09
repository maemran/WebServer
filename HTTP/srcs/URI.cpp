/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:52:32 by maemran           #+#    #+#             */
/*   Updated: 2026/04/09 20:19:41 by maemran          ###   ########.fr       */
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
		if ((uri[i] >= 9 && uri[i] <= 13) || uri[i] == ' ')
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
        std::string subUri = uri.substr(0, 7);
		if (subUri.find("http://") == std::string::npos)
			throw badURIException("Scheme Error");
	}
}

void    URI::uriParser()
{
    int i = 7;
    if (uri[0] != '/')
    {
        this->scheme = uri.substr(0, 7);
        for (; i < (int)uri.length(); i++)
        {
            if (uri[i] == '/')
                break;
            this->host += uri[i];
        }
        for (; i < (int)uri.length(); i++)
            this->path += uri[i];
    }
    else
        this->path = uri;
}

void    URI::uriValidate()
{
    if (path.find("<") != std::string::npos 
        || path.find(">") != std::string::npos
        || path.find("\\") != std::string::npos
        || path.find("{") != std::string::npos
        || path.find("}") != std::string::npos)
		throw badURIException("Wrong Path");
}

void    URI::printClassAtributes()
{
    std::cout << "URI Scheme: " << this->scheme << std::endl;
    std::cout << "URI Host: " << this->host << std::endl;
    std::cout << "URI Path: " << this->path << std::endl;
}

void    URI::uriHandler()
{
    uriCheck();
    uriParser();
    uriValidate();
    printClassAtributes();
}
