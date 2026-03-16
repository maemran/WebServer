/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/03/17 01:02:58 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../HttpConfig.hpp"
#include  "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "../HttpConfig.hpp"

#include <iostream>

// 405 Method Not Allowed   if the location not allow the method

//400   Bad Request
//501   Not Implemented
//505   HTTP Version Not Supported
int main(void)
{
    // HttpConfig config;
    // ServerConfig server;

    HttpConfig config;
    config.setRoot("/var/www/http");
    config.setIndex("index.html");
    config.setAutoindex(false);
    config.setMaxBodySize(1024 * 1024);
    config.addAllowedMethod("GET");
    config.addAllowedMethod("POST");
    config.addErrorPage(404, "/errors/404.html");

    ServerConfig serverOne;
    serverOne.setListenIp("127.0.0.1");
    serverOne.setListenPort(8080);
    serverOne.setRoot("/var/www/server_one");
    serverOne.setIndex("index.html");
    serverOne.setAutoindex(false);
    serverOne.setMaxBodySize(2 * 1024 * 1024);
    serverOne.addAllowedMethod("GET");
    serverOne.addAllowedMethod("POST");
    serverOne.addErrorPage(500, "/errors/500.html");

    LocationConfig serverOneRoot;
    serverOneRoot.setPath("/hello/");
    serverOneRoot.setRoot("./var/www/server_one/public");
    serverOneRoot.setIndex("index.html");
    serverOneRoot.setAutoindex(false);
    serverOneRoot.setMaxBodySize(1024 * 1024);
    serverOneRoot.addAllowedMethod("GET");
    serverOneRoot.addAllowedMethod("POST");
    serverOneRoot.addErrorPage(404, "errors/404.html");

    LocationConfig serverOneApi;
    serverOneApi.setPath("/api");
    serverOneApi.setRoot("/var/www/server_one/api");
    serverOneApi.setIndex("index.json");
    serverOneApi.setAutoindex(false);
    serverOneApi.setMaxBodySize(512 * 1024);
    serverOneApi.addAllowedMethod("GET");
    serverOneApi.addAllowedMethod("POST");
    serverOneApi.addAllowedMethod("DELETE");
    serverOneApi.addRedirection(301, "https://api.server-one.local/v1");
    serverOneApi.addErrorPage(404, "errors/404.html");

    serverOne.addLocation(serverOneRoot);
    serverOne.addLocation(serverOneApi);

    ServerConfig serverTwo;
    serverTwo.setListenIp("127.0.0.1");
    serverTwo.setListenPort(9090);
    serverTwo.setRoot("/var/www/server_two");
    serverTwo.setIndex("home.html");
    serverTwo.setAutoindex(true);
    serverTwo.setMaxBodySize(4 * 1024 * 1024);
    serverTwo.addAllowedMethod("GET");
    serverTwo.addAllowedMethod("POST");
    serverTwo.addAllowedMethod("DELETE");
    serverTwo.addErrorPage(403, "/errors/403.html");

    LocationConfig serverTwoRoot;
    serverTwoRoot.setPath("/");
    serverTwoRoot.setRoot("/var/www/server_two/public");
    serverTwoRoot.setIndex("home.html");
    serverTwoRoot.setAutoindex(true);
    serverTwoRoot.setMaxBodySize(1024 * 1024);
    serverTwoRoot.addAllowedMethod("GET");
    serverTwoRoot.addAllowedMethod("POST");
    serverTwoRoot.addErrorPage(404, "/errors/404.html");

    LocationConfig serverTwoUploads;
    serverTwoUploads.setPath("/uploads");
    serverTwoUploads.setRoot("/var/www/server_two/uploads");
    serverTwoUploads.setIndex("upload.html");
    serverTwoUploads.setAutoindex(false);
    serverTwoUploads.setMaxBodySize(8 * 1024 * 1024);
    serverTwoUploads.addAllowedMethod("POST");
    serverTwoUploads.addAllowedMethod("DELETE");
    serverTwoUploads.addErrorPage(413, "/errors/413.html");

    serverTwo.addLocation(serverTwoRoot);
    serverTwo.addLocation(serverTwoUploads);

    config.addServer(serverOne);
    config.addServer(serverTwo);
    (void)config;

    std::string request =
        "GET /ap/ HTTP/1.0\r\n" // ///var/
        // "Host: localhost:7070\r\n"
        "User-Agent: Mozilla/5.0;\r\n";
        // "Content-Type: text/plain;\r\n"
        // "Content-Length: 27\r\n"
        // "Connection: close\r\n"
        // "\r\n"
        // "ooooooooo\no"

    HttpRequest obj;
    obj.requestHandler(request);
    HttpResponse resp(obj, config, 0);
    std::cout << "===============================" << std::endl;
    std::cout << "===> HTTP Response Handler <===" << std::endl;
    std::cout << "===============================" << std::endl;
    resp.responseHandler();
    //std::cout << request << std::endl;
}