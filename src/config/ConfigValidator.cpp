/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:52:56 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/24 16:43:16 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigValidator.hpp"
#include <sys/stat.h>
#include <cstdio>

/* ========================================= */
/*              CONSTRUCTOR                  */
/* ========================================= */

ConfigValidator::ConfigValidator() {}
ConfigValidator::~ConfigValidator() {}

/* ========================================= */
/*              PUBLIC ENTRY                 */
/* ========================================= */

void ConfigValidator::validate(HttpConfig& http)
{
    std::vector<ServerConfig>& servers = http.getServers();

    if (servers.empty())
        throw std::runtime_error("No servers defined");

    // 🔥 Apply inheritance ONCE
    applyInheritance(http);
    checkIndexFiles(http.getIndexFiles());
    checkDuplicatePorts(servers);

    for (size_t i = 0; i < servers.size(); i++)
        validateServer(servers[i]);
}

/* ========================================= */
/*              SERVER VALIDATION            */
/* ========================================= */
void ConfigValidator::validateServer(const ServerConfig& server)
{
    int port = server.getListenPort();
    checkPortRange(port);

    if (server.getRoot().empty())
        throw std::runtime_error("Server missing root directive");

    checkRootExists(server.getRoot());
    checkIndexFiles(server.getIndexFiles());

    checkClientMaxBodySize(server.getMaxBodySize());

    const std::vector<LocationConfig>& locations =
        server.getLocations();

    for (size_t i = 0; i < locations.size(); i++) {
        validateLocation(locations[i]);
    }
        
}

/* ========================================= */
/*             LOCATION VALIDATION           */
/* ========================================= */

void ConfigValidator::validateLocation(const LocationConfig& location)
{
    /* ---------- PATH VALIDATION ---------- */

    if (location.getPath().empty())
        throw std::runtime_error("Location path is empty");

    if (location.getPath()[0] != '/')
        throw std::runtime_error("Location path must start with '/'");

    /* ---------- ROOT VALIDATION ---------- */

    if (location.getRoot().empty())
        throw std::runtime_error("Location has no root defined");
    
    checkRootExists(location.getRoot());

    /* ---------- METHODS VALIDATION ---------- */

    checkMethods(location.getMethods());
    checkIndexFiles(location.getIndexFiles());
    /* ---------- RETURN (REDIRECT) VALIDATION ---------- */

    
    // if (location.hasRedirect())
    // {
    //     // int code = location.getRedirectCode();
    //     // std::string url = location.getRedirectUrl();

    //     // Check redirect status code
    //     // printf("-------------%d\n", code);
    //     // if (code / 100 != 3) {
    //     //     printf("-------------%d\n", code);
    //     //     throw std::runtime_error("Invalid redirect status code");
    //     // }

    //     // Check redirect URL
    //     // if (url.empty() ||
    //     //     (url[0] != '/' &&
    //     //      url.find("http://") != 0 &&
    //     //      url.find("https://") != 0))
    //     // {
    //     //     throw std::runtime_error(
    //     //         "Invalid redirect URL: must start with '/' or http:// or https://");
    //     // }
    // }
}
/* ========================================= */
/*               CHECKERS                    */
/* ========================================= */

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
        if (!indexFiles[i].empty() && indexFiles[i][0] == '/')
        {
            throw std::runtime_error("Index file must NOT start with '/'");
        }
    }
}

/* ========================================= */
/*             INHERITANCE LOGIC             */
/* ========================================= */
void ConfigValidator::applyInheritance(HttpConfig& http)
{
    std::vector<ServerConfig>& servers = http.getServers();

    for (size_t i = 0; i < servers.size(); i++)
    {
        ServerConfig& server = servers[i];

        /* ---------- SERVER inherits from HTTP ---------- */

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

        /* ---------- LOCATION inherits from SERVER ---------- */

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