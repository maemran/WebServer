/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 02:48:40 by maemran           #+#    #+#             */
/*   Updated: 2026/04/09 20:19:44 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "../../Config/include/ServerConfig.hpp"

class   router
{
    private:
        std::string routedPath;
        std::map<int , std::string> routedErrorPages;

        void    addForwardSlash(std::string& str);
    public:
        router();
        router(const router& other);
        router& operator=(const router& other);
        ~router();
        router(const LocationConfig& loc);
        router(const ServerConfig& serv);

        const std::string& getRoutedPath() const;
        const std::map<int, std::string>& getRoutedErrorPages() const;
};

#endif