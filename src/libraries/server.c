#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080
#define BUF_SIZE 1024

void handle_client(int client_socket, int parent_to_child[], int child_to_parent[]) {
    close(parent_to_child[1]);
    close(child_to_parent[0]);

    char buffer[BUF_SIZE];
    while (1) {
        int read_size = read(client_socket, buffer, BUF_SIZE - 1);
        if (read_size <= 0) {
            break;
        }

        buffer[read_size] = '\0';
        write(parent_to_child[0], buffer, strlen(buffer));

        read(child_to_parent[1], buffer, BUF_SIZE - 1);
        write(client_socket, buffer, strlen(buffer));
    }

    close(client_socket);
    exit(0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    pid_t pid;

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) == -1) {
        perror("listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        client_addr_len = sizeof(client_addr);
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) {
            perror("accept failed");
            continue;
        }

        int parent_to_child[2];
        int child_to_parent[2];
        pipe(parent_to_child);
        pipe(child_to_parent);

        if ((pid = fork()) == 0) {
            close(server_socket);
            handle_client(client_socket, parent_to_child, child_to_parent);
        } else {
            close(client_socket);
            close(parent_to_child[0]);
            close(child_to_parent[1]);

            char buffer[BUF_SIZE];
            while (1) {
                read(parent_to_child[1], buffer, BUF_SIZE - 1);
                printf("Message from client: %s\n", buffer);

                // Echo the message back
                write(child_to_parent[0], buffer, strlen(buffer));
            }
        }
    }

    close(server_socket);
    return 0;
}
