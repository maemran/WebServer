/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 06:55:15 by maemran           #+#    #+#             */
/*   Updated: 2026/03/20 02:07:17 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../serversocket.hpp"
#include "../HttpConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <iostream>

int main(void)
{
    try
    {
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
        serverOne.setRoot("../webroot");
        serverOne.setIndex("index.html");
        serverOne.setAutoindex(false);
        serverOne.setMaxBodySize(2 * 1024 * 1024);
        serverOne.addAllowedMethod("GET");
        serverOne.addAllowedMethod("POST");
        serverOne.addErrorPage(404, "/error_pages/404.html");

        LocationConfig serverOneRoot;
        serverOneRoot.setPath("/images");
        serverOneRoot.setRoot("../webroot");
        serverOneRoot.addIndexFile("image.stxt");
        serverOneRoot.setAutoindex(false);
        serverOneRoot.setMaxBodySize(1024 * 1024);
        serverOneRoot.addAllowedMethod("GET");
        serverOneRoot.addAllowedMethod("HEAD");
        serverOneRoot.addErrorPage(404, "error_pages/404.html");

        LocationConfig serverOneApi;
        serverOneApi.setPath("/images/iconss");
        serverOneApi.setRoot("../webroot");
        serverOneApi.setIndex("index.json");
        serverOneApi.setAutoindex(false);
        serverOneApi.setMaxBodySize(512 * 1024);
        serverOneApi.addAllowedMethod("GET");
        serverOneApi.addAllowedMethod("POST");
        serverOneApi.addAllowedMethod("DELETE");
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

        // Start the socket server
        ServerSocket server(config);
        server.start();
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}