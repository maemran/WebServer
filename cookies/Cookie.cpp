/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:10:00 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/06 15:18:27 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Cookie.hpp"
#include <sstream>
#include <ctime>

Cookie::Cookie() 
    : name(""), value(""), domain(""), path("/"), expires(0), 
      maxAge(-1), secure(false), httpOnly(false), sameSite(false) {}

Cookie::Cookie(const std::string& name, const std::string& value)
    : name(name), value(value), domain(""), path("/"), expires(0),
      maxAge(-1), secure(false), httpOnly(false), sameSite(false) {}

Cookie::Cookie(const Cookie& other)
    : name(other.name), value(other.value), domain(other.domain), 
      path(other.path), expires(other.expires), maxAge(other.maxAge),
      secure(other.secure), httpOnly(other.httpOnly), sameSite(other.sameSite) {}

Cookie& Cookie::operator=(const Cookie& other)
{
    if (this != &other)
    {
        name = other.name;
        value = other.value;
        domain = other.domain;
        path = other.path;
        expires = other.expires;
        maxAge = other.maxAge;
        secure = other.secure;
        httpOnly = other.httpOnly;
        sameSite = other.sameSite;
    }
    return *this;
}

Cookie::~Cookie() {}

/* ===================================== */
/*            Getters                    */
/* ===================================== */

const std::string& Cookie::getName() const { return name; }
const std::string& Cookie::getValue() const { return value; }
const std::string& Cookie::getDomain() const { return domain; }
const std::string& Cookie::getPath() const { return path; }
time_t Cookie::getExpires() const { return expires; }
int Cookie::getMaxAge() const { return maxAge; }
bool Cookie::isSecure() const { return secure; }
bool Cookie::isHttpOnly() const { return httpOnly; }
bool Cookie::isSameSite() const { return sameSite; }

/* ===================================== */
/*            Setters                    */
/* ===================================== */

void Cookie::setName(const std::string& n) { name = n; }
void Cookie::setValue(const std::string& v) { value = v; }
void Cookie::setDomain(const std::string& d) { domain = d; }
void Cookie::setPath(const std::string& p) { path = p; }
void Cookie::setExpires(time_t e) { expires = e; }
void Cookie::setMaxAge(int m) { maxAge = m; }
void Cookie::setSecure(bool s) { secure = s; }
void Cookie::setHttpOnly(bool h) { httpOnly = h; }
void Cookie::setSameSite(bool s) { sameSite = s; }

/* ===================================== */
/*      Generate Set-Cookie Header       */
/* ===================================== */

std::string Cookie::toSetCookieHeader() const
{
    std::stringstream ss;
    
    // Name=Value (required)
    ss << name << "=" << value;
    
    // Path (default is /)
    ss << "; Path=" << path;
    
    // Domain (optional)
    if (!domain.empty())
        ss << "; Domain=" << domain;
    
    // Expires or Max-Age
    if (expires > 0)
    {
        char buffer[100];
        struct tm* timeinfo = gmtime(&expires);
        strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
        ss << "; Expires=" << buffer;
    }
    else if (maxAge >= 0)
    {
        ss << "; Max-Age=" << maxAge;
    }
    
    // Security attributes
    if (secure)
        ss << "; Secure";
    
    if (httpOnly)
        ss << "; HttpOnly";
    
    if (sameSite)
        ss << "; SameSite=Strict";
    
    return ss.str();
}

/* ===================================== */
/*      Parse Cookie Header              */
/* ===================================== */

std::map<std::string, std::string> Cookie::parseCookieHeader(const std::string& cookieHeader)
{
    std::map<std::string, std::string> cookies;
    std::stringstream ss(cookieHeader);
    std::string pair;
    
    // Cookie header format: "name1=value1; name2=value2; name3=value3"
    while (std::getline(ss, pair, ';'))
    {
        // Trim leading whitespace
        size_t start = pair.find_first_not_of(" \t");
        if (start != std::string::npos)
            pair = pair.substr(start);
        
        // Trim trailing whitespace
        size_t end = pair.find_last_not_of(" \t");
        if (end != std::string::npos)
            pair = pair.substr(0, end + 1);
        
        // Split by '='
        size_t eqPos = pair.find('=');
        if (eqPos != std::string::npos)
        {
            std::string name = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);
            
            // Trim spaces around name and value
            if (!name.empty() && !value.empty())
                cookies[name] = value;
        }
    }
    
    return cookies;
}

/* ===================================== */
/*      Check if Cookie Expired          */
/* ===================================== */

bool Cookie::isExpired() const
{
    if (expires == 0)
        return false;  // Session cookie, never expires until browser closes
    
    time_t now = time(NULL);
    return (now > expires);
}
