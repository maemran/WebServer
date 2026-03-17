/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 03:01:42 by maemran           #+#    #+#             */
/*   Updated: 2026/03/17 01:47:27 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "router.hpp"
#include <iostream>

router::router() {}

router::router(const router& other)
{
    this->routedPath = other.routedPath;
    this->routedErrorPages = other.routedErrorPages;
}

router& router::operator=(const router& other)
{
    if (this != &other)
    {
        this->routedPath = other.routedPath;
        this->routedErrorPages = other.routedErrorPages;
    }
    return *this;
}

router::~router() {}

void    router::addForwardSlash(std::string& str)
{
    std::string slash = "/";
    std::string temp = slash + str;
    str = temp;
}

const std::string& router::getRoutedPath() const
{
    return routedPath;
}

const std::map<int, std::string>& router::getRoutedErrorPages() const
{
    return routedErrorPages;
}

router::router(const LocationConfig& loc)
{
    this->routedPath = loc.getRoot() + loc.getPath();
    for (std::map<int, std::string>::const_iterator it = (loc.getErrorPages().begin());
    it != loc.getErrorPages().end(); ++it)
    {
        if (it->second[0] != '/')
            addForwardSlash((std::string&)(it->second));
        std::string temp = loc.getRoot() + it->second;
        routedErrorPages.insert(std::make_pair(it->first, temp));
        temp = "";
    }
}

router::router(const ServerConfig& serv)
{
    for (std::map<int, std::string>::const_iterator it = (serv.getErrorPages().begin());
    it != serv.getErrorPages().end(); ++it)
    {
        if (it->second[0] != '/')
            addForwardSlash((std::string&)(it->second));
        std::string temp = serv.getRoot() + it->second;
        routedErrorPages.insert(std::make_pair(it->first, temp));
        temp = "";
    }
}