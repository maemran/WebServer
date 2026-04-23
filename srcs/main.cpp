/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/04/22 21:02:36 by maemran          ###   ########.fr       */
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
		std::string filename = "config.conf";
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

    //std::string request =
    //    "GET /images/test.py?name=mohammad HTTP/1.0\r\n" // ///var/
    //    // "Host: localhost:7070\r\n"
    //    "User-Agent: Mozilla/5.0\r\n"
    //    "Content-Type: text/html\r\n"
    //    "Content-Length: 8\r\n"
    //    // "Connection: close\r\n"
    //    "\r\n"
    //    "mohammad";
	//std::cout << "hello" <<std::endl;
    //HttpRequest obj;
    //obj.requestHandler(request);
    //HttpResponse resp(obj, config, 0);
    //std::cout << "===============================" << std::endl;
    //std::cout << "===> HTTP Response Handler <===" << std::endl;
    //std::cout << "===============================" << std::endl;
    //resp.responseHandler();
    //std::cout << request << std::endl;
}
