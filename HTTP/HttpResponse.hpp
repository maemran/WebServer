/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:43:06 by maemran           #+#    #+#             */
/*   Updated: 2026/03/17 00:50:58 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <fstream>
#include "HttpRequest.hpp"
#include "../HttpConfig.hpp"
#include "router.hpp"
#include <sstream>


class   HttpResponse
{
    private:
        std::string response;
        std::string body;
        std::string statusCode;
        std::string reasonPhrase;
        std::vector<std::string>    headers;
        std::map<std::string, std::string>  statusValue;
        std::map<std::string, std::string>  extensionTypes;
        std::string location;
        HttpRequest request;
        HttpConfig config;
        ServerConfig server;
        LocationConfig loc;
        int serverIndex;

        void    generateDefaultErrorPage(const std::string& statusCode, const std::string&  ReasonPhrase);
        void    printClassAtributes();
        void    createResponse();
        void    errorPageResponse();
        void    findErrorPage();
        void    findPath();
        void    contentTypeSelector(const std::string& file);
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
        void    addHeader(const std::string& header);
        
        
        void responseHandler();

        class   errorResponseException: std::exception
        {
            private:
                char*   statusCode;
            public:
                errorResponseException(const char* statCode);
                const char* what() const throw();
        };
};

#endif