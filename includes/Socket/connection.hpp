#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <ctime>

enum State
{
    READING,
    SENDING,
    DONE
};

class Connection
{
private:
    int clientFd;
    std::string clientIp;
    int clientPort;
    std::string serverIp;
    int serverPort;
    size_t serverIndex;

    std::string buffer;
    std::string lastRequest;
    std::string writeBuffer;
    size_t bytesSent;
    State state;
    bool headerParsed;
    size_t contentLength;
    size_t bodyBytesReceived;
    std::string bodyFilePath;
    int bodyFileFd;
    int sendFileFd;
    time_t lastActivity;

public:
    Connection(int fd, const std::string& cIp, int cPort,
               const std::string& sIp, int sPort, size_t sIndex);
    ~Connection();
    Connection(const Connection& other);
    Connection& operator=(const Connection& other);

    int getClientFd() const;
    std::string getClientIp() const;
    int getClientPort() const;
    std::string getServerIp() const;
    int getServerPort() const;
    size_t getServerIndex() const;

    std::string& getBuffer();
    void append(const char* data, size_t size);

    bool isRequestComplete();
    std::string extractRequest();
    void consumeRequest();

    const std::string& getRequest() const;
    const std::string& getBodyFilePath() const;

    void setResponse(const std::string& res);
    std::string& getWriteBuffer();
    size_t& getBytesSent();
    std::string getMethod();

    State getState() const;
    void setState(State s);

    time_t getLastActivity() const;
    void updateLastActivity();

    int  getSendFileFd() const;
    void setSendFileFd(int fd);
    void closeSendFileFd();
};

#endif
