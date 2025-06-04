#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include "common.hpp"

static volatile int iswork = true;

static void handle_signal(int sig)
{
    printf("Interrupt by signal[%d]\n", sig);
    iswork = false;
}

typedef struct Client_Obj_t {
    char ip_str[INET6_ADDRSTRLEN];
    int fd;
} Client_Obj_t;

static void clear_client_obj(Client_Obj_t *obj)
{
    obj->fd = -1;
    memset(obj->ip_str, 0, sizeof(obj->ip_str));
}

static void block_signal()
{
    signal(SIGINT, handle_signal);
}

int server_select()
{
    block_signal();
    int rc = 0;
    int server_fd, client_fd;
    struct addrinfo hints, *serverinfo, *p;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    Client_Obj_t client_objs[CLIENT_MAX] = {};
    char buffer[1024] = {};

    for (int i = 0; i < CLIENT_MAX; ++i) {
        client_objs[i].fd = -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    rc = getaddrinfo("localhost", PORT_STR, &hints, &serverinfo);
    if (rc < 0) {
        // perror(strerror(errno));
        perror(gai_strerror(rc));
        // fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return rc;
    }

    for (p = serverinfo; p != NULL; p = p->ai_next) {
        server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_fd >= 0) {
            break;
        }
    }

    if (p == NULL) {
        perror("domain not available");
        return -1;
    }
    freeaddrinfo(serverinfo);

    int yes = 1;
    rc = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (rc < 0) {
        puts("setsockopt failed");
        perror(strerror(errno));
        return rc;
    }

    // int flag = fcntl(server_fd, F_GETFL, 0);
    // fcntl(server_fd, F_SETFL, flag | O_NONBLOCK);

    rc = bind(server_fd, (struct sockaddr *) p->ai_addr, p->ai_addrlen);
    if (rc < 0) {
        puts("setsockopt failed");
        perror(strerror(errno));
        return rc;
    }

    rc = listen(server_fd, 10);
    if (rc < 0) {
        puts("listen failed");
        perror(strerror(errno));
        return rc;
    }

    fd_set read_fds, all_fds;
    FD_ZERO(&all_fds);
    FD_SET(server_fd, &all_fds);
    int maxfd = server_fd;

    puts("server waiting for client connection...");
    while (iswork) {
        read_fds = all_fds;
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
            client_fd = accept(server_fd, (struct sockaddr *) &client_addr,
                               &client_addr_len);
            if (client_fd < 0) {
                puts("accept failed");
                perror(strerror(errno));
                continue;
            }
            int i = 0;
            bool reject = true;
            for (i = 0; i < CLIENT_MAX; ++i) {
                if (client_objs[i].fd != -1) {
                    continue;
                }
                reject = false;
                client_objs[i].fd = client_fd;
                FD_SET(client_fd, &all_fds);
                if (client_fd > maxfd) {
                    maxfd = client_fd;
                }
                break;
            }
            if (reject) {
                puts("Exceed MAX connection, reject connection...");
                close(client_fd);
                continue;
            }
            inet_ntop(client_addr.ss_family, Get_addr(&client_addr),
                      client_objs[i].ip_str, sizeof(client_addr));
            printf("accept new connection from [%s]\n", client_objs[i].ip_str);
        }

        for (int i = 0; i < CLIENT_MAX; ++i) {
            int fd = client_objs[i].fd;
            if (fd == -1) {
                continue;
            }
            if (FD_ISSET(fd, &read_fds)) {
                // printf("handle fd[%d]\n", fd);
                rc = recv(fd, buffer, sizeof(buffer), 0);
                if (rc > 0) {
                    buffer[rc] = '\0';
                    printf("recv[%s][%d] > %s\n", client_objs[i].ip_str, rc,
                           buffer);
                } else if (rc == 0) {
                    printf("client[%s] close connection...\n",
                           client_objs[i].ip_str);
                    FD_CLR(fd, &all_fds);
                    close(fd);
                    clear_client_obj(&client_objs[i]);
                }
                if (--ready <= 0) {
                    break;
                }
            }
        }
    }
    puts("close server...");
    for (int i = 0; i < CLIENT_MAX; ++i) {
        if (client_objs[i].fd == -1) {
            continue;
        }
        close(client_objs[i].fd);
        clear_client_obj(&client_objs[i]);
    }
    close(server_fd);
    return rc;
};
