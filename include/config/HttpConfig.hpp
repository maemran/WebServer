/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:41:32 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/04 15:39:03 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include "ServerConfig.hpp"
#include <vector>
#include <string>

class HttpConfig
{
	private:
    	std::string root;
    	 std::vector<std::string> indexFiles;
		bool autoindex;
		size_t client_max_body_size;
		std::map<int , std::string> error_pages;
		std::vector<std::string> allowed_methods;
    	std::vector<ServerConfig> servers;
	
	public:
		HttpConfig();
		HttpConfig(const HttpConfig& other);
		HttpConfig&	operator=(const HttpConfig& other);
		~HttpConfig();

		const std::string&	getRoot() const;
		 bool	getAutoindex() const;
		 size_t getMaxBodySize() const;
		const std::map<int, std::string>&	getErrorPages() const;
		const std::vector<std::string>&	getMethods() const;
		const std::vector<ServerConfig>& getServers() const;
		std::vector<ServerConfig>& getServers() ;

		void	setRoot(const std::string& root);
		void	setAutoindex(bool autoindex);
		void	setMaxBodySize(size_t size);
		void	addErrorPage(int code, const std::string& page);
		void	addAllowedMethod(const std::string& method);
		void	addServer(const ServerConfig& server);
		void addIndexFile(const std::string& file);
    	const std::vector<std::string>& getIndexFiles() const;
};

#endif