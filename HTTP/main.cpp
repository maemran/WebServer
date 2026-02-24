/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/02/24 07:34:41 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../HttpConfig.hpp"
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
        "Hello this is POST body!!";

    HttpRequest obj;
    obj.requestParsing(request);
    std::cout << request << std::endl;
}