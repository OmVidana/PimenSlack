#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "mysql_functions.h"
#include "mysdl_functions.h"
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar con el servidor");
        exit(EXIT_FAILURE);
    }

    printf("Ingrese su nombre de usuario (o 'exit' para salir): ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if (strcmp(buffer, "exit") == 0) {
        char message[] = "exit";
        send(client_socket, message, strlen(message), 0);
        printf("Solicitud de cierre de servidor enviada\n");
        close(client_socket);
        return 0;
    }

    printf("Ingrese su contraseña: ");
    char password[BUFFER_SIZE];
    fgets(password, BUFFER_SIZE, stdin);
    password[strcspn(password, "\n")] = '\0';

    char message[BUFFER_SIZE];
    sprintf(message, "%s %s", buffer, password);

    send(client_socket, message, strlen(message), 0);

    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        perror("Error al recibir datos del servidor");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    int authenticated = atoi(buffer);

    if (authenticated) {
        printf("Autenticación exitosa\n");
    } else {
        printf("Autenticación fallida\n");
    }

    close(client_socket);
    return 0;
}
//    init_mysdl();
//    main_loop();

//    MYSQL *con = connect_to_mysql();
//    create_database(con);
//    use_database(con);
//    create_table(con);
//
//    insert_user(con, "Alice");
//    insert_user(con, "Bob");
//    insert_user(con, "Charlie");
//
//    find_user(con, "Bob");
//    find_user(con, "David");
//
//    mysql_close(con);
//    return EXIT_SUCCESS;
