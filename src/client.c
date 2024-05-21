#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "rsa.h"

#define BUFFER_SIZE 2048

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char service[BUFFER_SIZE];
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char group_name[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int bytes_received;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar al servidor");
        exit(EXIT_FAILURE);
    }

    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    char *decrypted = decrypt( buffer);
    if (bytes_received <= 0) {
        perror("Error al recibir datos del servidor");
        exit(EXIT_FAILURE);
    }
    decrypted[bytes_received] = '\0';
    printf("%s", decrypted);

    fgets(service, sizeof(service), stdin);
    service[strcspn(service, "\n")] = 0;

    if (strcmp(service, "exit") == 0) {
        printf("Salida solicitada...\n");
        send(client_socket, "exit", strlen("exit"), 0);
        close(client_socket);
        exit(EXIT_SUCCESS);
    }

    printf("Ingrese el nombre de usuario: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    if (strcmp(service, "auth") == 0) {
        printf("Ingrese la contraseña: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        char combined_credentials[BUFFER_SIZE * 2];
        snprintf(combined_credentials, sizeof(combined_credentials), "autentificar\n%s\n%s", username, password);
        char* cyphered_credentials = encrypt(combined_credentials);
        printf("Ciphered credentials: %s\n", cyphered_credentials);
        send(client_socket, cyphered_credentials, strlen(cyphered_credentials), 0);
    } else if (strcmp(service, "register") == 0) {
        printf("Ingrese la contraseña: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        char combined_credentials[BUFFER_SIZE * 2];
        snprintf(combined_credentials, sizeof(combined_credentials), "register\n%s\n%s", username, password);
        send(client_socket, combined_credentials, strlen(combined_credentials), 0);
    }
    else if (strcmp(service, "creargrupo") == 0) {
        printf("Ingrese el nombre del grupo: ");
        fgets(group_name, sizeof(group_name), stdin);
        group_name[strcspn(group_name, "\n")] = 0;

        char create_group_request[BUFFER_SIZE * 2];
        snprintf(create_group_request, sizeof(create_group_request), "creargrupo\n%s\n%s", username, group_name);
        send(client_socket, create_group_request, strlen(create_group_request), 0);
    } else {
        printf("Servicio no válido\n");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s\n", buffer);

    close(client_socket);

    return 0;
}