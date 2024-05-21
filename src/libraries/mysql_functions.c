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

void insert_row(MYSQL *con, const char *table_name, const char *data[][2]) {
    char *insert;
    char *names = "";
    char *values = "";
    asprintf(&insert, "INSERT INTO %s", table_name);
    for (int i = 0; data[i][0] != NULL; ++i) {
        if (data[i + 1][0] != NULL) {
            asprintf(&names, "%s%s, ", names, data[i][0]);
            asprintf(&values, "%s'%s', ", values, data[i][1]);
        }
        else {
            asprintf(&names, "%s%s", names, data[i][0]);
            asprintf(&values, "%s'%s'", values, data[i][1]);
        }
    }
    printf("%s\n", names);
    printf("%s\n", values);
    asprintf(&insert, "%s (%s) VALUES (%s);", insert,names, values);
    printf("%s\n", insert);

    if (mysql_query(con, insert)) {
        fprintf(stderr, "Error inserting user: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    printf("Data inserted in %s table, successfully!\n", table_name);
}

bool find_user(MYSQL *con, const char *username, const char *password) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[200]; // Ajustamos el tamaño del buffer
    sprintf(query, "SELECT * FROM users WHERE name='%s' AND password='%s'", username, password);
    if (mysql_query(con, query)) {
        fprintf(stderr, "Error finding user: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    result = mysql_store_result(con);
    if (result == NULL) {
        fprintf(stderr, "No result returned\n");
        mysql_close(con);
        exit(1);
    }
    row = mysql_fetch_row(result);
    if (row == NULL) {
        return false;
    } else {
        return true;

    }
}
