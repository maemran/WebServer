/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:51:29 by maemran           #+#    #+#             */
/*   Updated: 2026/04/09 21:10:29 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

int HttpResponse::fileNum = 1;

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
    indexFound = false;
    uploadedFiles.push_back("../webroot/images/1");// convert it to static
    server = config.getServers()[serverIndex];
    statusCode = "200";
    reasonPhrase["200"] = "OK";
    reasonPhrase["201"] = "Created";
    reasonPhrase["204"] = "No Content";
    reasonPhrase["301"] = "Moved Permanently";
    reasonPhrase["302"] = "Moved Temporarily";
    reasonPhrase["400"] = "Bad Request";
    reasonPhrase["404"] = "Not Found";
    reasonPhrase["403"] = "Forbidden";
    reasonPhrase["405"] = "Method Not Allowed";
    reasonPhrase["411"] = "Length Required";
    reasonPhrase["413"] = "Content Too Large";
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
	{
        content += line;
		if (!(file.eof()))
			content += "\n";
	}
    return content;
}

int ft_stoi(std::string str)
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
    if (request.getMethod() != "HEAD")
        addHeader("Content-Length", ft_itos(body.size()));
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
    if (request.getMethod() != "HEAD")
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
    std::map<int, std::string>::const_iterator it = errorPage.find(ft_stoi(statusCode));
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
    if (request.getMethod() == "HEAD")
        addHeader("Content-Length", ft_itos(body.size()));
}

std::string HttpResponse::removeLastSlash(const std::string& path)
{
    std::string normalized = path;

    if (normalized.length() > 1 && normalized[normalized.length() - 1] == '/')
        normalized.erase(normalized.length() - 1, 1);
    return normalized;
}

int HttpResponse::exactPath()
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
        return 0;
    loc = server.getLocations()[index];
    return 1;
}

void    HttpResponse::pathExists(const std::string& path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0)
        return ;
    throw errorResponseException("404");
}

int HttpResponse::indexExist(const std::string& path)
{
    struct stat s;
    std::string indexPath = path;

    if (indexPath[indexPath.length() - 1] != '/')
        indexPath += '/';
    indexPath.append("index.html");
    if (stat(indexPath.c_str(), &s) == 0)
        return 1;
    return 0;
}

int HttpResponse::isDirectory(const std::string& path)
{
    struct stat s;

    if (stat(path.c_str(), &s) == 0)
    {
        if (S_ISDIR(s.st_mode))
            return 1;
        else if (S_ISREG(s.st_mode))
            return 0;
    }
    else
        throw errorResponseException("404");
    return 0;
}

void HttpResponse::longestMatch()
{
    const std::string requestPath = removeLastSlash(request.getUri().getPath());
    const std::vector<LocationConfig>& locations = server.getLocations();
    int bestIndex = -1;
    size_t bestLen = 0;

    for (size_t i = 0; i < locations.size(); i++)
    {
        std::string locationPath = removeLastSlash(locations[i].getPath());
        if (requestPath.compare(0, locationPath.length(), locationPath) != 0)
            continue;
        if (!(locationPath == "/"
            || requestPath.length() == locationPath.length()
            || requestPath[locationPath.length()] == '/'))
            continue;
        if (locationPath.length() > bestLen)
        {
            bestLen = locationPath.length();
            bestIndex = static_cast<int>(i);
        }
    }
    if (bestIndex == -1)
        throw errorResponseException("404");
    loc = locations[bestIndex];
}

void    HttpResponse::findPath()
{
    if (exactPath())
        redirectionCheck();
    else
    {
        longestMatch();
        redirectionCheck();
        std::string path = loc.getRoot() + request.getUri().getPath();
        if (isDirectory(path))
            directory = path;
        else
            file = path;
    }
}

void    HttpResponse::redirectionCheck()
{
    const std::map<int, std::string>& redirections = loc.getRedirections();
    if (loc.hasRedirect())
    {
        std::map<int, std::string>::const_iterator it = redirections.begin();
        addHeader("Location", it->second);
        generateDefaultPage(ft_itos(it->first), reasonPhrase[ft_itos(it->first)]);
        // addHeader("Content-Length", ft_itos(body.size()));
        throw redirectResponseException(ft_itos(it->first));
    }
}

void    HttpResponse::generateDirectoryListing(const std::string& path)
{
	DIR* dir;
    struct dirent* ent;
    std::stringstream html;

	dir = opendir(path.c_str());
	html << "<!DOCTYPE HTML>\n<html lang=\"en\">\n<head>\n"
             << "<meta charset=\"utf-8\">\n"
             << "<title>Directory listing for " << path << "</title>\n"
             << "</head>\n<body>\n"
             << "<h1>Directory listing for " << path << "</h1>\n"
             << "<hr>\n<ul>\n";
	while ((ent = readdir(dir)) != NULL) 
	{
        std::string name = ent->d_name;
        if (name == "." || name == "..")
			continue;
        std::string fullPath = path + "/" + name;
        if (isDirectory(fullPath)) 
            name += "/";
        html << "<li><a href=\"" << name << "\">" << name << "</a></li>\n";
    }
	html << "</ul>\n<hr>\n</body>\n</html>";
    addHeader("Content-Type", "text/html");
    closedir(dir);
	body = html.str();
}

void    HttpResponse::choseIndexFile()
{
    router r(loc);
    struct stat s;
    std::string temp = r.getRoutedPath();
    const std::vector<std::string>& index = loc.getIndexFiles();
    for (int i = 0; i < (int)index.size(); i++)
    {
        std::string path;
        if (r.getRoutedPath()[r.getRoutedPath().length() - 1] != '/')
            temp += "/";
        path = temp + index[i]; 
        if (stat(path.c_str(), &s) == 0)
        {
            body = readFile(path);
            contentTypeSelector(path);
            indexFound = true;
            // addHeader("Content-Length", ft_itos(body.size()));
            return;
        }
    }
}

void    HttpResponse::GETWithExactPath()
{
    router r(loc);
    pathExists(r.getRoutedPath());
    if (!(loc.getIndexFiles().empty()))
    {
        choseIndexFile();
        if (indexFound)
            return;
    }
    if (loc.getAutoindex() && !indexFound)// true if it on
    {
        generateDirectoryListing(r.getRoutedPath());
        return;
    }
        throw errorResponseException("403");
}

void     HttpResponse::GETMethod()
{
	if (directory != "")
    {
        if (indexExist(directory))
        {
            std::string indexPath = directory;
            if (indexPath[indexPath.length() - 1] != '/')
                indexPath += '/';
            body = readFile(indexPath + "index.html");
            addHeader("Content-Type", "text/html");
        }
        else
            generateDirectoryListing(directory);
    }
    else if (file != "")
    {
        contentTypeSelector(file);
        body = readFile(file);
    }
    else
        GETWithExactPath();
}

void     HttpResponse::HEADMethod()
{
    GETMethod();
    addHeader("Content-Length", ft_itos(body.size()));
    body = "";
}

void    HttpResponse::contentLengthValidation()
{
    std::map<std::string, std::string>::const_iterator it = request.getHeaders().find("content-length");
    if (it == request.getHeaders().end())
        throw errorResponseException("411");
}

void    HttpResponse::entityBodySizeCheck()
{
    std::map<std::string, std::string>::const_iterator it = request.getHeaders().find("content-length");
    int contentLength = ft_stoi(it->second);
    if ((int)request.getEntityBody().size() < contentLength)
        throw errorResponseException("400");
}

void    HttpResponse::POSTMethodChecks()
{
    std::string filePath = loc.getRoot();
    if (filePath[filePath.length() - 1] != '/')
        filePath += '/';
    filePath += request.getUri().getPath();
    contentLengthValidation();
    entityBodySizeCheck();
    if (loc.getMaxBodySize() < request.getEntityBody().size())
        throw errorResponseException("413");
    if (file != "" || !isDirectory(filePath))
        throw errorResponseException("405");
}

std::string HttpResponse::contentTypeToExtension()
{
    std::map<std::string, std::string>::const_iterator headerIt = request.getHeaders().find("content-type");
    if (headerIt == request.getHeaders().end())
        return "";
    std::string contentType = headerIt->second;
    for (std::map<std::string, std::string>::const_iterator it = extensionTypes.begin();
        it != extensionTypes.end(); ++it)
    {
        if (it->second == contentType)
            return it->first;
    }
    return "";
}

void     HttpResponse::POSTMethod()
{
    POSTMethodChecks();
    std::string extension = contentTypeToExtension();
    std::string uploadPath = loc.getRoot();
    std::string uploadedFile = request.getUri().getPath();
    std::string fileName = ft_itos(fileNum) + extension;
    
    if (uploadPath[uploadPath.length() - 1] != '/')
        uploadPath += '/';
    if (uploadedFile[uploadedFile.length() - 1] != '/')
        uploadedFile += '/';
    std::string fullPath = uploadPath + uploadedFile + fileName;
    std::ofstream outFile(fullPath.c_str(), std::ios::out | std::ios::trunc);
    if (!outFile.is_open())
        throw errorResponseException("403");
    outFile << request.getEntityBody();
    outFile.close();
    uploadedFiles.push_back(fullPath);
    contentTypeSelector(fullPath);
    body = readFile(fullPath);
    addHeader("Location", (uploadedFile + fileName));
    statusCode = "201";
    fileNum++;
}

void    HttpResponse::DELMethodChecks()
{
    int flag = -1;
    std::string deletedPath = loc.getRoot();
    if (deletedPath[deletedPath.length() - 1] != '/')
        deletedPath += '/';
    deletedPath += request.getUri().getPath();
    if (directory != "" || isDirectory(deletedPath))
        throw errorResponseException("403");
    for (int i = 0; i < (int)uploadedFiles.size(); i++)
    {
        if (uploadedFiles[i] == file || uploadedFiles[i] == deletedPath)
            flag = i;
    }
    if (flag == -1)
        throw errorResponseException("403");
}

void     HttpResponse::DELMethod()
{
    DELMethodChecks();
    std::string deletedPath = loc.getRoot();
    if (deletedPath[deletedPath.length() - 1] != '/')
        deletedPath += '/';
    deletedPath += request.getUri().getPath();
    
    if (std::remove(deletedPath.c_str()) != 0)
        throw errorResponseException("403");
    for (std::vector<std::string>::iterator it = uploadedFiles.begin(); it != uploadedFiles.end();)
    {
        if (*it == deletedPath || *it == file)
            it = uploadedFiles.erase(it);
        else
            ++it;
    }
    statusCode = "204";
    fileNum--;
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