/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:51:29 by maemran           #+#    #+#             */
/*   Updated: 2026/03/11 18:49:05 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(const HttpResponse& other)
{
    this->response = other.response;
    this->request = other.request;
    this->config = other.config;
    this->serverIndex = other.serverIndex;
}
HttpResponse&   HttpResponse::operator=(const HttpResponse& other)
{
    if (this != &other)
    {
        this->response = other.response;
        this->request = other.request;
        this->config = other.config;
        this->serverIndex = other.serverIndex;
    }
    return *this;
}

HttpResponse::~HttpResponse() {}

const std::string& HttpResponse::getResponse() const
{
    return response;
}

const HttpRequest& HttpResponse::getRequest() const
{
    return request;
}

const HttpConfig& HttpResponse::getConfig() const
{
    return config;
}

int HttpResponse::getServerIndex() const
{
    return serverIndex;
}

void  HttpResponse::setResponse(const std::string& response)
{
    this->response = response;
}

void  HttpResponse::setRequest(const HttpRequest& request)
{
    this->request = request;
}

void  HttpResponse::setConfig(const HttpConfig& config)
{
    this->config = config;
}

void  HttpResponse::setServerIndex(int serverIndex)
{
    this->serverIndex = serverIndex;
}

void    HttpResponse::generate_default_error_page(std::string& statusCode, std::string&  ReasonPhrase)
{
    std::string errorPage = 
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head> <title> Error page </title> </head>\n"
    "<body>\n"
    "<h1>";
    errorPage.append(statusCode);
    std::string secondPart = 
    "</h1>\n"
    "<hr>\n<br>\n<p>";
    secondPart.append(ReasonPhrase);
    std::string thirdPart;
}

std::string HttpResponse::generateResponse()
{
    if (request.getStatusCode() != "200")
    

    return 0;
}