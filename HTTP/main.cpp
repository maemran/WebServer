/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/02/25 15:56:32 by maemran          ###   ########.fr       */
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
        "POST /upload HTTP/1.1\r\n"
        "Host: localhost:7070\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 27\r\n"
        "Connection: close\r\n"
        "\r\n"
        "oooooooooo";

    HttpRequest obj;
    obj.requestParsing(request);
    //std::cout << request << std::endl;
}