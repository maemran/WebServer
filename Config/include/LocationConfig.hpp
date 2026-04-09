/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:51 by maemran           #+#    #+#             */
/*   Updated: 2026/04/09 20:35:57 by maemran          ###   ########.fr       */
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
		std::vector<std::string>	indexFiles;
		bool autoindex;
		std::map<int , std::string> error_page;
		std::map<int , std::string> redirections;
   		std::vector<std::string> allowed_methods;
		size_t client_max_body_size;
		int redirect_code;
		std::string redirect_url;
		bool has_redirect;
		std::map<std::string, std::string> cgi_map;

	public:
		LocationConfig();
		LocationConfig(const LocationConfig& other);
		LocationConfig&	operator=(const LocationConfig& other);
		~LocationConfig();

		const std::string&	getPath() const;
		const std::string&	getRoot() const;
		const std::string&	getIndex() const;
		bool	getAutoindex() const;
		const std::vector<std::string>&	getMethods() const;
		const std::map<int, std::string>&	getErrorPages() const;
		const std::map<int, std::string>&	getRedirections() const;
		int getRedirectCode() const;
		bool hasRedirect() const;
		size_t getMaxBodySize() const;//
		const std::string&	getRedirectUrl() const;//
		const std::vector<std::string>&	getIndexFiles() const;
		//bool	isRedirection() const;
		
		void	setPath(const std::string& path);
		void	setRoot(const std::string& root);
		void	setAutoindex(bool autoindex);
		void	addAllowedMethod(const std::string& method);
		void	addErrorPage(int code, const std::string& page);
		void	addRedirection(int code, const std::string& url);
		void	setMaxBodySize(size_t size);
		void 	setRedirect(int code, const std::string& url);
		void	addIndexFile(const std::string& file);
		void addCgi(const std::string& ext, const std::string& path);
		const std::map<std::string, std::string>& getCgiMap() const;

};

#endif