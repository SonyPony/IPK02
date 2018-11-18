/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <exception>
#include <functional>
#include <string>
#include <netinet/in.h> // socketaddr_in
#include <map>

using namespace std;
using ustring = basic_string<unsigned char>;

class ConnectionError: public std::exception {
    private:
        const char* m_msg;

    public:
        explicit ConnectionError(const char* msg): m_msg{msg} {}
        const char* what() const throw() override { return  m_msg; }
};

class Connection {
    private:
        struct sockaddr_in m_serverAddr;
        int m_socketFd;
        std::function<void(ustring, struct sockaddr*)> m_readCallback;
        size_t m_bufferSize;
        bool m_reading;
        std::string m_host;
        uint16_t m_port;
        bool m_passReadError;

    public:
        Connection(std::string host, uint16_t port,
                   size_t bufferSize = 1024, bool passReadError = true, bool reading = true);
        void initClient(unsigned int timeout = 100000);
        void initServer();

        void setReadCallback(std::function<void(ustring, struct sockaddr*)> callback);
        ssize_t sendData(ustring data) const;
        ssize_t sendData(const unsigned char *data, size_t dataLen) const;
        void startRead();
        void stopRead();

        void closeSocket();
};

#endif //CONNECTION_H
