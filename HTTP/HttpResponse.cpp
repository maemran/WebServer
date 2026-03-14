/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:51:29 by maemran           #+#    #+#             */
/*   Updated: 2026/03/15 01:25:04 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <iostream>

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(const HttpResponse& other)
{
    this->response = other.response;
    this->request = other.request;
    this->config = other.config;
    this->serverIndex = other.serverIndex;
    this->body = other.body;
    this->statusCode = other.statusCode;
    this->reasonPhrase = other.reasonPhrase;
    this->headers = other.headers;
}
HttpResponse&   HttpResponse::operator=(const HttpResponse& other)
{
    if (this != &other)
    {
        this->response = other.response;
        this->request = other.request;
        this->config = other.config;
        this->serverIndex = other.serverIndex;
        this->body = other.body;
        this->statusCode = other.statusCode;
        this->reasonPhrase = other.reasonPhrase;
        this->headers = other.headers;
    }
    return *this;
}

HttpResponse::HttpResponse(const HttpRequest& request, const HttpConfig& config, int serverIndex)
{
    this->request = request;
    this->config = config;
    this->serverIndex = serverIndex;
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

const std::string& HttpResponse::getBody() const
{
    return body;
}

const std::string& HttpResponse::getStatusCode() const
{
    return statusCode;
}

const std::string& HttpResponse::getReasonPhrase() const
{
    return reasonPhrase;
}

const std::vector<std::string>& HttpResponse::getHeaders() const
{
    return headers;
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

void  HttpResponse::setBody(const std::string& body)
{
    this->body = body;
}

void  HttpResponse::setStatusCode(const std::string& statusCode)
{
    this->statusCode = statusCode;
}

void  HttpResponse::setReasonPhrase(const std::string& reasonPhrase)
{
    this->reasonPhrase = reasonPhrase;
}

void  HttpResponse::addHeader(const std::string& header)
{
    this->headers.push_back(header);
}

void    HttpResponse::generateDefaultErrorPage(const std::string& statusCode, const std::string&  ReasonPhrase)
{
    std::string errorPage = 
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head> <title> Error page </title>\n"
    "<style>\nh1 {\ntext-align: center;\n}\n</style>\n</head>"
    "<body>\n"
    "<h1>";
    errorPage.append(statusCode);
    std::string secondPart = 
    "</h1>\n"
    "<hr>\n<br>\n<h1>";
    secondPart.append(ReasonPhrase);
    std::string thirdPart = "</h1>\n</body>\n</html>";
    errorPage.append(secondPart);
    errorPage.append(thirdPart);
    this->body = errorPage;
}

std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    std::string line;
    std::string content;
    
    while (std::getline(file, line))
        content += line + "\n";
    return content;
}

// function combine all response element
// function create auto index
// function generate default error page                     DONE
// function for redirection check new path
//  | Request | Location used | Root used | Final path |
//  | ------- | ------------- | --------- | ---------- |
//  | `/old`  | `/old`        | ignored   | redirect   |
//  | `/new`  | `/new`        | `/www`    | `/www/new` |

// function check if the error page is exist or not
// function find most match path

// read files function                                      DONE
// router to change all paths with root



// Connection: close        close tcp connection after each response
// 301 
// HTTP/1.1 301 Moved Permanently
// Date: Sun, 15 Mar 2026 12:30:00 GMT
// Server: MyWebServer/1.0
// Location: https://example.com/new-page
// Content-Type: text/html
// Content-Length: 178
// Connection: close

// <html>
// <head>
// <title>301 Moved Permanently</title>
// </head>
// <body>
// <h1>Moved Permanently</h1>
// <p>The document has moved <a href="https://example.com/new-page">here</a>.</p>
// </body>
// </html>

void    HttpResponse::createResponse()
{
    
}

void    HttpResponse::responseHandler()
{
    
}

void    HttpResponse::printClassAtributes()
{
    // std::cout << "Response: " << response << std::endl;
    std::cout << "Body: " << body << std::endl;
    // std::cout << "Status Code: " << statusCode << std::endl;
    // std::cout << "Reason Phrase: " << reasonPhrase << std::endl;
    // std::cout << "Headers: " << std::endl;
    // for (size_t i = 0; i < headers.size(); i++)
    //     std::cout << headers[i] << std::endl;
    // std::cout << "Server Index: " << serverIndex << std::endl;
}