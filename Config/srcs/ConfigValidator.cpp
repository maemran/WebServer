/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:52:56 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/10 11:36:25 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigValidator.hpp"
#include <sys/stat.h>
#include <cstdio>
#include <sstream>
#include <cctype>
#include <cstdlib>

ConfigValidator::ConfigValidator() {}
ConfigValidator::~ConfigValidator() {}

void ConfigValidator::validate(HttpConfig& http)
{
    std::vector<ServerConfig>& servers = http.getServers();

    if (servers.empty())
        throw std::runtime_error("No servers defined");

    applyInheritance(http);
    checkIndexFiles(http.getIndexFiles());
    checkDuplicatePorts(servers);

    for (size_t i = 0; i < servers.size(); i++)
        validateServer(servers[i]);
}

void ConfigValidator::validateServer(const ServerConfig& server)
{
    int port = server.getListenPort();
    checkPortRange(port);
    
    checkListenIp(server.getListenIp());

    if (server.getRoot().empty())
        throw std::runtime_error("Server missing root directive");

    checkRootExists(server.getRoot());
    checkIndexFiles(server.getIndexFiles());

    checkClientMaxBodySize(server.getMaxBodySize());
    checkErrorPages(server.getErrorPages(), server.getRoot());
    const std::vector<LocationConfig>& locations =
        server.getLocations();

    for (size_t i = 0; i < locations.size(); i++) {
        validateLocation(locations[i]);
    }
        
}

void ConfigValidator::validateLocation(const LocationConfig& location)
{
    if (location.getPath().empty())
        throw std::runtime_error("Location path is empty");

    if (location.getPath()[0] != '/')
        throw std::runtime_error("Location path must start with '/'");

    if (location.getRoot().empty())
        throw std::runtime_error("Location has no root defined");
    
    checkRootExists(location.getRoot());

    checkMethods(location.getMethods());
    checkIndexFiles(location.getIndexFiles());
    checkErrorPages(location.getErrorPages(), location.getRoot());

}

void ConfigValidator::checkErrorPages(const std::map<int, std::string>& errors,
                                     const std::string& root)
{
    struct stat info;

    for (std::map<int, std::string>::const_iterator it = errors.begin();
         it != errors.end(); ++it)
    {
        int code = it->first;
        const std::string& path = it->second;

        if (code < 100 || code > 599)
        {
            std::ostringstream oss;
            oss << code;
            throw std::runtime_error("Invalid error_page code: " + oss.str());
        }

        if (path.empty() || path[0] != '/')
            throw std::runtime_error("error_page path must start with '/': " + path);

        std::string full = root;
        if (!full.empty() && full[full.size() - 1] == '/')
            full.resize(full.size() - 1);
        full += path;

        if (stat(full.c_str(), &info) != 0)
            throw std::runtime_error("Error page file does not exist: " + full);

        if (!S_ISREG(info.st_mode))
            throw std::runtime_error("Error page is not a regular file: " + full);
    }
}
void ConfigValidator::checkPortRange(int port)
{
    if (port <= 0 || port > 65535)
        throw std::runtime_error("Invalid port range");
        
}

void ConfigValidator::checkDuplicatePorts(
    const std::vector<ServerConfig>& servers)
{
    std::set<int> usedPorts;

    for (size_t i = 0; i < servers.size(); i++)
    {
        int port = servers[i].getListenPort();

        if (usedPorts.count(port))
            throw std::runtime_error("Duplicate listen port detected");

        usedPorts.insert(port);
    }
}

void ConfigValidator::checkRootExists(const std::string& root)
{
    struct stat info;

    if (stat(root.c_str(), &info) != 0)
        throw std::runtime_error("Root path does not exist: " + root);

    if (!(info.st_mode & S_IFDIR))
        throw std::runtime_error("Root is not a directory: " + root);
}

void ConfigValidator::checkMethods(
    const std::vector<std::string>& methods)
{
    for (size_t i = 0; i < methods.size(); i++)
    {
        if (methods[i] != "GET" &&
            methods[i] != "POST" &&
            methods[i] != "DELETE" &&
			methods[i] != "HEAD")
        {
            throw std::runtime_error("Invalid HTTP method: " +
                                     methods[i]);
        }
    }
}

void ConfigValidator::checkClientMaxBodySize(size_t size)
{
    if (size == 0)
        return;

    if (size > 100000000)
        throw std::runtime_error(
            "client_max_body_size too large");
}

void ConfigValidator::checkIndexFiles(const std::vector<std::string>& indexFiles)
{
    for (size_t i = 0; i < indexFiles.size(); i++)
    {
        const std::string& file = indexFiles[i];
        
        if (file.empty())
            throw std::runtime_error("Index file cannot be empty");
        
        if (file[0] == '/')
            throw std::runtime_error("Index file must NOT start with '/': " + file);
        
        if (file[file.length() - 1] == '/')
            throw std::runtime_error("Index file must NOT end with '/': " + file);
    }
}

bool ConfigValidator::isValidIP(const std::string& ip)
{
    int dotCount = 0;
    for (size_t i = 0; i < ip.length(); i++)
    {
        if (ip[i] == '.')
            dotCount++;
    }
    
    if (dotCount != 3)
        return false;
    
    std::stringstream ss(ip);
    std::string octet;
    int octetCount = 0;
    
    while (std::getline(ss, octet, '.'))
    {
        octetCount++;
        
        if (octet.empty())
            return false;
        
        for (size_t i = 0; i < octet.length(); i++)
        {
            if (!isdigit(octet[i]))
                return false;
        }
        
        int value = std::atoi(octet.c_str());
        if (value < 0 || value > 255)
            return false;
    }
    
    if (octetCount != 4)
        return false;
    
    return true;
}

void ConfigValidator::checkListenIp(const std::string& ip)
{
    if (!isValidIP(ip))
        throw std::runtime_error("Invalid IP address format: " + ip + 
                                ". Must be valid IPv4 (e.g., 127.0.0.1)");
}

void ConfigValidator::applyInheritance(HttpConfig& http)
{
    std::vector<ServerConfig>& servers = http.getServers();

    for (size_t i = 0; i < servers.size(); i++)
    {
        ServerConfig& server = servers[i];

        if (server.getRoot().empty())
            server.setRoot(http.getRoot());

        if (server.getIndexFiles().empty())
        {
            const std::vector<std::string>& httpIndex = http.getIndexFiles();
            for (size_t k = 0; k < httpIndex.size(); k++)
                server.addIndexFile(httpIndex[k]);
        }

        if (server.getMaxBodySize() == 0)
            server.setMaxBodySize(http.getMaxBodySize());

        const std::map<int, std::string>& httpErrors = http.getErrorPages();
        const std::map<int, std::string>& srvErrors = server.getErrorPages();

        for (std::map<int, std::string>::const_iterator it = httpErrors.begin();
             it != httpErrors.end(); ++it)
        {
            if (srvErrors.find(it->first) == srvErrors.end())
                server.addErrorPage(it->first, it->second);
        }

        std::vector<LocationConfig>& locations = server.getLocations();

        for (size_t j = 0; j < locations.size(); j++)
        {
            LocationConfig& location = locations[j];

            if (location.getRoot().empty())
                location.setRoot(server.getRoot());

            if (location.getIndexFiles().empty())
            {
                const std::vector<std::string>& serverIndex = server.getIndexFiles();
                for (size_t k = 0; k < serverIndex.size(); k++)
                    location.addIndexFile(serverIndex[k]);
            }

            if (location.getMaxBodySize() == 0)
                 location.setMaxBodySize(server.getMaxBodySize());

            const std::map<int, std::string>& srvErrors2 = server.getErrorPages();
            const std::map<int, std::string>& locErrors = location.getErrorPages();

            for (std::map<int, std::string>::const_iterator it = srvErrors2.begin();
                 it != srvErrors2.end(); ++it)
            {
                if (locErrors.find(it->first) == locErrors.end())
                    location.addErrorPage(it->first, it->second);
            }
        }
    }
}