/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 05:40:32 by maemran           #+#    #+#             */
/*   Updated: 2026/03/04 19:49:42 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <map>
#include "URI.hpp"
#include <vector>

class   HttpRequest
{
    private:
        std::string requestLine;
        std::string method;
        URI uri;
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::string entityBody;
        std::string statusCode;
        double  versionNum;
		
		std::vector<std::string>	requestLexer(const std::string& request);
		std::string	extractBody(const std::string& request);
        void    storingHeaders(std::vector<std::string> requestElements);
        void    requestParser(const std::string& request);
        void    requestCheck(const std::string& request);
        void    requestLineParser();
        void    methodValidation();
        void    versionValidation();
        void    headersValidation();
        void    requestValidate();
        void    printClassAtributes();
        
    public:
        HttpRequest();
        HttpRequest(const HttpRequest& other);
        HttpRequest&    operator=(const HttpRequest& other);
        ~HttpRequest();

        const std::string& getRequestLine() const;
        const std::string& getMethod() const;
        URI& getUri();
        const std::string& getHttpVersion() const;
        const std::map<std::string, std::string>& getHeaders() const;
        const std::string& getEntityBody() const;
        std::string   getStatusCode() const;

        void    setRequestLine(const std::string& requestLine);
        void    setMethod(const std::string& method);
        void    setUri(const URI& uri);
        void    setHttpVersion(const std::string& httpVersion);
        void    setHeaders(const std::map<std::string, std::string>& headers);
        void    setEntityBody(const std::string& entityBody);
        void    setStatusCode(const std::string& StatusCode);

        void    requestHandler(std::string& request);

        class   badRequestException: std::exception
        {
            private:
                char*   statusCode;
            public:
                badRequestException(const char* statCode);
                const char* what() const throw();
        };
};

#endif