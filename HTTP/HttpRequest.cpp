/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:46:08 by maemran           #+#    #+#             */
/*   Updated: 2026/02/24 07:25:33 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

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
    
}