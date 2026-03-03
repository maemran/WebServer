/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:52:56 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/02/23 11:52:57 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigValidator.hpp"
#include <sys/stat.h>

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

    checkClientMaxBodySize(server.getMaxBodySize());

    const std::vector<LocationConfig>& locations =
        server.getLocations();

    for (size_t i = 0; i < locations.size(); i++)
        validateLocation(locations[i]);
}

/* ========================================= */
/*             LOCATION VALIDATION           */
/* ========================================= */

void ConfigValidator::validateLocation(const LocationConfig& location)
{
    if (location.getPath().empty())
        throw std::runtime_error("Location path is empty");

    if (location.getRoot().empty())
        throw std::runtime_error("Location has no root defined");

    checkRootExists(location.getRoot());

    checkMethods(location.getMethods());
}

/* ========================================= */
/*               CHECKERS                    */
/* ========================================= */

void ConfigValidator::checkPortRange(int port)
{
    if (port < 1024 || port > 65535)
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
            methods[i] != "DELETE")
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

/* ========================================= */
/*             INHERITANCE LOGIC             */
/* ========================================= */

void ConfigValidator::applyInheritance(HttpConfig& http)
{
    std::vector<ServerConfig>& servers = http.getServers();

    for (size_t i = 0; i < servers.size(); i++)
    {
        ServerConfig& server = servers[i];

        // SERVER inherits from HTTP
        if (server.getRoot().empty())
            server.setRoot(http.getRoot());

        if (server.getIndex().empty())
            server.setIndex(http.getIndex());

        // LOCATION inherits from SERVER
        std::vector<LocationConfig>& locations = server.getLocations();

        for (size_t j = 0; j < locations.size(); j++)
        {
            LocationConfig& location = locations[j];

            if (location.getRoot().empty())
                location.setRoot(server.getRoot());

            if (location.getIndex().empty())
                location.setIndex(server.getIndex());
        }
    }
}