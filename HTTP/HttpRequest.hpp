/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 05:40:32 by maemran           #+#    #+#             */
/*   Updated: 2026/02/28 13:04:51 by maemran          ###   ########.fr       */
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
        int statusCode;
        void    storingHeaders(std::vector<std::string> requestElements);
        void    requestLineParser();
        
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
        int   getStatusCode() const;

        void    setRequestLine(const std::string& requestLine);
        void    setMethod(const std::string& method);
        void    setUri(const URI& uri);
        void    setHttpVersion(const std::string& httpVersion);
        void    setHeaders(const std::map<std::string, std::string>& headers);
        void    setEntityBody(const std::string& entityBody);
        void    setStatusCode(int StatusCode);

        void    requestParser(const std::string& request);
        int     requestCheck(const std::string& request);
        void    printClassAtributes();
};

#endif