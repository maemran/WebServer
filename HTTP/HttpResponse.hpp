/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:43:06 by maemran           #+#    #+#             */
/*   Updated: 2026/03/11 18:46:37 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include "HttpRequest.hpp"
#include "HttpConfig.hpp"

class   HttpResponse
{
    private:
        std::string response;
        HttpRequest request;
        HttpConfig config;
        int serverIndex;

        void    generate_default_error_page(std::string& statusCode, std::string&  ReasonPhrase);
    public:
        HttpResponse();
        HttpResponse(const HttpResponse& other);
        HttpResponse&   operator=(const HttpResponse& other);
        HttpResponse(const HttpRequest& request, const HttpConfig& config, int serverIndex);
        ~HttpResponse();
        
        const std::string& getResponse() const;
        const HttpRequest& getRequest() const;
        const HttpConfig& getConfig() const;
        int getServerIndex() const;
        void    setResponse(const std::string& response);
        void    setRequest(const HttpRequest& request);
        void    setConfig(const HttpConfig& config);
        void    setServerIndex(int index);
        
        
        std::string generateResponse();
};

#endif