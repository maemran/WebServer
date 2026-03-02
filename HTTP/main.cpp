/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/03/02 16:50:36 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../HttpConfig.hpp"
#include  "HttpRequest.hpp"

#include <iostream>

int main(void)
{
    // HttpConfig config;
    // ServerConfig server;

    std::string request =
        "POST http://webwhiteboard.com:9090/var/ HTTP/1.0\r\n" // if the URL include /r or /n    //split body form the massege inside the checker then check th /r/n
        // "Host: localhost:7070\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Type: text/plain\r\n\r\n"
        // "Content-Length: 27\r\n"
        "Connection: close\r\n"
        "\r\n"
        "oooooooooo";

    HttpRequest obj;
    if (!obj.requestCheck(request))
    {
        std::cout << "status-code: " << obj.getStatusCode() << std::endl;
        std::cout << "problem" << std::endl;
    }
    else
    {
        obj.requestParser(request);
		obj.getUri().uriCheck();
		if (obj.getUri().getUriError())
			std::cout << "probelm" << std::endl;
		else
        	obj.printClassAtributes();
    }
        
    //std::cout << request << std::endl;
}