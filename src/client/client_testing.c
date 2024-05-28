#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cjson/cJSON.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define ACTION_SIZE 256
#define MESSAGE_SIZE 65535

void send_json(int socket_fd, cJSON *json) {
    char *json_str = cJSON_PrintUnformatted(json);
    send(socket_fd, json_str, strlen(json_str), 0);
    free(json_str);
}

cJSON *receive_json(int socket_fd) {
    char buffer[ACTION_SIZE + MESSAGE_SIZE];
    ssize_t bytes_received = recv(socket_fd, buffer, ACTION_SIZE + MESSAGE_SIZE, 0);
    if (bytes_received <= 0) {
        fprintf(stderr, "Error receiving data from server\n");
        return NULL;
    }
    buffer[bytes_received] = '\0';
    return cJSON_Parse(buffer);
}

int main() {
    int socket_fd;
    struct sockaddr_in server_addr;

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to server
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Register
    cJSON *register_json = cJSON_CreateObject();
    cJSON_AddStringToObject(register_json, "action", "register");
    cJSON *register_data = cJSON_CreateObject();
    cJSON_AddStringToObject(register_data, "username", "patomimo");
    cJSON_AddStringToObject(register_data, "password", "password1");
    cJSON_AddItemToObject(register_json, "data", register_data);
    send_json(socket_fd, register_json);
    printf("Register Sent: %s\n", cJSON_Print(register_json));
    cJSON_Delete(register_json);

    // Wait for register response
    cJSON *register_response = receive_json(socket_fd);
    if (!register_response) {
        exit(EXIT_FAILURE);
    }
    printf("Register response: %s\n", cJSON_Print(register_response));
    cJSON_Delete(register_response);

    char *user_id;
    // Login
    cJSON *login_json = cJSON_CreateObject();
    cJSON_AddStringToObject(login_json, "action", "login");
    cJSON *login_data = cJSON_CreateObject();
    cJSON_AddStringToObject(login_data, "username", "patomimo");
    cJSON_AddStringToObject(login_data, "password", "password1");
    cJSON_AddItemToObject(login_json, "data", login_data);
    send_json(socket_fd, login_json);
    printf("LogIn Sent: %s\n", cJSON_Print(login_json));
    cJSON_Delete(login_json);

    // Wait for login response
    cJSON *login_response = receive_json(socket_fd);
    if (!login_response) {
        exit(EXIT_FAILURE);
    }
    cJSON *login_data_response = cJSON_GetObjectItemCaseSensitive(login_response, "data");
    cJSON *login_status = cJSON_GetObjectItemCaseSensitive(login_data_response, "status");
    if (strcmp(login_status->valuestring, "Success") == 0) {
        cJSON *login_return = cJSON_GetObjectItemCaseSensitive(login_data_response, "return");
        asprintf(&user_id, "%s", login_return->valuestring);
        printf("User ID: %s\n", login_return->valuestring);
    }
    printf("Login response: %s\n", cJSON_Print(login_response));
    cJSON_Delete(login_response);

    // Create chatroom
    cJSON *create_chatroom_json = cJSON_CreateObject();
    cJSON_AddStringToObject(create_chatroom_json, "action", "createcroom");
    cJSON *create_chatroom_data = cJSON_CreateObject();
    cJSON_AddStringToObject(create_chatroom_data, "name", "chatroom12345");
    cJSON_AddStringToObject(create_chatroom_data, "administrator_id", user_id); // Assuming user ID is 1
    cJSON_AddItemToObject(create_chatroom_json, "data", create_chatroom_data);
    send_json(socket_fd, create_chatroom_json);
    printf("Chatroom Creation Sent: %s\n", cJSON_Print(create_chatroom_json));
    cJSON_Delete(create_chatroom_json);
    // Wait for chatroom creation response
    cJSON *chatroom_response = receive_json(socket_fd);
    if (!chatroom_response) {
        exit(EXIT_FAILURE);
    }
    char *chatroom_id;
    cJSON *chatroom_data_response = cJSON_GetObjectItemCaseSensitive(chatroom_response, "data");
    cJSON *chatroom_status = cJSON_GetObjectItemCaseSensitive(chatroom_data_response, "status");
    if (strcmp(chatroom_status->valuestring, "Success") == 0) {
        cJSON *chatroom_return = cJSON_GetObjectItemCaseSensitive(chatroom_data_response, "return");
        asprintf(&chatroom_id, "%s", chatroom_return->valuestring);
        printf("Chatroom ID: %s\n", chatroom_return->valuestring);
    }
    printf("Chatroom creation response: %s\n", cJSON_Print(chatroom_response));
    cJSON_Delete(chatroom_response);

    // Send message to chatroom
    cJSON *send_message_json = cJSON_CreateObject();
    cJSON_AddStringToObject(send_message_json, "action", "msg");
    cJSON *send_message_data = cJSON_CreateObject();
    cJSON_AddStringToObject(send_message_data, "msg", "Hello, chatroom!");
    cJSON_AddStringToObject(send_message_data, "user_id", user_id);
    cJSON_AddStringToObject(send_message_data, "channel_id", chatroom_id);
    cJSON_AddItemToObject(send_message_json, "data", send_message_data);
    send_json(socket_fd, send_message_json);
    printf("Exit Sent: %s\n", cJSON_Print(send_message_json));
    cJSON_Delete(send_message_json);

    // Wait for message response
    cJSON *message_response = receive_json(socket_fd);
    if (!message_response) {
        exit(EXIT_FAILURE);
    }
    printf("Message response: %s\n", cJSON_Print(message_response));
    cJSON_Delete(message_response);

    // Logout
    cJSON *logout_json = cJSON_CreateObject();
    cJSON_AddStringToObject(logout_json, "action", "exit");
    send_json(socket_fd, logout_json);
    cJSON_Delete(logout_json);

    // Close socket
    close(socket_fd);

    return 0;
}
