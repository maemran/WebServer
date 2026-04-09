/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:18:54 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/25 11:07:27 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/LocationConfig.hpp"

// Default constructor
LocationConfig::LocationConfig()
    : path(""),
      root(""),
      autoindex(false),
      client_max_body_size(0),
      redirect_code(0),
      redirect_url(""),
      has_redirect(false)
{}

// Copy constructor
LocationConfig::LocationConfig(const LocationConfig& other)
    : path(other.path),
      root(other.root),
      indexFiles(other.indexFiles),
      autoindex(other.autoindex),
      error_page(other.error_page),
      redirections(other.redirections),
      allowed_methods(other.allowed_methods),
      client_max_body_size(other.client_max_body_size),
      redirect_code(other.redirect_code),
      redirect_url(other.redirect_url),
      has_redirect(other.has_redirect),
      cgi_map(other.cgi_map)
{}

// Assignment operator
LocationConfig& LocationConfig::operator=(const LocationConfig& other)
{
    if (this != &other)
    {
        path = other.path;
        root = other.root;
        indexFiles = other.indexFiles;
        autoindex = other.autoindex;
        error_page = other.error_page;
        redirections = other.redirections;
        allowed_methods = other.allowed_methods;
        client_max_body_size = other.client_max_body_size;

        redirect_code = other.redirect_code;
        redirect_url = other.redirect_url;
        has_redirect = other.has_redirect;
        cgi_map = other.cgi_map; 
    }
    return *this;
}

// Destructor
LocationConfig::~LocationConfig() {}

// Getters
const std::string& LocationConfig::getPath() const { return path; }
const std::string& LocationConfig::getRoot() const { return root; }
bool LocationConfig::getAutoindex() const { return autoindex; }
const std::vector<std::string>& LocationConfig::getMethods() const { return allowed_methods; }
const std::map<int, std::string>& LocationConfig::getErrorPages() const { return error_page; }
const std::map<int, std::string>& LocationConfig::getRedirections() const { return redirections; }
size_t LocationConfig::getMaxBodySize() const { return client_max_body_size; }
void LocationConfig::setRedirect(int code, const std::string& url)
{
    redirect_code = code;
    redirect_url = url;
    has_redirect = true;
}

// Setters
void LocationConfig::setPath(const std::string& p) { path = p; }
void LocationConfig::setRoot(const std::string& r) { root = r; }
void LocationConfig::setAutoindex(bool a) { autoindex = a; }
void LocationConfig::addAllowedMethod(const std::string& method) { allowed_methods.push_back(method); }
void LocationConfig::addErrorPage(int code, const std::string& page) { error_page[code] = page; }
void LocationConfig::addRedirection(int code, const std::string& url) { redirections[code] = url; }
void LocationConfig::setMaxBodySize(size_t size) { client_max_body_size = size; }

void LocationConfig::addIndexFile(const std::string& file)
{
    indexFiles.push_back(file);
}

std::vector<std::string> LocationConfig::getIndexFiles() const
{
    return indexFiles;
}
bool LocationConfig::hasRedirect() const
{
    return has_redirect;
}

int LocationConfig::getRedirectCode() const
{
    return redirect_code;
}

const std::string& LocationConfig::getRedirectUrl() const
{
    return redirect_url;
}

void LocationConfig::addCgi(const std::string& ext, const std::string& path)
{
    cgi_map[ext] = path;
}

const std::map<std::string, std::string>& LocationConfig::getCgiMap() const
{
    return cgi_map;
}