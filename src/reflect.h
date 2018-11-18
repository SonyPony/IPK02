/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#ifndef REFLECT_H
#define REFLECT_H

#include <sys/socket.h>
#include "argumentparser.h"

bool process_client(int client_socket, sockaddr *addr, socklen_t *addr_len);

int reflect(ArgumentsServer args);

#endif //REFLECT_H
