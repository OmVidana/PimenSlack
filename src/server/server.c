#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#define PORT 12345
#define BACKLOG 10
#define MAX_CLIENTS 10

typedef struct client_info {
    pid_t pid;
    int pipe_fd[2];
    int client_sock;
} client_info_t;

client_info_t clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client_to_list(pid_t pid) {
    FILE *file = fopen("list.txt", "a");
    if (file == NULL) {
        perror("ERROR opening list.txt");
        return;
    }
    fprintf(file, "Client PID: %d\n", pid);
    fclose(file);
}

void *handle_client(void *arg) {
    client_info_t *client = (client_info_t *) arg;
    char buffer[256];
    int n;


    fd_set read_fds;
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(client->client_sock, &read_fds);
        FD_SET(client->pipe_fd[0], &read_fds);
        int max_fd = client->client_sock > client->pipe_fd[0] ? client->client_sock : client->pipe_fd[0];

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR) {
            perror("select error");
            break;
        }

        if (FD_ISSET(client->pipe_fd[0], &read_fds)) {
            bzero(buffer, 256);
            n = read(client->pipe_fd[0], buffer, 255);
            if (n > 0) {
                printf("Read from pipe: %s\n", buffer);

                if (fcntl(client->client_sock, F_GETFL) != -1) {
                    n = write(client->client_sock, buffer, strlen(buffer));
                    if (n < 0) {
                        perror("ERROR writing to socket");
                        printf("client_sock: %d\n", client->client_sock);
                        printf("errno: %d\n", errno);
                        break;
                    }
                } else {
                    perror("ERROR writing to socket: Invalid file descriptor");
                    break;
                }
            }
        }
    }

    close(client->client_sock);
    close(client->pipe_fd[0]);
    pthread_exit(NULL);
}

void *handle_server_input(void *arg) {
    pid_t pid;
    char message[256];

    while (1) {
        printf("Enter PID to send message to: ");
        scanf("%d", &pid);
        printf("Enter message: ");
        scanf(" %[^\n]", message);

        pthread_mutex_lock(&client_mutex);
        for (int i = 0; i < client_count; i++) {
            if (clients[i].pid == pid) {
                printf("port: %d\n", clients[i].pipe_fd[1]);
                write(clients[i].pipe_fd[1], message, strlen(message));
                break;
            }
        }
        pthread_mutex_unlock(&client_mutex);
    }

    pthread_exit(NULL);
}

int main() {
    int server_sock, client_sock;
    socklen_t client_len;
    struct sockaddr_in server_addr, client_addr;
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    int optval = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("ERROR setting SO_REUSEADDR");
        close(server_sock);
        exit(1);
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR on binding");
        close(server_sock);
        exit(1);
    }

    listen(server_sock, BACKLOG);

    printf("Server listening on port %d\n", PORT);

    pthread_create(&tid, NULL, handle_server_input, NULL);

    while (1) {
        client_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_len);
        if (client_sock < 0) {
            perror("ERROR on accept");
            continue;
        }

        printf("Accepted client socket: %d\n", client_sock);

        if (client_count >= MAX_CLIENTS) {
            printf("Max clients reached, rejecting connection...\n");
            close(client_sock);
            continue;
        }

        if (pipe(clients[client_count].pipe_fd) == -1) {
            perror("ERROR creating pipe");
            close(client_sock);
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("ERROR on fork");
            close(client_sock);
            close(clients[client_count].pipe_fd[0]);
            close(clients[client_count].pipe_fd[1]);
            continue;
        }

        if (pid == 0) {
            close(server_sock);
            close(clients[client_count].pipe_fd[1]);
            client_info_t client = clients[client_count];
            client.client_sock = client_sock;
            handle_client((void *) &client);
            exit(0);
        } else {
            close(client_sock);
            close(clients[client_count].pipe_fd[0]);
            clients[client_count].pid = pid;
            clients[client_count].client_sock = client_sock;
            add_client_to_list(pid);
            client_count++;

            client_info_t *new_client_info = malloc(sizeof(client_info_t));
            if (new_client_info == NULL) {
                perror("ERROR allocating memory");
                close(client_sock);
                continue;
            }
            *new_client_info = clients[client_count - 1];

            pthread_t client_tid;
            pthread_create(&client_tid, NULL, handle_client, (void *) new_client_info);
        }
    }

    close(server_sock);
    return 0;
}
