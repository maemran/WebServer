/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:15:00 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/06 15:18:09 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>
#include <map>
#include <ctime>

/**
 * @class Session
 * @brief Represents a single user session with stored data
 * 
 * Sessions are server-side storage for user data. This class manages:
 * - Session ID (unique identifier)
 * - Session data (key-value pairs)
 * - Creation time
 * - Last access time
 * - Session timeout
 */
class Session
{
    public:
        // Constructors
        Session();
        Session(const std::string& sessionId);
        Session(const Session& other);
        Session& operator=(const Session& other);
        ~Session();

        // Getters
        const std::string& getSessionId() const;
        time_t getCreatedTime() const;
        time_t getLastAccessTime() const;
        int getTimeoutMinutes() const;
        
        // Session data management
        const std::map<std::string, std::string>& getData() const;
        std::string get(const std::string& key) const;
        std::string get(const std::string& key, const std::string& defaultValue) const;
        bool has(const std::string& key) const;
        void set(const std::string& key, const std::string& value);
        void remove(const std::string& key);
        void clear();

        // Session lifecycle
        void setTimeoutMinutes(int minutes);
        void updateLastAccessTime();
        bool isExpired() const;
        bool isExpired(int timeoutSeconds) const;

    private:
        std::string sessionId;
        std::map<std::string, std::string> data;  // Session data storage
        time_t createdTime;                        // When session was created
        time_t lastAccessTime;                     // When session was last accessed
        int timeoutMinutes;                        // Session timeout in minutes
};

/**
 * @class SessionManager
 * @brief Manages multiple sessions for users
 * 
 * This is a simple in-memory session manager. For production, use:
 * - Database storage
 * - Redis/Memcached
 * - File-based persistence
 */
class SessionManager
{
    public:
        // Singleton instance
        static SessionManager& getInstance();

        // Session management
        Session createSession();
        Session& getSession(const std::string& sessionId);
        bool hasSession(const std::string& sessionId) const;
        void removeSession(const std::string& sessionId);
        void clearExpiredSessions();
        
        // Configuration
        void setSessionTimeout(int minutes);
        int getSessionTimeout() const;

    private:
        // Singleton pattern
        SessionManager();
        SessionManager(const SessionManager&);
        SessionManager& operator=(const SessionManager&);
        ~SessionManager();

        static SessionManager* instance;
        
        // Storage
        std::map<std::string, Session> sessions;
        int defaultTimeoutMinutes;

        // Helper: Generate unique session ID
        std::string generateSessionId();
};

#endif
