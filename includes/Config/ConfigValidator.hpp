/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:43:39 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/10 11:43:06 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include <set>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "HttpConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

class ConfigValidator
{
private:
    void validateServer(const ServerConfig& server);
    void validateLocation(const LocationConfig& location);

    void checkPortRange(int port);
    void checkDuplicatePorts(const std::vector<ServerConfig>& servers);
    void checkRootExists(const std::string& root);
    void checkMethods(const std::vector<std::string>& methods);
    void checkClientMaxBodySize(size_t size);
    void checkIndexFiles(const std::vector<std::string>& indexFiles);
    void checkErrorPages(const std::map<int, std::string>& errors,
                         const std::string& root);
    bool isValidIP(const std::string& ip);
    void checkListenIp(const std::string& ip);

public:
    ConfigValidator();
    ~ConfigValidator();

    void validate(HttpConfig& http);
     void applyInheritance(HttpConfig& http);
     
};

#endif