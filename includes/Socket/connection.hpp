#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <cstdlib>
#include <iostream>

enum State
{
    READING,   // accumulating request data
    SENDING,   // writing response (may need multiple sends)
    DONE       // finished, ready to close
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

    std::string buffer;        // raw incoming data
    std::string lastRequest;   // full request
    std::string writeBuffer;   // response
    size_t bytesSent;
    State state;
    bool headerParsed;
    size_t expectedRequestSize;

    void updateRequestTracking();
    void resetRequestTracking();

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

    // 🔥 NEW FUNCTIONS

    std::string& getBuffer();
    void append(const char* data, size_t size);

    bool isRequestComplete();
    std::string extractRequest();
    void consumeRequest();

    const std::string& getRequest() const;

    void setResponse(const std::string& res);
    std::string& getWriteBuffer();
    size_t& getBytesSent();
    std::string getMethod();

    State getState() const;
    void setState(State s);
};

#endif