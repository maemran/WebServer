/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:34 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/25 11:15:58 by saabo-sh         ###   ########.fr       */
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
		const std::string&  getRoot() const;
		bool	getAutoindex() const;
		const std::vector<std::string>&	getMethods() const;
		const std::map<int, std::string>&	getErrorPages() const;
		const std::map<int, std::string>&	getRedirections() const;
		 size_t getMaxBodySize() const;
		 int getRedirectCode() const;
		const std::string& getRedirectUrl() const;
		bool hasRedirect() const;
		
		void	setPath(const std::string& path);
		void	setRoot(const std::string& root);
		void	setAutoindex(bool autoindex);
		void	addAllowedMethod(const std::string& method);
		void	addErrorPage(int code, const std::string& page);
		void	addRedirection(int code, const std::string& url);
		void	setMaxBodySize(size_t size);
		void setRedirect(int code, const std::string& url);
		void addIndexFile(const std::string& file);
		std::vector<std::string> getIndexFiles() const;
		void addCgi(const std::string& ext, const std::string& path);
		const std::map<std::string, std::string>& getCgiMap() const;

};

#endif