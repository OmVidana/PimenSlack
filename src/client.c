#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mysdl_functions.h"

#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int bytes_received;

//    init_mysdl();
//    main_loop();

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar al servidor");
        exit(EXIT_FAILURE);
    }

    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        perror("Error al recibir datos del servidor");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("%s", buffer);

    printf("Ingrese nombre de usuario: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    if (strcmp(username, "exit") == 0) {
        printf("Salida solicitada...\n");
        send(client_socket, "exit", strlen("exit"), 0);
        close(client_socket);
        exit(EXIT_SUCCESS);
    }

    printf("Ingrese contraseña: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    char combined_credentials[BUFFER_SIZE * 2];
    snprintf(combined_credentials, sizeof(combined_credentials), "%s:%s", username, password);

    send(client_socket, combined_credentials, strlen(combined_credentials), 0);
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);
    if (strcmp(buffer, "1") == 0) {
        printf( "Usuario autenticado exitosamente\n");
    } else {
        printf( "Usuario o contraseña incorrectos\n");
    }

    close(client_socket);

    return 0;
}
