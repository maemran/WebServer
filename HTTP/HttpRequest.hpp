/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 05:40:32 by maemran           #+#    #+#             */
/*   Updated: 2026/02/24 07:23:48 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <map>
#include "URI.hpp"

class   HttpRequest
{
    private:
        std::string requestLine;
        std::string method;
        URI uri;
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::string entityBody;
        
    public:
        HttpRequest();
        HttpRequest(const HttpRequest& other);
        HttpRequest&    operator=(const HttpRequest& other);
        ~HttpRequest();

        const std::string& getRequestLine() const;
        const std::string& getMethod() const;
        const URI& getUri() const;
        const std::string& getHttpVersion() const;
        const std::map<std::string, std::string>& getHeaders() const;
        const std::string& getEntityBody() const;

        void    setRequestLine(const std::string& requestLine);
        void    setMethod(const std::string& method);
        void    setUri(const URI& uri);
        void    setHttpVersion(const std::string& httpVersion);
        void    setHeaders(const std::map<std::string, std::string>& headers);
        void    setEntityBody(const std::string& entityBody);

        void    requestParsing(const std::string& request);
};

#endif