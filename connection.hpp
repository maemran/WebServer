#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>

class Connection
{
private:
    int clientFd;
    std::string clientIp;
    int clientPort;
    std::string serverIp;
    int serverPort;
    size_t serverIndex;
public:
    Connection(int fd, const std::string& cIp, int cPort,const std::string& sIp, int sPort, size_t sIndex);
    ~Connection();
    Connection(const Connection& other);
        Connection&	operator=(const Connection& other);
    int getClientFd() const;
    std::string getClientIp() const;
    int getClientPort() const;
    std::string getServerIp() const;
    int getServerPort() const;
    size_t getServerIndex() const;
};

#endif