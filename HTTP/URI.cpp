/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:52:32 by maemran           #+#    #+#             */
/*   Updated: 2026/02/28 14:26:29 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"

URI::URI()
{
	uriError = 0;
}

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

int URI::getUriError() const
{
	return this->uriError;
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

void URI::setUriError(int uriError)
{
	this->uriError = uriError;
}

void     URI::uriCheck()
{
	int colonFlag = 0;
	if (uri == "")
		uriError = 1;
	if (uri[0] == ' ')
		uriError = 1;
	for (int i = 0; i < (int)uri.length(); i++)
	{
		if (uri[i] == ' ')
			uriError = 1;
		return;
	}
	if (uri[0] != '/')
	{
		for (int i = 0; i < ((int)uri.length()); i++)
		{
			if (uri[i] == ':')
				colonFlag++;
		}
		if (colonFlag == 0)
		{
			uriError = 1;
			return;
		}
		if(uri.find(":///") != std::string::npos
			|| uri.find("///") != std::string::npos)
			uriError = 1;
		for (int i = 0; i < (int)uri.length(); i++)
		{
			if (i <= ((int)uri.length() - 3) && uri[i] == ':'
				&& uri[i + 1] == '/' && uri[i + 2] != '/')
			{
				uriError = 1;
				break;
			}
		}
		for (int i = 0; i < (int)uri.length(); i++)
		{
			if (i <= ((int)uri.length() - 3) && uri[i] == ':'
				&& uri[i + 1] != '/' && uri[i + 2] != '/' 
				&& uri.find("://") == std::string::npos)
			{
				uriError = 1;
				break;
			}
		}
	}
}

//void    uriParser(const std::string& request);
