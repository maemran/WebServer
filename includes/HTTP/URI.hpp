/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:18:25 by maemran           #+#    #+#             */
/*   Updated: 2026/03/04 16:51:05 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
#define URI_HPP

#include <string>
#include <exception>

class   URI
{
    private:
        std::string uri;
        std::string scheme;
        std::string host;
        std::string path;
        void	pathWithSpacesCheck();
        void    printClassAtributes();
        void    uriParser();
        void    uriCheck();
        void    uriValidate();
        
    public:
        URI();
        URI(const std::string& uri);
        URI(const URI& other);
        URI& operator=(const URI& other);
        ~URI();

        const std::string& getUri() const;
        const std::string& getScheme() const;
        const std::string& getHost() const;
        const std::string& getPath() const;

        // void parseUri(const std::string& uri);
        void    setUri(const std::string& uri);
        void    setScheme(const std::string& scheme);
        void    setHost(const std::string& host);
        void    setPath(const std::string& path);

        void    uriHandler();

        class   badURIException: std::exception
        {
            private:
                char *errMessage;
            public:
                badURIException(const char *errMessage);
                const char* what() const throw();
        };
};

#endif