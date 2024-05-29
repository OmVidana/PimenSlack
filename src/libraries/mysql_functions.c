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

typedef struct {
    char ***rows;
    unsigned int num_rows;
    unsigned int num_fields;
    char **field_names;
} QueryResult;


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
    printf("%s\n", insert);
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

    asprintf(&insert, "%s (%s) VALUES (%s);", insert, names, values);
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

QueryResult *fetch_records(MYSQL *con, const char *table_name, const char *conditions) {
    char query[512];
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    unsigned int num_fields;
    unsigned int num_rows = 0;
    unsigned int i, j;

    if (conditions && strlen(conditions) > 0) {
        snprintf(query, sizeof(query), "SELECT * FROM %s WHERE %s", table_name, conditions);
    } else {
        snprintf(query, sizeof(query), "SELECT * FROM %s", table_name);
    }

    if (mysql_query(con, query)) {
        fprintf(stderr, "Error retrieving data: %s\n", mysql_error(con));
        return NULL;
    }

    result = mysql_store_result(con);
    if (result == NULL) {
        fprintf(stderr, "Error storing result: %s\n", mysql_error(con));
        return NULL;
    }

    num_fields = mysql_num_fields(result);
    num_rows = mysql_num_rows(result);

    QueryResult *query_result = malloc(sizeof(QueryResult));
    query_result->num_fields = num_fields;
    query_result->num_rows = num_rows;
    query_result->rows = malloc(num_rows * sizeof(char **));
    query_result->field_names = malloc(num_fields * sizeof(char *));

    // Store field names
    for (i = 0; (field = mysql_fetch_field(result)); i++) {
        query_result->field_names[i] = strdup(field->name);
    }

    for (i = 0; (row = mysql_fetch_row(result)); i++) {
        query_result->rows[i] = malloc(num_fields * sizeof(char *));
        for (j = 0; j < num_fields; j++) {
            query_result->rows[i][j] = row[j] ? strdup(row[j]) : NULL;
        }
    }

    mysql_free_result(result);
    return query_result;
}

void free_query_result(QueryResult *query_result) {
    if (query_result) {
        for (unsigned int i = 0; i < query_result->num_rows; i++) {
            for (unsigned int j = 0; j < query_result->num_fields; j++) {
                free(query_result->rows[i][j]);
            }
            free(query_result->rows[i]);
        }
        free(query_result->rows);
        free(query_result);
    }
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
        printf("Successfully Logged In.\n");
    }

    mysql_free_result(result);
    return user_id;
}

uint8_t find_chatroom(MYSQL *con, const char *name, const char *admin_id) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *query;
    uint8_t channel_id = 0;

    asprintf(&query, "SELECT ID FROM channels WHERE name='%s' AND administrator_id='%s';", name, admin_id);
    if (mysql_query(con, query)) {
        fprintf(stderr, "Error finding Table: %s\n", mysql_error(con));
        free(query);
        return channel_id;
    }

    free(query);

    result = mysql_store_result(con);
    if (result == NULL) {
        fprintf(stderr, "No result returned\n");
        return channel_id;
    }

    row = mysql_fetch_row(result);
    if (row != NULL) {
        channel_id = (uint8_t) atoi(row[0]);
        printf("Successfully Logged In.\n");
    }

    mysql_free_result(result);
    return channel_id;
}

QueryResult *fetch_chatrooms(MYSQL *con, const char *conditions) {
    if (conditions && strlen(conditions) > 0) {
        return fetch_records(con, "channels", conditions);
    } else {
        return fetch_records(con, "channels", NULL);
    }
}

QueryResult *fetch_messages(MYSQL *con, const char *conditions) {
    if (conditions && strlen(conditions) > 0) {
        return fetch_records(con, "messages", conditions);
    } else {
        return fetch_records(con, "messages", NULL);
    }
}
