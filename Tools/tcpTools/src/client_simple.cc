#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.hpp"

static volatile bool iswork = true;

int client_simple()
{
    int server_fd, rc = 0;
    struct sockaddr_in server_addr;
    char buffer[1024] = {};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        puts("socket failed");
        return server_fd;
    }

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);

    rc = connect(server_fd, (struct sockaddr *) &server_addr,
                 sizeof(server_addr));
    if (rc < 0) {
        puts("connect failed");
        return rc;
    }

    strncpy(buffer, "hello", sizeof(buffer));
    rc = send(server_fd, buffer, strlen(buffer), 0);
    printf("send[%d] < %s\n", rc, buffer);

    close(server_fd);
    return rc;
}

int client_loop_simple()
{
    int server_fd, rc = 0;
    struct sockaddr_in server_addr;
    char buffer[1024] = {};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        puts("socket failed");
        return server_fd;
    }

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);

    rc = connect(server_fd, (struct sockaddr *) &server_addr,
                 sizeof(server_addr));
    if (rc < 0) {
        puts("connect failed");
        return rc;
    }

    char *ptr = NULL;

    while (iswork) {
        fgets(buffer, sizeof(buffer), stdin);

        ptr = strchr(buffer, '\n');
        if (ptr) {
            *ptr = '\0';
        }

        rc = send(server_fd, buffer, strlen(buffer), MSG_NOSIGNAL);
        if (rc > 0) {
            printf("send[%d] < %s\n", rc, buffer);
        } else {
            puts("send failed");
            rc = recv(server_fd, buffer, sizeof(buffer), O_NONBLOCK);
            if (rc == 0) {
                puts("peer close connction, client exit");
                perror(strerror(errno));
                iswork = false;
                continue;
            }
            perror(strerror(errno));
        }

        if (strcmp(buffer, "quit") == 0) {
            iswork = false;
        }
    }
    puts("client close...");
    close(server_fd);
    return rc;
}
