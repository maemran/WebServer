/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:46:08 by maemran           #+#    #+#             */
/*   Updated: 2026/02/25 15:51:19 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <vector>
#include <iostream>

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest& other)
{
    // this->request = other.request;
    this->requestLine = other.requestLine;
    this->method = other.method;
    this->uri = other.uri;
    this->httpVersion = other.httpVersion;
    this->headers = other.headers;
    this->entityBody = other.entityBody;
}
HttpRequest&    HttpRequest::operator=(const HttpRequest& other)
{
    if (this != &other)
    {
        // this->request = other.request;
        this->requestLine = other.requestLine;
        this->method = other.method;
        this->uri = other.uri;
        this->httpVersion = other.httpVersion;
        this->headers = other.headers;
        this->entityBody = other.entityBody;
    }
    return *this;
}

HttpRequest::~HttpRequest() {}

const std::string& HttpRequest::getRequestLine() const
{
    return this->requestLine;
}

const std::string& HttpRequest::getMethod() const
{
    return this->method;
}

const URI& HttpRequest::getUri() const
{
    return this->uri;
}

const std::string& HttpRequest::getHttpVersion() const
{
    return this->httpVersion;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const
{
    return this->headers;
}

const std::string& HttpRequest::getEntityBody() const
{
    return this->entityBody;
}

void    HttpRequest::setRequestLine(const std::string& requestLine)
{
    this->requestLine = requestLine;
}

void    HttpRequest::setMethod(const std::string& method)
{
    this->method = method;
}

void    HttpRequest::setUri(const URI& uri)
{
    this->uri = uri;
}

void    HttpRequest::setHttpVersion(const std::string& httpVersion)
{
    this->httpVersion = httpVersion;
}

void    HttpRequest::setHeaders(const std::map<std::string, std::string>& headers)
{
    this->headers = headers;
}

void    HttpRequest::setEntityBody(const std::string& entityBody)
{
    this->entityBody = entityBody;
}

void    HttpRequest::requestParsing(const std::string& request)
{
    int i = 0;
	int start = 0;
	int end = 0;
	std::string temp;
	std::vector<std::string>	requestElements;
	while (i < (int)request.length())
	{
		if (i != 0 && (request[i - 1] == '\r' && request[i] == '\n'))
		{
			for (; start < i - 1; start++)
				temp += request[start];
			start = i + 1;
			requestElements.push_back(temp);
			temp = "";
		}
		i++;
	}
	for (int i = (int)request.length() - 1; i >= 0; i--)
	{
		if (i != 0 && (request[i - 3] == '\r' && request[i - 2] == '\n'
			&& request[i - 1] == '\r' && request[i] == '\n'))
		{	
			end = i + 1;
			break;
		}
	}
	for (int i = end; i < (int)request.length(); i++)
		this->entityBody += request[i];
	for (int i = 0; i < (int)(requestElements.size()); i++)
		std::cout << requestElements[i] << std::endl;
	std::cout << this->entityBody << std::endl;
}