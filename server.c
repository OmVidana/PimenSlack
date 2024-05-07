// Servidor
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    char username[32];
    char password[32];
} User;

User users[] = {
        {"bob", "password1"},
        {"alice", "password2"},
        {"charlie", "password3"},
        {"dave", "password4"},
        {"eve", "password5"},
};

int num_users = sizeof(users) / sizeof(User);

int authenticate(char *username, char *password) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(username, users[i].username) == 0 &&
            strcmp(password, users[i].password) == 0) {
            return 1;
        }
    }
    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        printf("Error al recibir datos del cliente\n");
        close(client_socket);
        return;
    }

    char *username = strtok(buffer, " ");
    char *password = strtok(NULL, " ");

    int authenticated = authenticate(username, password);
    char response[BUFFER_SIZE];
    sprintf(response, "%d", authenticated);

    if (strcmp(username, "exit") == 0) {
        printf("Servidor terminado por solicitud del cliente\n");
        kill(getppid(), SIGTERM);
    }

    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int option = 1;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Permite reutilizar la misma dirección y puerto
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        perror("Error en setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al vincular el socket");
        exit(EXIT_FAILURE);
    }

    listen(server_socket, MAX_CLIENTS);
    printf("Servidor iniciado y escuchando en el puerto 8080\n");

    while (1) {
        addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Error al aceptar la conexión del cliente");
            continue;
        }

        pid_t child_pid = fork();
        if (child_pid == 0) {
            close(server_socket);
            handle_client(client_socket);
            exit(EXIT_SUCCESS);
        } else if (child_pid < 0) {
            perror("Error al crear el proceso hijo");
            close(client_socket);
        } else {
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}