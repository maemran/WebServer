/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:18:25 by maemran           #+#    #+#             */
/*   Updated: 2026/02/28 12:58:16 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
#define URI_HPP

#include <string>

class   URI
{
    private:
        std::string uri;
        std::string scheme;
        std::string host;
        std::string path;
		int	uriError;
    
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
		int getUriError() const;

        // void parseUri(const std::string& uri);
        void    setUri(const std::string& uri);
        void    setScheme(const std::string& scheme);
        void    setHost(const std::string& host);
        void    setPath(const std::string& path);
		void	setUriError(int uriError);

		//void    uriParser();
        void     uriCheck();
};

#endif