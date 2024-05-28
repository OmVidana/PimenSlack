#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void *send_message(void *sockfd_ptr) {
    int sockfd = *(int *)sockfd_ptr;
    char buffer[256];
    char pid_str[10];
    char message[256];

    while (1) {
        printf("Enter PID: ");
        fgets(pid_str, sizeof(pid_str), stdin);

        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);

        snprintf(buffer, sizeof(buffer), "PID: %sMessage: %s", pid_str, message);

        int n = send(sockfd, buffer, strlen(buffer), 0);
        if (n < 0) {
            error("ERROR sending to socket");
        }
    }
    return NULL;
}

int main() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    pthread_t thread_id;

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created with fd: %d\n", sockfd);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    // Obtener la dirección del servidor
    server = gethostbyname(SERVER_IP);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    // Configurar la estructura del servidor
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    // Conectarse al servidor
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }

    printf("Connected to the server.\n");

    // Crear el hilo para enviar mensajes
    if (pthread_create(&thread_id, NULL, send_message, (void *)&sockfd) < 0) {
        error("ERROR creating thread");
    }

    // Leer mensajes del servidor
    while (1) {
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            error("ERROR reading from socket");
        } else if (n == 0) {
            printf("Server closed the connection\n");
            break;
        }
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    pthread_cancel(thread_id);
    pthread_join(thread_id, NULL);
    return 0;
}
