/* TO DO:
 * Avoid duplicate entries Database
 * Limit amount entries Database
 * Remove Row
 * Add to Chatroom
 * Get Chatroom's I'm in and their respective Data
 * Apply Search Conditions
 * Fill Relation Tables
*/

#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "rsa.h"
#include "mysql_functions.h"
#include <mysql/mysql.h>
#include <cjson/cJSON.h>

#define MAX_CLIENTS 16
#define BUFFER_SIZE 16384
#define ACTION_SIZE 256
#define MESSAGE_SIZE 65535
#define SYNC_DATA "sync"
#define LOGIN "login"
#define REGISTER "register"
#define CREATE_CHATROOM "createcroom"
#define REMVF_CHATROOM "cremove"
#define LEAVE_CHATROOM "cleave"
#define SEND_MESSAGE "msg"
#define LOGOUT "exit"
#define AUTHENTICATED "auth"
#define ERROR_ACTION "error"

int register_user(MYSQL *con, const char *name, const char *password, const char *public_key, const char *private_key,
                  const char *status) {
    return insert_row(con, "users", (const char *[][2]) {{"name",                   name},
                                                         {"password",               password},
                                                         {"public_encryption_key",  public_key},
                                                         {"private_encryption_key", private_key},
                                                         {"status",                 status},
                                                         {NULL, NULL}});
}

uint8_t login(MYSQL *con, const char *username, const char *password) {
    return find_user(con, username, password);
}

int create_chatroom(MYSQL *con, const char *chatroom_name, int user_id) {
    char *admin_id;
    asprintf(&admin_id, "%d", user_id);
    insert_row(con, "channels", (const char *[][2]) {{"name",             chatroom_name},
                                                     {"administrator_id", admin_id},
                                                     {NULL, NULL}});
    return find_chatroom(con, chatroom_name, admin_id);
}

int create_message(MYSQL *con, const char *message, int user_id, int channel_id) {
    char *sender_id;
    asprintf(&sender_id, "%d", user_id);
    char *sender_channel_id;
    asprintf(&sender_channel_id, "%d", channel_id);
    return insert_row(con, "messages", (const char *[][2]) {{"msg",        message},
                                                            {"user_id",    sender_id},
                                                            {"channel_id", sender_channel_id}});

}



//TODO
/*
 * Group message
 * Leave chatroom -- If administrator leaves, next user in chatroom will be the new administrator
 * Add user to chatroom
 * Request to join chatroom
 * Remove user from chatroom -- Include users must be administrator
 * Logout
 *
 */

/**
 * @brief Handle client connection and authentication
 *
 * @param client_socket Client socket descriptor
 * @param con MySQL connection handle
 */
void handle_client(int client_socket, MYSQL *con) {
    char combined_credentials[BUFFER_SIZE];
    char service[50];
    char username[50];
    char password[50];
    char groupname[50];
    int bytes_received;

    char message[] = "Auth/CreateGroup/Exit:";
    char *ciphertext = encrypt(message);
    printf("Ciphertext: %s\n", ciphertext);
    send(client_socket, ciphertext, strlen(ciphertext), 0);
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
    char *decoded_credentials = decrypt(combined_credentials);

    printf("Decoded credentials: %s\n", decoded_credentials);

    char *token = strtok(decoded_credentials, "\n");
    if (token != NULL) {
        strcpy(service, token);
    }

    if (strcmp(service, "autentificar") == 0) {
        printf("Handling autentificar\n");
        token = strtok(NULL, "\n");
        if (token != NULL) {
            strcpy(username, token);
            token = strtok(NULL, "\n");
            if (token != NULL) {
                strcpy(password, token);
                if (find_user(con, username, password) != 0) {
                    char *token_for_user = encrypt(username);
                    send(client_socket, token_for_user, strlen(token_for_user), 0);

                } else {
                    send(client_socket, "0", strlen("0"), 0);
                }
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


    } else if (strcmp(service, "register") == 0) {
        printf("Registering user\n");
        token = strtok(NULL, "\n");
        if (token != NULL) {
            strcpy(username, token);
            token = strtok(NULL, "\n");
            if (token != NULL) {
                strcpy(password, token);
            }
        }

        if (register_user(con, username, password, "0", "0", "0") != -1) {
            send(client_socket, "1", strlen("1"), 0);
        } else {
            send(client_socket, "0", strlen("0"), 0);
        }

    } else if (strcmp(service, "creargrupo") == 0) {
        printf("Creando grupo\n");
        token = strtok(NULL, "\n");
        if (token != NULL) {
            strcpy(username, token);
            token = strtok(NULL, "\n");
            if (token != NULL) {
                strcpy(groupname, token);
            }
        }
        const char *group_name = groupname;
        const char *creator = username;
        char *chatroom_token;
        if (create_chatroom(con, group_name, 1) != -1) {
            asprintf(&chatroom_token, "<%s><%s><%s>", creator, group_name, "true");
            send(client_socket, chatroom_token, strlen(chatroom_token), 0);
        } else {
            asprintf(&chatroom_token, "<%s><%s><%s>", creator, group_name, "false");
            send(client_socket, "0", strlen(""), 0);
        }
    } else {
        printf("Unknown service: %s\n", service);
    }
    close(client_socket);
}

void action_selector(MYSQL *con, int client_socket, const char *action, cJSON *data, bool *connected, bool *authenticated,
                uint8_t *user_id) {
    cJSON *response = cJSON_CreateObject();
    cJSON *response_data = cJSON_CreateObject();
    cJSON_AddItemToObject(response, "data", response_data);

    if (strcmp(action, SYNC_DATA) == 0 && *authenticated) {
        printf("Syncing data\n");

        QueryResult *chatrooms = fetch_chatrooms(con, NULL);

        cJSON *sync_json = cJSON_CreateObject();
        cJSON *sync_data = cJSON_CreateObject();
        cJSON *chatrooms_json = cJSON_CreateObject();

        for (unsigned int i = 0; i < chatrooms->num_rows; i++) {
            cJSON *chatroom = cJSON_CreateObject();
            const char *id = chatrooms->rows[i][0];

            for (unsigned int j = 1; j < chatrooms->num_fields; j++) {
                const char *field_name = chatrooms->field_names[j];
                const char *value = chatrooms->rows[i][j];
                cJSON_AddStringToObject(chatroom, field_name, value ? value : "NULL");
            }

            cJSON_AddItemToObject(chatrooms_json, id, chatroom);
        }
        free_query_result(chatrooms);

        cJSON_AddItemToObject(sync_data, "chatrooms", chatrooms_json);
        cJSON_AddStringToObject(sync_json, "action", "sync");
        cJSON_AddItemToObject(sync_json, "data", data);

        char *json_str = cJSON_PrintUnformatted(sync_json);

        send(client_socket, json_str, strlen(json_str), 0);
        free(json_str);
        cJSON_Delete(sync_json);
        // Enlistar todos los chatroom --> donde esta el user_id
        // Enlistar todos los mensajes de estos chatroom

    } else if (strcmp(action, LOGIN) == 0 && !*authenticated) {
        //Recibir las keys de encrypt
        *user_id = login(con, cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring,
                         cJSON_GetObjectItemCaseSensitive(data, "password")->valuestring);
        if (*user_id == 0) {
            printf("Invalid username or password\n");
            cJSON_AddStringToObject(response, "action", ERROR_ACTION);
            cJSON_AddStringToObject(response_data, "status", "Failure");
            cJSON_AddStringToObject(response_data, "return", "Invalid username or password.");
        } else {
            printf("Successfully logged in with user ID: %d\n", *user_id);
            cJSON_AddStringToObject(response, "action", AUTHENTICATED);
            cJSON_AddStringToObject(response_data, "status", "Success");
            char *user_id_str;
            asprintf(&user_id_str, "%d", *user_id);

            FILE *f = fopen("public.txt", "r");
            if (f == NULL) {
                perror("Failed to open file");
            }

            char *public_key = malloc(1024 * sizeof(char));
            if (public_key == NULL) {
                perror("Failed to allocate memory");
                fclose(f);
            }

            if (fgets(public_key, 1024, f) != NULL) {
                size_t len = strlen(public_key);
                if (len > 0 && public_key[len - 1] == '\n') {
                    public_key[len - 1] = '\0';
                }
                printf("Public Key: %s\n", public_key);
            } else {
                perror("Failed to read from file");
            }

            fclose(f);


            cJSON_AddStringToObject(response_data, "user_id", user_id_str);
            cJSON_AddStringToObject(response_data, "public_key", public_key);
            *authenticated = true;
            free(public_key);
            // Set Online DB
        }
    } else if (strcmp(action, REGISTER) == 0 && !*authenticated) {
        const char *username = cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring;
        const char *password = cJSON_GetObjectItemCaseSensitive(data, "password")->valuestring;
        if (register_user(con, username, password, "7", "13", "0") == -1) {
            cJSON_AddStringToObject(response, "action", ERROR_ACTION);
            cJSON_AddStringToObject(response_data, "status", "failure");
            cJSON_AddStringToObject(response_data, "return", "User registration failed");
        } else {
            printf("Successfully registered in.\n");
            cJSON_AddStringToObject(response, "action", REGISTER);
            cJSON_AddStringToObject(response_data, "status", "Success");
            cJSON_AddStringToObject(response_data, "return", "(Encryption Key) User registered successfully");
        }
    } else if (strcmp(action, CREATE_CHATROOM) == 0 && *authenticated) {
        char *chatroom_name = cJSON_GetObjectItemCaseSensitive(data, "name")->valuestring;
        chatroom_name = decrypt(chatroom_name);
        char *administrator_id = cJSON_GetObjectItemCaseSensitive(data, "administrator_id")->valuestring;
        administrator_id = decrypt(administrator_id);
        int chatroom_id;
        chatroom_id = create_chatroom(con, chatroom_name, atoi(administrator_id));
        if (chatroom_id == -1) {
            printf("Error Chat\n");
            cJSON_AddStringToObject(response, "action", ERROR_ACTION);
            cJSON_AddStringToObject(response_data, "status", "Failure");
            cJSON_AddStringToObject(response_data, "return", "Chatroom creation failed");
        } else {
            printf("Success Chat\n");
            char *chatroom_id_str;
            asprintf(&chatroom_id_str, "%d", chatroom_id);
            cJSON_AddStringToObject(response, "action", CREATE_CHATROOM);
            cJSON_AddStringToObject(response_data, "status", "Success");
            cJSON_AddStringToObject(response_data, "return", chatroom_id_str);
        }
    } else if (strcmp(action, SEND_MESSAGE) == 0 && *authenticated) {
        const char *message = cJSON_GetObjectItemCaseSensitive(data, "msg")->valuestring;
        const char *json_user_id = cJSON_GetObjectItemCaseSensitive(data, "user_id")->valuestring;
        printf("%s\n", message);
        if (*user_id != (uint8_t) atoi(json_user_id)) return;
        printf("Pasó\n");
        const char *channel_id = cJSON_GetObjectItemCaseSensitive(data, "channel_id")->valuestring;
        if (create_message(con, message, atoi(json_user_id), atoi(channel_id)) == -1) {
            printf("Message Not Sent\n");
            cJSON_AddStringToObject(response, "action", ERROR_ACTION);
            cJSON_AddStringToObject(response_data, "status", "Failure");
            cJSON_AddStringToObject(response_data, "return", "Message creation failed");
        } else {
            printf("Message Sent\n");
            cJSON_AddStringToObject(response, "action", SEND_MESSAGE);
            cJSON_AddStringToObject(response_data, "status", "Success");
            cJSON_AddStringToObject(response_data, "return", "Message sent successfully");
        }
    }
        //TODO
        /*
         * Group message
         * Leave chatroom
         * Add user to chatroom
         * Request to join chatroom
         * Remove user from chatroom -- Include users must be administrator
         * Logout
         *
         */

    else if (strcmp(action, REMVF_CHATROOM) == 0 && *authenticated) {

    } else if (strcmp(action, LEAVE_CHATROOM) == 0 && *authenticated) {

    } else if (strcmp(action, LOGOUT) == 0 && *authenticated) {
        printf("Exiting...\n");
        *authenticated = false;
        *connected = false;
        // Set Offline DB
        cJSON_AddStringToObject(response, "action", LOGOUT);
        cJSON_AddStringToObject(response_data, "status", "success");
        cJSON_AddStringToObject(response_data, "return", "Log In Off...");
    }
    char *response_str = cJSON_PrintUnformatted(response);
    send(client_socket, response_str, strlen(response_str), 0);
    cJSON_Delete(response);
    free(response_str);
}

void client_loop(MYSQL *con, int client_socket) {
    cJSON *actionJSON;
    char buffer[ACTION_SIZE + MESSAGE_SIZE];
    bool connected = true;
    bool authenticated = false;
    uint8_t user_id;
    while (connected) {
        memset(buffer, 0, ACTION_SIZE + MESSAGE_SIZE);
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Error receiving data from client");
            memset(buffer, 0, ACTION_SIZE + MESSAGE_SIZE);
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Receive Action:\n\t%s\n", buffer);
        actionJSON = cJSON_Parse(buffer);
        if (!actionJSON) {
            printf("Error parsing JSON Action\n");
            memset(buffer, 0, ACTION_SIZE + MESSAGE_SIZE);
            connected = false;
        }

        action_selector(con, client_socket, cJSON_GetObjectItemCaseSensitive(actionJSON, "action")->valuestring,
                        cJSON_GetObjectItemCaseSensitive(actionJSON, "data"), &connected, &authenticated, &user_id);
    }

    close(client_socket);
}

/**
 * @brief Kill all child processes and exit
 */
void kill_children() {
    kill(0, SIGTERM);
    exit(EXIT_SUCCESS);
}

/**
 * @brief Server main function
 *
 * @return int Exit status
 */
int main() {
    MYSQL *con = connect_and_create_database();
    create_all_tables(con);
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
            client_loop(con, client_socket);
            //handle_client(client_socket, con);
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