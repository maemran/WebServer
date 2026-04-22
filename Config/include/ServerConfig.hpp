/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:37:33 by maemran           #+#    #+#             */
/*   Updated: 2026/04/10 11:35:26 by maemran          ###   ########.fr       */
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
    	std::vector<std::string> indexFiles;
		bool autoindex;
		std::map<int , std::string> error_page;
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
		std::vector<LocationConfig>& getLocations();
		std::vector<ServerConfig>& getServers();

		void	setListenIp(const std::string& ip);
		void	setListenPort(int port);
		void	setRoot(const std::string& root);
		void	setAutoindex(bool autoindex);
		void	addAllowedMethod(const std::string& method);
		void	addErrorPage(int code, const std::string& page);
		void	setMaxBodySize(size_t size);
		void	addLocation(const LocationConfig& location);
		void 	resolveInheritance() ;
		void 	addIndexFile(const std::string& file);
		const 	std::vector<std::string>& getIndexFiles() const;
		
};

#endif