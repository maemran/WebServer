/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/03/04 15:44:31 by maemran          ###   ########.fr       */
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
        "POST http:///webwhiteboard.com:9090/var/ HTTP/1.0\r\n" // ///var/
        // "Host: localhost:7070\r\n"
        "User-Agent: Mozilla/5.0\r\n";
        // "Content-Type: text/plain\r\n"
        // "Content-Length: 27\r\n"
        // "Connection: close\r\n"
        // "\r\n"
        // "ooooooooo\no";

    HttpRequest obj;
    obj.requestHandler(request);
        
    //std::cout << request << std::endl;
}