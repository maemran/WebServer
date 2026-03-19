/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 07:09:20 by maemran           #+#    #+#             */
/*   Updated: 2026/03/19 14:51:16 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig() {}

LocationConfig::LocationConfig(const LocationConfig& other)
{
    *this = other;
}

LocationConfig& LocationConfig::operator=(const LocationConfig& other)
{
    if (this != &other)
    {
        path = other.path;
        root = other.root;
        index = other.index;
        autoindex = other.autoindex;
        error_pages = other.error_pages;
        redirections = other.redirections;
        allowed_methods = other.allowed_methods;
        max_body_size = other.max_body_size;
    }
    return *this;
}

LocationConfig::~LocationConfig() {}

const std::string& LocationConfig::getPath() const
{
    return path;
}

const std::string& LocationConfig::getRoot() const
{
    return root;
}

const std::string& LocationConfig::getIndex() const
{
    return index;
}

bool LocationConfig::getAutoindex() const
{
    return autoindex;
}

const std::vector<std::string>& LocationConfig::getMethods() const
{
    return allowed_methods;
}

const std::map<int, std::string>& LocationConfig::getErrorPages() const
{
    return error_pages;
}

const std::map<int, std::string>& LocationConfig::getRedirections() const
{
    return redirections;
}

size_t LocationConfig::getMaxBodySize() const
{
    return max_body_size;
}

bool LocationConfig::isRedirection() const
{
    return has_redirect;
}

const std::string& LocationConfig::getRedirectUrl() const
{
    if (!redirections.empty())
        return redirections.begin()->second;
    static const std::string emptyString = "";
    return emptyString;
}

void LocationConfig::setPath(const std::string& path)
{
    this->path = path;
}

void LocationConfig::setRoot(const std::string& root)
{
    this->root = root;
}

void LocationConfig::setIndex(const std::string& index)
{
    this->index = index;
}

void LocationConfig::setAutoindex(bool autoindex)
{
    this->autoindex = autoindex;
}

void LocationConfig::addAllowedMethod(const std::string& method)
{
    allowed_methods.push_back(method);
}

void LocationConfig::addErrorPage(int code, const std::string& page)
{
    error_pages[code] = page;
}

void LocationConfig::addRedirection(int code, const std::string& url)
{
    redirections[code] = url;
}

void LocationConfig::setMaxBodySize(size_t size)
{
    max_body_size = size;
}
