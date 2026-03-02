/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:21:11 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/02/23 14:40:03 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "LocationConfig.hpp"

class ServerConfig
{
	private:
    	std::string listen_ip;
    	int listen_port;
		std::string root;
    	std::string index;
		bool autoindex;
		std::map<int , std::string> error_pages;
   		std::vector<std::string> allowed_methods;
		size_t client_max_body_size;
    	std::vector<LocationConfig> locations;
		
	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		ServerConfig&	operator=(const ServerConfig& other);
		~ServerConfig();

		const std::string&	getListenIp() const;
		 int	getListenPort() const;
		const std::string&	getRoot() const;
		const std::string&	getIndex() const;
		 bool	getAutoindex() const;
		const std::vector<std::string>&	getMethods() const;
		const std::map<int, std::string>&	getErrorPages() const;
		 size_t getMaxBodySize() const;
		const std::vector<LocationConfig>& getLocations() const;

		void	setListenIp(const std::string& ip);
		void	setListenPort(int port);
		void	setRoot(const std::string& root);
		void	setIndex(const std::string& index);
		void	setAutoindex(bool autoindex);
		void	addAllowedMethod(const std::string& method);
		void	addErrorPage(int code, const std::string& page);
		void	setMaxBodySize(size_t size);
		void	addLocation(const LocationConfig& location);
		void 	resolveInheritance() ;
};

#endif