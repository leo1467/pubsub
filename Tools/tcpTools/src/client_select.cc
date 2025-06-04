#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

#include "common.hpp"

static volatile bool iswork = true;

static void handle_signal(int sig)
{
    printf("Interrupt by signal[%d]\n", sig);
    iswork = false;
}

static void block_signal()
{
    signal(SIGINT, handle_signal);
}

int client_select()
{
    block_signal();

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
    char ip_str[INET6_ADDRSTRLEN] = {};
    inet_ntop(server_addr.sin_family, Get_addr(&server_addr), ip_str,
              sizeof(server_addr));
    printf("connect to server[%s]\n", ip_str);

    fd_set read_fds, all_fds;
    FD_ZERO(&all_fds);
    FD_SET(server_fd, &all_fds);
    FD_SET(STDIN_FILENO, &all_fds);

    char *ptr = NULL;
    int maxfd = server_fd > STDIN_FILENO ? server_fd : STDIN_FILENO;
    while (iswork) {
        read_fds = all_fds;
        printf("Enter msg : ");
        fflush(stdout);
        int ready = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                puts("select failed");
                perror(strerror(errno));
            }
        } else if (ready == 0) {
            puts("continue select");
            continue;
        }

        if (FD_ISSET(server_fd, &read_fds)) {
            rc = recv(server_fd, buffer, sizeof(buffer), 0);
            if (rc > 0) {
                buffer[rc] = '\0';
                printf("recv[%s][%d] > %s\n", ip_str, rc, buffer);
            } else if (rc == 0) {
                printf("\npeer close connction\n");
                iswork = false;
                continue;
            } else {
                puts("recv failed");
                perror(strerror(errno));
            }
        }
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            fgets(buffer, sizeof(buffer), stdin);

            ptr = strchr(buffer, '\n');
            if (ptr) {
                *ptr = '\0';
            }

            if (strlen(buffer) == 0) {
                puts("No msg entered");
                continue;
            }
            rc = send(server_fd, buffer, strlen(buffer), MSG_NOSIGNAL);
            if (rc > 0) {
                printf("send[%d] < %s\n", rc, buffer);
            } else {
                puts("send failed");
                perror(strerror(errno));
            }

            if (strcmp(buffer, "quit") == 0) {
                iswork = false;
            }
        }
    }
    puts("client close...");
    close(server_fd);
    return rc;
}
