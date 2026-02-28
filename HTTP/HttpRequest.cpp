/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:46:08 by maemran           #+#    #+#             */
/*   Updated: 2026/02/28 15:01:00 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <iostream>

HttpRequest::HttpRequest()
{
    statusCode = 200;
}

HttpRequest::HttpRequest(const HttpRequest& other)
{
    // this->request = other.request;
    this->requestLine = other.requestLine;
    this->method = other.method;
    this->uri = other.uri;
    this->httpVersion = other.httpVersion;
    this->headers = other.headers;
    this->entityBody = other.entityBody;
    this->statusCode = other.statusCode;
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
        this->statusCode = other.statusCode;
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

URI& HttpRequest::getUri()
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

int HttpRequest::getStatusCode() const
{
    return this->statusCode;
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

void    HttpRequest::setStatusCode(int StatusCode)
{
    this->statusCode = StatusCode;
}

int HttpRequest::requestCheck(const std::string& request)
{
    int flag1 = 0, flag2 = 0, flag3 = 1, flag4 = 1;
    if(request.c_str()[0] == '\r' && request.c_str()[1] == '\n'
        && request.c_str()[2] == '\0')
        flag4 = 0;
    for (int i = 0; i < (int)request.length(); i++)
    {
        if (i != 0 && request[i - 1] == '\r' && request[i] == '\n')
            flag1 = 1;
    }
    for (int i = (int)request.length() - 1; i >= 0; i--)
    {
        if (request[(int)request.length() - 1] == '\n'
            && request[(int)request.length() - 2] == '\r')
        {
            flag2 = 1;
            break;
        }
        if (i > 2 && (request[i - 3] == '\r' && request[i - 2] == '\n'
			&& request[i - 1] == '\r' && request[i] == '\n'))
            flag2 = 1;
    }
    for (int i = 0; i < (int)request.size(); i++)
    {
        if (i != 0 && ((request[i - 1] == '\r' && request[i] != '\n')
            || (request[i - 1] != '\r' && request[i] == '\n')))
            flag3 = 0;
    }
    if (flag1 == 0 || flag2 == 0 || flag3 == 0 || flag4 == 0)
    {
        statusCode = 400;
        return 0;
    }
    return 1;
}

void	HttpRequest::requestLineParser()
{
	int start = 0;
	int end = 0;
	int flag = 0;
	std::string temp;
	std::vector<std::string>    tempVector;
	const char    *str = requestLine.c_str();

	for (int i = 0; i < (int)requestLine.length(); i++)
	{
		if (requestLine[i] == ' ')
			flag++;
	}
	if (flag == 1)
	{
		for (int i = 0; i < (int)requestLine.length(); i++)
    	{
    	    if (((((str[i] >= 0 && str[i] <= 126) && str[i] != 32) && str[i + 1] == ' ')
    	    	|| (((str[i] >= 0 && str[i] <= 126) && str[i] != 32) && str[i + 1] == '\0')))
    	    {
    	        for (; start < i + 1; start++)
					temp += requestLine[start];
    	        start = i + 2;
    	        tempVector.push_back(temp);
    	        temp = "";
    	    }
    	}
		this->method = tempVector[0];
		if (tempVector.size() >= 2)
			this->uri.setUri(tempVector[1]);
		return ;
	}
	for(int i = 0; i < (int)requestLine.length(); i++)
	{
		if (str[i + 1] == ' ' || str[i + 1] == '\0')
		{
			for (; start < i + 1; start++)
				temp += requestLine[start];
			if (str[i + 1] != '\0')
				start = i + 2;
			else
				start = -1;
			break;
		}
	}
	if (start == -1)
	{
		this->method = temp;
		return;
	}
	tempVector.push_back(temp);
	temp = "";
	for (int i = ((int)requestLine.length() - 1); i >= 0; i--)
	{
		if (str[i] == ' ')
		{
			for (end = i + 1; end < (int)requestLine.length(); end++)
				temp += requestLine[end];
			end = i - 1;
			break;
		}
	}
	tempVector.push_back(temp);
	temp = "";
	for (; start <= end; start++)
		temp += requestLine[start];
	tempVector.push_back(temp);
	this->method = tempVector[0];
    if (tempVector.size() >= 2)
		this->httpVersion = tempVector[1];
    if (tempVector.size() >= 3)
		this->uri.setUri(tempVector[2]);
}

void    HttpRequest::storingHeaders(std::vector<std::string> requestElements)
{
    int start;
    int flag;
    std::string temp1, temp2;
    for (int i = 1; i < (int)requestElements.size(); i++)
    {
        start = 0, flag = 0;
        temp1 = "", temp2 = "";
        for (int j = 0; j < (int)requestElements[i].length(); j++)
        {
            if (requestElements[i][j] == ':')
            {
                flag = 1;
                for (; start < j; start++)
				    temp1 += requestElements[i][start];
                j++;
                for (; j < (int)requestElements[i].length(); j++)
                    temp2 += requestElements[i][j];
            }
        }
        if (flag != 1)
            this->headers.insert(std::make_pair(requestElements[i], ""));
        else
            this->headers.insert(std::make_pair(temp1, temp2));
    }
}

void    HttpRequest::requestParser(const std::string& request)
{
    int i = 0;
	int start = 0;
	int end = 0;
	std::string temp;
	std::vector<std::string>	requestElements;
    /*Request line & headers Parser*/
	while (i < (int)request.length())
	{
		if (i != 0 && (request[i - 1] == '\r' && request[i] == '\n'))
		{
			for (; start < i - 1; start++)
				temp += request[start];
			start = i + 1;
            if (temp != "")
			    requestElements.push_back(temp);
			temp = "";
		}
		i++;
	}
    this->requestLine = requestElements[0];
    requestLineParser();
    storingHeaders(requestElements);
    /*Entity Body parser*/
	for (int i = (int)request.length() - 1; i >= 0; i--)
	{
        if (request[(int)request.length() - 1] == '\n'
            && request[(int)request.length() - 2] == '\r')
            break;
		if (i > 2 && (request[i - 3] == '\r' && request[i - 2] == '\n'
			&& request[i - 1] == '\r' && request[i] == '\n'))
		{	
			end = i + 1;
			break;
		}
	}
	for (int i = end; i != 0 && i < (int)request.length(); i++)
		this->entityBody += request[i];
}

void    HttpRequest::printClassAtributes()
{
    std::cout << "Method: " << method << std::endl;
    std::cout << "URL: " << uri.getUri() << std::endl;
    std::cout << "version: " << httpVersion << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin();
    it != headers.end(); ++it)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }
    std::cout << "Entity Bode: " << entityBody << std::endl;
}