/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:51:29 by maemran           #+#    #+#             */
/*   Updated: 2026/03/17 04:05:07 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <algorithm>

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
    server = config.getServers()[serverIndex];
    statusCode = "200";
    reasonPhrase["200"] = "OK";
    reasonPhrase["301"] = "Moved Permanently";
    reasonPhrase["302"] = "Moved Temporarily";
    reasonPhrase["400"] = "Bad Request";
    reasonPhrase["405"] = "Method Not Allowed";
    reasonPhrase["501"] = "Not Implemented";

    extensionTypes[".aac"] = "audio/aac";
    extensionTypes[".abw"] = "application/x-abiword";
    extensionTypes[".apng"] = "image/apng";
    extensionTypes[".arc"] = "application/x-freearc";
    extensionTypes[".avif"] = "image/avif";
    extensionTypes[".avi"] = "video/x-msvideo";
    extensionTypes[".azw"] = "application/vnd.amazon.ebook";
    extensionTypes[".bin"] = "application/octet-stream";
    extensionTypes[".bmp"] = "image/bmp";
    extensionTypes[".bz"] = "application/x-bzip";
    extensionTypes[".bz2"] = "application/x-bzip2";
    extensionTypes[".cda"] = "application/x-cdf";
    extensionTypes[".csh"] = "application/x-csh";
    extensionTypes[".css"] = "text/css";
    extensionTypes[".csv"] = "text/csv";
    extensionTypes[".doc"] = "application/msword";
    extensionTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    extensionTypes[".eot"] = "application/vnd.ms-fontobject";
    extensionTypes[".epub"] = "application/epub+zip";
    extensionTypes[".gz"] = "application/gzip";
    extensionTypes[".gif"] = "image/gif";
    extensionTypes[".htm"] = "text/html";
    extensionTypes[".html"] = "text/html";
    extensionTypes[".ico"] = "image/vnd.microsoft.icon";
    extensionTypes[".ics"] = "text/calendar";
    extensionTypes[".jar"] = "application/java-archive";
    extensionTypes[".jpeg"] = "image/jpeg";
    extensionTypes[".jpg"] = "image/jpeg";
    extensionTypes[".js"] = "text/javascript";
    extensionTypes[".json"] = "application/json";
    extensionTypes[".jsonld"] = "application/ld+json";
    extensionTypes[".md"] = "text/markdown";
    extensionTypes[".mid"] = "audio/midi";
    extensionTypes[".midi"] = "audio/midi";
    extensionTypes[".mjs"] = "text/javascript";
    extensionTypes[".mp3"] = "audio/mpeg";
    extensionTypes[".mp4"] = "video/mp4";
    extensionTypes[".mpeg"] = "video/mpeg";
    extensionTypes[".mpkg"] = "application/vnd.apple.installer+xml";
    extensionTypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
    extensionTypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    extensionTypes[".odt"] = "application/vnd.oasis.opendocument.text";
    extensionTypes[".oga"] = "audio/ogg";
    extensionTypes[".ogv"] = "video/ogg";
    extensionTypes[".ogx"] = "application/ogg";
    extensionTypes[".opus"] = "audio/ogg";
    extensionTypes[".otf"] = "font/otf";
    extensionTypes[".png"] = "image/png";
    extensionTypes[".pdf"] = "application/pdf";
    extensionTypes[".php"] = "application/x-httpd-php";
    extensionTypes[".ppt"] = "application/vnd.ms-powerpoint";
    extensionTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    extensionTypes[".rar"] = "application/vnd.rar";
    extensionTypes[".rtf"] = "application/rtf";
    extensionTypes[".sh"] = "application/x-sh";
    extensionTypes[".svg"] = "image/svg+xml";
    extensionTypes[".tar"] = "application/x-tar";
    extensionTypes[".tif"] = "image/tiff";
    extensionTypes[".tiff"] = "image/tiff";
    extensionTypes[".ts"] = "video/mp2t";
    extensionTypes[".ttf"] = "font/ttf";
    extensionTypes[".txt"] = "text/plain";
    extensionTypes[".vsd"] = "application/vnd.visio";
    extensionTypes[".wav"] = "audio/wav";
    extensionTypes[".weba"] = "audio/webm";
    extensionTypes[".webm"] = "video/webm";
    extensionTypes[".webmanifest"] = "application/manifest+json";
    extensionTypes[".webp"] = "image/webp";
    extensionTypes[".woff"] = "font/woff";
    extensionTypes[".woff2"] = "font/woff2";
    extensionTypes[".xhtml"] = "application/xhtml+xml";
    extensionTypes[".xls"] = "application/vnd.ms-excel";
    extensionTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    extensionTypes[".xml"] = "application/xml";
    extensionTypes[".xul"] = "application/vnd.mozilla.xul+xml";
    extensionTypes[".zip"] = "application/zip";
    extensionTypes[".3gp"] = "video/3gpp";
    extensionTypes[".3g2"] = "video/3gpp2";
    extensionTypes[".7z"] = "application/x-7z-compressed";
}

HttpResponse::~HttpResponse() {}

HttpResponse::errorResponseException::errorResponseException(const std::string& statCode)
{
    this->statusCode = statCode;
}

const char *HttpResponse::errorResponseException::what() const throw()
{
    return this->statusCode.c_str();
}

HttpResponse::errorResponseException::~errorResponseException() throw() {}

HttpResponse::redirectResponseException::redirectResponseException(const std::string& statCode)
{
    statusCode = statCode;
}

const char *HttpResponse::redirectResponseException::what() const throw()
{
    return this->statusCode.c_str();
}

HttpResponse::redirectResponseException::~redirectResponseException() throw() {}

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

void  HttpResponse::addHeader(const std::string& key, const std::string& value)
{
    this->headers.push_back(key + ": " + value);
}

void    HttpResponse::generateDefaultPage(const std::string& statusCode, const std::string&  ReasonPhrase)
{
    std::string errorPage = 
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head> <title> html page </title>\n"
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
    std::ifstream file(path.c_str());
    std::string line;
    std::string content;
    
    while (std::getline(file, line))
        content += line + "\n";
    return content;
}

// function create auto index
//  | Request | Location used | Root used | Final path |
//  | ------- | ------------- | --------- | ---------- |
//  | `/old`  | `/old`        | ignored   | redirect   |
//  | `/new`  | `/new`        | `/www`    | `/www/new` |

// function find most match path
 
// function generate default error page                     DONE
// read files function                                      DONE
// router to change all paths with root                     DONE
// function combine all response element                    DONE




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

// error_page 502 /errors/502.html; and (root var;)     عادي الباث لو مابلش ب (/)     ""Not important""

// if the location is 
// location /mo {} and the request is GET /mo/ HTTP/1.0  the /mo and th /mo/ is the same path
// and if the location is location /mo/ {} and the request is GET /mo HTTP/1.0  the /mo/ and th /mo  is the same path


//request /mo/icons
//locations /    /mo  /mo/iocons 

//request /mo
// Locations /  /mo  /mo/icons

//request /mo/icons/icons.html
// Locations /  /mo  /mo/icons

//request /mo/hello
// 

int ft_itos(std::string str)
{
    std::stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

std::string ft_itos(int num)
{
    std::stringstream ss;
    std::string str;
    ss << num;
    ss >> str;
    if (num == 0)
        str = "0";
    return str;
}

std::string HttpResponse::getCurrentDate()
{
    std::time_t now = std::time(NULL);
    std::tm* gmt = std::gmtime(&now);
    char buffer[64];

    if (gmt == NULL)
        return "";
    if (std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt) == 0)
        return "";
    return std::string(buffer);
}

void    HttpResponse::createResponse()
{
    addHeader("Server", "WebServer/1.0");
    addHeader("Date", getCurrentDate());
    addHeader("Connection", "close");
    if (request.getVersionNum() != 0.9)
    {
        response = "HTTP/1.0 " + statusCode + " " + reasonPhrase[statusCode] + "\r\n";
        for (size_t i = 0; i < headers.size(); i++)
            response += headers[i] + "\r\n";
        response += "\r\n";
    }
    response += body;
}

void    HttpResponse::contentTypeSelector(const std::string& file)
{
    size_t dotPos = file.find_last_of('.');
    if (dotPos == std::string::npos)
    {
        addHeader("Content-Type", "application/octet-stream");
        return;
    }
    std::string extension = file.substr(dotPos);
    std::map<std::string, std::string>::iterator it = extensionTypes.find(extension);
    if (it != extensionTypes.end())
        addHeader("Content-Type", it->second);
    else
        addHeader("Content-Type", "application/octet-stream");
}

std::map<int, std::string>    HttpResponse::chosePagePos()
{
    if (statusCode == "400" || statusCode == "404"
        || statusCode == "501" || statusCode == "505")
    {
        router r(server);
        return r.getRoutedErrorPages();
    }
    router r(loc);
    return r.getRoutedErrorPages();
}

void    HttpResponse::findErrorPage()
{
    std::map<int, std::string>  errorPage = chosePagePos();
    std::map<int, std::string>::const_iterator it = errorPage.find(ft_itos(statusCode));
    if (it == errorPage.end())
    {
        generateDefaultPage(statusCode, reasonPhrase[statusCode]);
        addHeader("Content-Type", "text/html");
    }
    else
    {
        body = readFile(it->second);
        contentTypeSelector(it->second);
    }
} 

void HttpResponse::errorPageResponse()
{
    if (request.getStatusCode() != "200")
        statusCode = request.getStatusCode();
    findErrorPage();
    addHeader("Content-Length", ft_itos(body.size()));
}

std::string HttpResponse::removeLastSlash(const std::string& path)
{
    std::string normalized = path;

    if (normalized.length() > 1 && normalized[normalized.length() - 1] == '/')
        normalized.erase(normalized.length() - 1, 1);
    return normalized;
}

void    HttpResponse::findPath()
{
    std::string requestPath = removeLastSlash(request.getUri().getPath());
    std::string locationPath;
    int  index = -1;
    for (int i = 0; i < (int)server.getLocations().size(); i++)
    {
        locationPath = removeLastSlash(server.getLocations()[i].getPath());
        if (locationPath == requestPath)
            index = i;
    }
    if (index == -1)
        throw errorResponseException("404");
    loc = server.getLocations()[index];
}

void    HttpResponse::redirectionCheck()
{
    const std::map<int, std::string>& redirections = loc.getRedirections();
    if (!(redirections.empty()))
    {
        std::map<int, std::string>::const_iterator it = redirections.begin();
        addHeader("Location", it->second);
        generateDefaultPage(ft_itos(it->first), reasonPhrase[ft_itos(it->first)]);
        addHeader("Content-Length", ft_itos(body.size()));
        throw redirectResponseException(ft_itos(it->first));
    }
}

void     HttpResponse::GETMethod()
{
    
}

void     HttpResponse::HEADMethod()
{
    
}

void     HttpResponse::POSTMethod()
{
    
}

void     HttpResponse::DELMethod()
{
    
}

void    HttpResponse::methodsHandler()
{
    const std::vector<std::string>& allowedMethods = loc.getMethods();
    if (std::find(allowedMethods.begin(), allowedMethods.end(), request.getMethod())
        == allowedMethods.end())
        throw errorResponseException("405");
    if(request.getMethod() == "GET")
        GETMethod();
    else if (request.getMethod() == "HEAD")
        HEADMethod();
    else if (request.getMethod() == "POST")
        POSTMethod();
    else if (request.getMethod() == "DELETE")
        DELMethod();
}

void    HttpResponse::responseHandler()
{
    try
    {
        if (request.getStatusCode() != "200")
            throw errorResponseException(request.getStatusCode());
        findPath();
        redirectionCheck();
        methodsHandler();
    }
    catch (errorResponseException& e)
    {
        statusCode = e.what();
        errorPageResponse();
    }
    catch (redirectResponseException& e)
    {
        statusCode = e.what();
    }
    createResponse();
    printClassAtributes();
}

void    HttpResponse::printClassAtributes()
{
    std::cout << response << std::endl;
}