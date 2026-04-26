/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:43:06 by maemran           #+#    #+#             */
/*   Updated: 2026/04/23 16:57:43 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <fstream>
#include "HttpRequest.hpp"
#include "HttpConfig.hpp"
#include "router.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <dirent.h> 
#include <sys/stat.h>

class Cookie;

class   HttpResponse
{
    private:
        std::string response;
        std::string directory;
        std::string file;
        std::string body;
        std::string statusCode;
        std::vector<std::string>    headers;
        std::vector<std::string>    uploadedFiles;
        std::map<std::string, std::string>  reasonPhrase;
        std::map<std::string, std::string>  extensionTypes;
        HttpRequest request;
        HttpConfig config;
        ServerConfig server;
        LocationConfig loc;
        int serverIndex;
        static int fileNum;
        bool indexFound;

        void    findPath();
        int     exactPath();
        void    longestMatch();
        int     isDirectory(const std::string& path);
        void    pathExists(const std::string& path);
        int     indexExist(const std::string& path);
        void    findErrorPage();
        void    createResponse();
        void    redirectionCheck();
        void    printClassAtributes();
        void    errorPageResponse();
        void    sessionHandler();
        void    methodsHandler();
        void    DELMethod();
        void    POSTMethod();
        void    HEADMethod();
        void    GETMethod();
        void    GETWithExactPath();
        void    choseIndexFile();
        void    contentLengthValidation();
        void    entityBodySizeCheck();
        void    POSTMethodChecks();
        void    DELMethodChecks();
		bool	CGIHandler();
        std::string contentTypeToExtension();
        void    contentTypeSelector(const std::string& file);
        void    generateDirectoryListing(const std::string& path);
        void    generateDefaultPage(const std::string& statusCode, const std::string&  ReasonPhrase);
        std::map<int, std::string>    chosePagePos();
        std::string removeLastSlash(const std::string& path);
        std::string getCurrentDate();
        
    public:
        HttpResponse();
        HttpResponse(const HttpResponse& other);
        HttpResponse&   operator=(const HttpResponse& other);
        HttpResponse(const HttpRequest& request, const HttpConfig& config, int serverIndex);
        ~HttpResponse();
        
        const std::string& getResponse() const;
        const HttpRequest& getRequest() const;
        const HttpConfig& getConfig() const;
        const std::string& getBody() const;
        const std::string& getStatusCode() const;
        const std::string& getReasonPhrase() const;
        const std::vector<std::string>& getHeaders() const;
        int getServerIndex() const;
        void    setResponse(const std::string& response);
        void    setRequest(const HttpRequest& request);
        void    setConfig(const HttpConfig& config);
        void    setServerIndex(int index);
        void    setBody(const std::string& body);
        void    setStatusCode(const std::string& statusCode);
        void    setReasonPhrase(const std::string& reasonPhrase);
        void    addHeader(const std::string& key, const std::string& value);
        void    addCookie(const Cookie& cookie);
        
        
        void responseHandler();

        class   errorResponseException: std::exception
        {
            private:
                std::string statusCode;
            public:
                errorResponseException(const std::string& statCode);
                const char* what() const throw();
                ~errorResponseException() throw();
        };

        class   redirectResponseException: std::exception
        {
            private:
                std::string statusCode;
            public:
                redirectResponseException(const std::string& statCode);
                const char* what() const throw();
                ~redirectResponseException() throw();
        };
};

#endif
