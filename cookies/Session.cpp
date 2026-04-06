/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:15:00 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/06 15:21:10 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Session.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

/* ===================================== */
/*        Session Implementation         */
/* ===================================== */

Session::Session()
    : sessionId(""), createdTime(time(NULL)), lastAccessTime(time(NULL)),
      timeoutMinutes(30) {}

Session::Session(const std::string& sessionId)
    : sessionId(sessionId), createdTime(time(NULL)), 
      lastAccessTime(time(NULL)), timeoutMinutes(30) {}

Session::Session(const Session& other)
    : sessionId(other.sessionId), data(other.data), 
      createdTime(other.createdTime), lastAccessTime(other.lastAccessTime),
      timeoutMinutes(other.timeoutMinutes) {}

Session& Session::operator=(const Session& other)
{
    if (this != &other)
    {
        sessionId = other.sessionId;
        data = other.data;
        createdTime = other.createdTime;
        lastAccessTime = other.lastAccessTime;
        timeoutMinutes = other.timeoutMinutes;
    }
    return *this;
}

Session::~Session() {}

/* ===================================== */
/*            Getters                    */
/* ===================================== */

const std::string& Session::getSessionId() const { return sessionId; }
time_t Session::getCreatedTime() const { return createdTime; }
time_t Session::getLastAccessTime() const { return lastAccessTime; }
int Session::getTimeoutMinutes() const { return timeoutMinutes; }
const std::map<std::string, std::string>& Session::getData() const { return data; }

/* ===================================== */
/*        Session Data Management        */
/* ===================================== */

std::string Session::get(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = data.find(key);
    if (it != data.end())
        return it->second;
    return "";
}

std::string Session::get(const std::string& key, const std::string& defaultValue) const
{
    std::map<std::string, std::string>::const_iterator it = data.find(key);
    if (it != data.end())
        return it->second;
    return defaultValue;
}

bool Session::has(const std::string& key) const
{
    return (data.find(key) != data.end());
}

void Session::set(const std::string& key, const std::string& value)
{
    data[key] = value;
    updateLastAccessTime();
}

void Session::remove(const std::string& key)
{
    data.erase(key);
}

void Session::clear()
{
    data.clear();
}

/* ===================================== */
/*        Session Lifecycle              */
/* ===================================== */

void Session::setTimeoutMinutes(int minutes)
{
    timeoutMinutes = minutes;
}

void Session::updateLastAccessTime()
{
    lastAccessTime = time(NULL);
}

bool Session::isExpired() const
{
    time_t now = time(NULL);
    time_t elapsed = now - lastAccessTime;
    return (elapsed > (timeoutMinutes * 60));
}

bool Session::isExpired(int timeoutSeconds) const
{
    time_t now = time(NULL);
    time_t elapsed = now - lastAccessTime;
    return (elapsed > timeoutSeconds);
}

/* ===================================== */
/*     SessionManager Implementation     */
/* ===================================== */

// Initialize static member
SessionManager* SessionManager::instance = NULL;

SessionManager& SessionManager::getInstance()
{
    if (instance == NULL)
        instance = new SessionManager();
    return *instance;
}

SessionManager::SessionManager() : defaultTimeoutMinutes(30)
{
    srand(time(NULL));
}

SessionManager::~SessionManager() {}

/* ===================================== */
/*      Session Management Methods       */
/* ===================================== */

Session SessionManager::createSession()
{
    std::string sessionId = generateSessionId();
    Session newSession(sessionId);
    newSession.setTimeoutMinutes(defaultTimeoutMinutes);
    sessions[sessionId] = newSession;
    return newSession;
}

Session& SessionManager::getSession(const std::string& sessionId)
{
    if (sessions.find(sessionId) != sessions.end())
    {
        sessions[sessionId].updateLastAccessTime();
        return sessions[sessionId];
    }
    
    // Return a dummy session if not found (handle this better in production)
    static Session dummy;
    return dummy;
}

bool SessionManager::hasSession(const std::string& sessionId) const
{
    return (sessions.find(sessionId) != sessions.end() &&
            !sessions.at(sessionId).isExpired());
}

void SessionManager::removeSession(const std::string& sessionId)
{
    sessions.erase(sessionId);
}

void SessionManager::clearExpiredSessions()
{
    std::map<std::string, Session>::iterator it = sessions.begin();
    while (it != sessions.end())
    {
        if (it->second.isExpired())
        {
            std::map<std::string, Session>::iterator temp = it;
            ++it;
            sessions.erase(temp);
        }
        else
            ++it;
    }
}

void SessionManager::setSessionTimeout(int minutes)
{
    defaultTimeoutMinutes = minutes;
}

int SessionManager::getSessionTimeout() const
{
    return defaultTimeoutMinutes;
}

/* ===================================== */
/*      Helper: Generate Session ID      */
/* ===================================== */

std::string SessionManager::generateSessionId()
{
    std::stringstream ss;
    
    // Generate a pseudo-random session ID
    // Format: SID_<timestamp>_<random>
    ss << "SID_" << time(NULL) << "_" << (rand() % 1000000);
    
    return ss.str();
}
