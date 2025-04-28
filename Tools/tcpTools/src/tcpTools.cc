#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8888

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("等待 client 連線中...\n");
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    read(client_fd, buffer, sizeof(buffer));
    printf("收到 client 訊息：%s\n", buffer);

    const char *response = "Hello from server!";
    write(client_fd, response, strlen(response));

    close(client_fd);
    close(server_fd);
    return 0;
}
