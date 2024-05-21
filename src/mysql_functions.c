#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

static char *host = "localhost";
static char *user = "root";
static char *pass = "";
static char *db = "pimenslack_db";
static char *socket = NULL;
unsigned int port = 3306;
unsigned int flags = 0;

MYSQL *connect_and_create_database() {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, host, user, pass, NULL, port, socket, flags) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(con));
        mysql_close(con);
        exit(EXIT_FAILURE);
    }

    char *create_db_query;
    asprintf(&create_db_query, "CREATE DATABASE IF NOT EXISTS %s", db);
    if (mysql_query(con, create_db_query)) {
        fprintf(stderr, "Error creating database: %s\n", mysql_error(con));
        mysql_close(con);
        exit(EXIT_FAILURE);
    }

    if (mysql_select_db(con, db)) {
        fprintf(stderr, "Error selecting database: %s\n", mysql_error(con));
        mysql_close(con);
        exit(EXIT_FAILURE);
    }

    printf("Connected to database %s\n", db);
    return con;
}

void create_table(MYSQL *con, const char *table_name, const char *columns[]) {
    char *create_table_query;
    asprintf(&create_table_query, "CREATE TABLE IF NOT EXISTS %s (", table_name);

    for (int i = 0; columns[i] != NULL; ++i) {
        if (columns[i + 1] != NULL)
            asprintf(&create_table_query, "%s%s, ", create_table_query, columns[i]);
        else
            asprintf(&create_table_query, "%s%s);", create_table_query, columns[i]);
    }

    printf("%s\n", create_table_query);

    if (mysql_query(con, create_table_query)) {
        fprintf(stderr, "Error creating table: %s\n", mysql_error(con));
        mysql_close(con);
        free(create_table_query);
        exit(1);
    }

    free(create_table_query);
    printf("%s table created successfully!\n", table_name);
}

void create_all_tables(MYSQL *con) {
    // Creación de Tabla Usuarios, Chatrooms y Mensajes
    create_table(con, "users", (const char *[]) {"ID SMALLINT UNSIGNED AUTO_INCREMENT PRIMARY KEY",
                                                 "name VARCHAR(16)",
                                                 "password VARCHAR(16)",
                                                 "public_encryption_key BIGINT UNSIGNED",
                                                 "private_encryption_key BIGINT UNSIGNED",
                                                 "status BOOLEAN",
                                                 NULL});
    create_table(con, "channels", (const char *[]) {"ID MEDIUMINT UNSIGNED AUTO_INCREMENT PRIMARY KEY",
                                                    "name VARCHAR(32)",
                                                    "administrator_id SMALLINT UNSIGNED",
                                                    "creation_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP",
                                                    "FOREIGN KEY (administrator_id) REFERENCES users(ID)",
                                                    NULL});
    create_table(con, "messages", (const char *[]) {"ID INT UNSIGNED AUTO_INCREMENT PRIMARY KEY",
                                                    "msg TEXT",
                                                    "sent_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP",
                                                    "user_id SMALLINT UNSIGNED",
                                                    "channel_id MEDIUMINT UNSIGNED",
                                                    "FOREIGN KEY (user_id) REFERENCES users(ID)",
                                                    "FOREIGN KEY (channel_id) REFERENCES channels(ID)",
                                                    NULL});
    // Tablas de Relación Canales_Usuarios y Usuarios-Mensajes
    create_table(con, "channels_users", (const char *[]) {"ID INT UNSIGNED AUTO_INCREMENT PRIMARY KEY",
                                                          "channel_id MEDIUMINT UNSIGNED",
                                                          "user_id SMALLINT UNSIGNED",
                                                          "FOREIGN KEY (channel_id) REFERENCES channels(ID)",
                                                          "FOREIGN KEY (user_id) REFERENCES users(ID)",
                                                          NULL});
    create_table(con, "user_messages", (const char *[]) {"ID INT UNSIGNED AUTO_INCREMENT PRIMARY KEY",
                                                         "user_id SMALLINT UNSIGNED",
                                                         "message_id INT UNSIGNED",
                                                         "FOREIGN KEY (user_id) REFERENCES users(ID)",
                                                         "FOREIGN KEY (message_id) REFERENCES messages(ID)",
                                                         NULL});
    create_table(con, "channel_messages", (const char *[]) {"ID INT UNSIGNED AUTO_INCREMENT PRIMARY KEY",
                                                            "channel_id MEDIUMINT UNSIGNED",
                                                            "message_id INT UNSIGNED",
                                                            "FOREIGN KEY (channel_id) REFERENCES channels(ID)",
                                                            "FOREIGN KEY (message_id) REFERENCES messages(ID)",
                                                            NULL});
}

int insert_row(MYSQL *con, const char *table_name, const char *data[][2]) {
    char *insert;
    char *names = "";
    char *values = "";
    asprintf(&insert, "INSERT INTO %s", table_name);

    for (int i = 0; data[i][0] != NULL; ++i) {
        printf("data: %s, %s\n", data[i][0], data[i][1]);
        if (data[i + 1][0] != NULL) {
            asprintf(&names, "%s%s, ", names, data[i][0]);
            asprintf(&values, "%s'%s', ", values, data[i][1]);
        } else {
            asprintf(&names, "%s%s", names, data[i][0]);
            asprintf(&values, "%s'%s'", values, data[i][1]);
        }
    }

    asprintf(&insert, "%s (%s) VALUES (%s);", insert,names, values);
    printf("%s\n", insert);

    if (mysql_query(con, insert)) {
        fprintf(stderr, "Error inserting user: %s\n", mysql_error(con));
        free(insert);
        free(names);
        free(values);
        return -1;
    }
    printf("Data inserted in %s table, successfully!\n", table_name);
    free(insert);
    free(names);
    free(values);
    return 0;
}

uint8_t find_user(MYSQL *con, const char *username, const char *password) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *query;
    uint8_t user_id = 0;

    asprintf(&query, "SELECT ID FROM users WHERE name='%s' AND password='%s';", username, password);
    if (mysql_query(con, query)) {
        fprintf(stderr, "Error finding user: %s\n", mysql_error(con));
        free(query);
        return user_id;
    }

    free(query);

    result = mysql_store_result(con);
    if (result == NULL) {
        fprintf(stderr, "No result returned\n");
        return user_id;
    }

    row = mysql_fetch_row(result);
    if (row != NULL) {
        user_id = (uint8_t) atoi(row[0]);
    }

    mysql_free_result(result);

    return user_id;
}