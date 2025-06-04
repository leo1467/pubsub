#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.hpp"

static volatile bool iswork = true;

int server_simple()
{
    int server_fd, client_fd, rc = 0;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {};
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        puts("socket failed");
        return server_fd;
    }

    int f = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &f, sizeof(f));

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    rc = bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rc < 0) {
        puts("bind failed");
        return rc;
    }
    listen(server_fd, 5);

    printf("waiting for client connection...\n");
    client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &addr_len);

    rc = read(client_fd, buffer, sizeof(buffer));
    printf("recv[%d] > %s\n", rc, buffer);

    close(client_fd);
    close(server_fd);

    return rc;
}
