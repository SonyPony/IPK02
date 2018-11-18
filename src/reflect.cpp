/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#include "reflect.h"
#include <iostream>
#include <stdexcept>
#include "probe.h"
#include "connection.h"

int reflect(ArgumentsServer args) {
    Connection connection{std::string(""), args.port, Probe::SIZE, true};
    try {
        connection.initServer();
    }

    catch(ConnectionError& e) {
        cerr << e.what() << endl;
        return 1;
    }

    connection.setReadCallback([&connection](ustring data, struct sockaddr* sender) {
        if(data.size() < Probe::SIZE)
            return;
        Probe probe_message{(unsigned char *) data.c_str()};

        if (0 > connection.sendData(probe_message.serialize(), Probe::SIZE))
            exit(44);
    });

    connection.startRead();

    return 0;
}
