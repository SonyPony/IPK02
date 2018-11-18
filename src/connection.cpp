/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#include "connection.h"
#include <sys/socket.h> // socket
#include <arpa/inet.h> // inet_aton
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <iostream>

Connection::Connection(std::string host, uint16_t port,
                       size_t bufferSize, bool passReadError, bool reading) {
    m_bufferSize = bufferSize;
    m_reading = reading;
    m_host = host;
    m_port = port;
    m_passReadError = passReadError;
}

void Connection::initClient(unsigned int timeout) {
    struct hostent *serverHostname = gethostbyname(m_host.c_str());

    if (serverHostname == nullptr) {
        throw ConnectionError("Cannot resolve host name.");
    }

    bzero((char *) &m_serverAddr, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;

    // TODO delete
    m_serverAddr.sin_addr = ((struct in_addr *) serverHostname->h_addr_list)[0];

    bcopy((char *)serverHostname->h_addr, (char *)&m_serverAddr.sin_addr.s_addr, serverHostname->h_length);
    m_serverAddr.sin_port = htons(static_cast<uint16_t>(m_port));

    // initClient socket
    if((m_socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
        throw ConnectionError("Socket creation error.");

    struct timeval socketTimeout={0, timeout}; //set timeout for 2 seconds
    setsockopt(m_socketFd,SOL_SOCKET,SO_RCVTIMEO,(char*)&socketTimeout, sizeof(struct timeval));
}

void Connection::initServer() {
    int opt = 1;
    if ((m_socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
        throw ConnectionError("Socket creation error.");

    if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        throw ConnectionError("Unable to setup socket.");

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;
    m_serverAddr.sin_port = htons(m_port);

    if (0 < ::bind(m_socketFd, (struct sockaddr *) &m_serverAddr, sizeof(m_serverAddr)))
        throw ConnectionError("Unable to bind.");

}

void Connection::startRead() {
    ssize_t readed_bytes = 0;
    unsigned char buffer[m_bufferSize];
    m_reading = true;
    socklen_t addr_len = sizeof(sockaddr_in);

    while(m_reading)
    {
        readed_bytes = recvfrom(m_socketFd,
                                buffer, sizeof(buffer), 0,
                                (struct sockaddr *) &m_serverAddr,
                                &addr_len);

        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            errno = 0;
            m_reading = false;
        }

        else if(readed_bytes <= 0 && !m_passReadError) {
            cerr << "Read error." << endl;
            exit(EXIT_FAILURE);
        }
        else
            m_readCallback(ustring(buffer, readed_bytes), (struct sockaddr *) &m_serverAddr);
    }
}

ssize_t Connection::sendData(ustring data) const {
    return sendto(m_socketFd, data.c_str(), data.size(), 0, (const struct sockaddr*) &m_serverAddr, sizeof(m_serverAddr));
}

ssize_t Connection::sendData(const unsigned char *data, size_t dataLen) const {
    return sendto(m_socketFd, data, dataLen, 0, (const struct sockaddr*) &m_serverAddr, sizeof(m_serverAddr));
}

void Connection::stopRead() {
    m_reading = false;
}

void Connection::setReadCallback(std::function<void(ustring, struct sockaddr*)> callback) {
    m_readCallback = callback;
}

void Connection::closeSocket() {
    close(m_socketFd);
}

