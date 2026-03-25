/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:43:39 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/24 14:08:13 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Check:
// port range
// duplicate ports
// missing root
// invalid methods
// empty location path
// client_max_body_size
//upload rules

#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include <set>
#include <stdexcept>
#include <iostream>
#include <vector>

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
    

public:
    ConfigValidator();
    ~ConfigValidator();

    void validate(HttpConfig& http);
     void applyInheritance(HttpConfig& http);
     
};

#endif