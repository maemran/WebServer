/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:10:00 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/06 15:18:09 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIE_HPP
#define COOKIE_HPP

#include <string>
#include <map>
#include <vector>
#include <ctime>

/**
 * @class Cookie
 * @brief Represents a single HTTP cookie with name, value, and attributes
 * 
 * HTTP Cookies are used to store client-side data. This class manages:
 * - Cookie name and value
 * - Expiration date
 * - Domain restrictions
 * - Path restrictions
 * - Security attributes (HttpOnly, Secure)
 */
class Cookie
{
    public:
        // Constructors
        Cookie();
        Cookie(const std::string& name, const std::string& value);
        Cookie(const Cookie& other);
        Cookie& operator=(const Cookie& other);
        ~Cookie();

        // Getters
        const std::string& getName() const;
        const std::string& getValue() const;
        const std::string& getDomain() const;
        const std::string& getPath() const;
        time_t getExpires() const;
        int getMaxAge() const;
        bool isSecure() const;
        bool isHttpOnly() const;
        bool isSameSite() const;

        // Setters
        void setName(const std::string& name);
        void setValue(const std::string& value);
        void setDomain(const std::string& domain);
        void setPath(const std::string& path);
        void setExpires(time_t expires);
        void setMaxAge(int maxAge);
        void setSecure(bool secure);
        void setHttpOnly(bool httpOnly);
        void setSameSite(bool sameSite);

        // Generate Set-Cookie header value
        std::string toSetCookieHeader() const;
        
        // Parse from Cookie header value (name=value; name2=value2; ...)
        static std::map<std::string, std::string> parseCookieHeader(const std::string& cookieHeader);
        
        // Check if cookie is expired
        bool isExpired() const;

    private:
        std::string name;
        std::string value;
        std::string domain;
        std::string path;
        time_t expires;     // 0 means session cookie (expires when browser closes)
        int maxAge;         // -1 means not set
        bool secure;        // Only send over HTTPS
        bool httpOnly;      // Not accessible from JavaScript
        bool sameSite;      // CSRF protection
};

#endif
