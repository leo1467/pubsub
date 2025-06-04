#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.hpp"

void *Get_addr(void *void_client_addr)
{
    sockaddr_storage *client_addr = (sockaddr_storage *) void_client_addr;
    if (client_addr->ss_family == AF_INET) {
        return &((sockaddr_in *) client_addr)->sin_addr;
    } else if (client_addr->ss_family == AF_INET6) {
        return &((sockaddr_in6 *) client_addr)->sin6_addr;
    }
    return NULL;
}
