#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

static char *host = "localhost";
static char *user = "root";
static char *pass = "";
static char *db   = "user_db";
static char *socket = NULL;
unsigned int port = 3306;
unsigned int flags = 0;

MYSQL *connect_to_mysql() {
    MYSQL *con = mysql_init(NULL);
    if (!mysql_real_connect(con, host, user, pass, db, port, socket, flags)) {
        fprintf(stderr, "Error %s (%d)", mysql_error(con), mysql_errno(con));
        exit(1);
    }
    printf("Connected\n");
    return con;
}

void create_database(MYSQL *con) {
    const char *create_db_query = "CREATE DATABASE IF NOT EXISTS user_db";
    if (mysql_query(con, create_db_query)) {
        fprintf(stderr, "Error creating database: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
}

void use_database(MYSQL *con) {
    if (mysql_query(con, "USE user_db")) {
        fprintf(stderr, "Error selecting database: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
}

void create_table(MYSQL *con) {
    const char *create_table_query = "CREATE TABLE IF NOT EXISTS users (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255))";
    if (mysql_query(con, create_table_query)) {
        fprintf(stderr, "Error creating table: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    printf("Users table created successfully!\n");
}

void insert_user(MYSQL *con, const char *username) {
    char query[100];
    sprintf(query, "INSERT INTO users (name) VALUES ('%s')", username);
    if (mysql_query(con, query)) {
        fprintf(stderr, "Error inserting user: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    printf("User '%s' inserted successfully!\n", username);
}

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

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
