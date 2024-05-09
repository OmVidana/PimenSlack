// Servidor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "src/mysql_functions.h"
#include <mysql/mysql.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

/**
 * @brief Handle client connection and authentication
 *
 * @param client_socket Client socket descriptor
 * @param con MySQL connection handle
 */
void handle_client(int client_socket, MYSQL *con) {
    char combined_credentials[BUFFER_SIZE];
    char username[50];
    char password[50];
    int bytes_received;

    send(client_socket, "Credentials", strlen("Credentials"), 0);
    bytes_received = recv(client_socket, combined_credentials, sizeof(combined_credentials), 0);
    if (bytes_received <= 0) {
        fprintf(stderr, "Error receiving data from client\n");
        close(client_socket);
        return;
    }
    combined_credentials[bytes_received] = '\0';

    printf("Credentials received: %s\n", combined_credentials);

    if (strcmp(combined_credentials, "exit") == 0) {
        printf("Client requested exit. Closing connection...\n");
        close(client_socket);
        kill(getpid(), SIGTERM);
        return;
    }

    char *token = strtok(combined_credentials, ":");
    if (token != NULL) {
        strcpy(username, token);
        printf("Username: %s\n", username);
        token = strtok(NULL, ":");
        if (token != NULL) {
            strcpy(password, token);
            printf("Password: %s\n", password);
        } else {
            fprintf(stderr, "Error: Password not found\n");
            close(client_socket);
            return;
        }
    } else {
        fprintf(stderr, "Error: Username not found\n");
        close(client_socket);
        return;
    }

    if (find_user(con, username, password)) {
        send(client_socket, "1", strlen("0"), 0);
    } else {
        send(client_socket, "0", strlen("0"), 0);
    }

    close(client_socket);
}

/**
 * @brief Kill all child processes and exit
 */
void kill_children( ) {
    kill(0, SIGTERM);
    exit(EXIT_SUCCESS);
}

/**
 * @brief Server main function
 *
 * @return int Exit status
 */
int main() {
    MYSQL *con = connect_to_mysql();
    create_database(con);
    use_database(con);
    create_table(con);

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int option = 1;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        perror("Error in setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    listen(server_socket, MAX_CLIENTS);
    printf("Server started and listening on port 8080\n");

    signal(SIGTERM, kill_children);

    while (1) {
        addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Error accepting client connection");
            continue;
        }

        pid_t child_pid = fork();
        if (child_pid == 0) {
            close(server_socket);
            handle_client(client_socket, con);
            exit(EXIT_SUCCESS);
        } else if (child_pid < 0) {
            perror("Error creating child process");
            close(client_socket);
        } else {
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}