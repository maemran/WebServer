/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:51 by maemran           #+#    #+#             */
/*   Updated: 2026/02/24 07:10:37 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <string>
#include <vector> 
#include <map>

class LocationConfig
{
	private:
    	std::string path;
    	std::string root;
    	std::string index;
		bool autoindex;
		std::map<int , std::string> error_pages;
		std::map<int , std::string> redirections;
   		std::vector<std::string> allowed_methods;
		size_t max_body_size;

	public:
		LocationConfig();
		LocationConfig(const LocationConfig& other);
		LocationConfig&	operator=(const LocationConfig& other);
		~LocationConfig();

		const std::string&	getPath() const;
		const std::string&	getRoot() const;
		const std::string&	getIndex() const;
		const bool	getAutoindex() const;
		const std::vector<std::string>&	getMethods() const;
		const std::map<int, std::string>&	getErrorPages() const;
		const std::map<int, std::string>&	getRedirections() const;
		const size_t getMaxBodySize() const;
		
		void	setPath(const std::string& path);
		void	setRoot(const std::string& root);
		void	setIndex(const std::string& index);
		void	setAutoindex(bool autoindex);
		void	addAllowedMethod(const std::string& method);
		void	addErrorPage(int code, const std::string& page);
		void	addRedirection(int code, const std::string& url);
		void	setMaxBodySize(size_t size);
};

#endif