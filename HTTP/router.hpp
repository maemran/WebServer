/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 02:48:40 by maemran           #+#    #+#             */
/*   Updated: 2026/03/15 03:45:30 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "../LocationConfig.hpp"

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

        const std::string& getRoutedPath() const;
        const std::map<int, std::string>& getRoutedErrorPages() const;
};

#endif