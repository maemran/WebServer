/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/04/27 13:29:19 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpConfig.hpp"
#include "ConfigLexer.hpp"
#include "ConfigParser.hpp"
#include "ConfigValidator.hpp"
#include "serversocket.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

static std::string readFileContent(const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
        throw std::runtime_error("Failed to open config file: " + path);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static HttpConfig prepareHttpConfig(const std::string& path)
{
    std::string content = readFileContent(path);

    ConfigLexer lexer(content);
    std::vector<Token> tokens = lexer.tokenize();

    ConfigParser parser(tokens);
    HttpConfig config = parser.parse();
    ConfigValidator validator;
    validator.validate(config);

    return config;
}

// 405 Method Not Allowed   if the location not allow the method

//400   Bad Request
//501   Not Implemented
//505   HTTP Version Not Supported
int main(int argc, char **argv)
{
	try {
		std::string filename = "./config_file/config.conf";
    	if (argc == 2)
    	    filename = argv[1];
    	HttpConfig config = prepareHttpConfig(filename);
		ServerSocket server(config);
    	server.start();
    	server.run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
