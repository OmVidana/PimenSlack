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

void create_table1(MYSQL *con, const char *table_name, const char *fields) {
    char create_table_query[1024];
    sprintf(create_table_query, "CREATE TABLE IF NOT EXISTS %s (%s)", table_name, fields);

    if (mysql_query(con, create_table_query)) {
        fprintf(stderr, "Error creating table: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    printf("Table '%s' created successfully!\n", table_name);
}


#include <string.h>

bool find_user(MYSQL *con, const char *username, const char *password) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[200];
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

int insert_user(MYSQL *con, const char *username, const char *password) {
    const char *query = "INSERT INTO users (name, password) VALUES (?, ?)";
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[2];

    stmt = mysql_stmt_init(con);
    if (!stmt) {
        fprintf(stderr, "mysql_stmt_init() failed\n");
        mysql_close(con);
        exit(1);
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        fprintf(stderr, "mysql_stmt_prepare() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(con);
        exit(1);
    }

    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)username;
    bind[0].buffer_length = strlen(username);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)password;
    bind[1].buffer_length = strlen(password);

    if (mysql_stmt_bind_param(stmt, bind)) {
        fprintf(stderr, "mysql_stmt_bind_param() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(con);
        exit(1);
    }

    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "mysql_stmt_execute() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(con);
        exit(1);
    }

    int user_id = (int)mysql_insert_id(con);

    if (!find_user(con, username, password)) {
       fprintf(stderr, "User '%s' not found!\n", username);
       return -1;
    }

    printf("User '%s' inserted successfully with ID %d!\n", username, user_id);

    mysql_stmt_close(stmt);
    return user_id;
}

bool find_chatroom(MYSQL *con, const char *group_name) {
    char query[1024];
    sprintf(query, "SELECT * FROM chatrooms WHERE group_name = '%s'", group_name);

    if (mysql_query(con, query)) {
        fprintf(stderr, "Error searching for group: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        fprintf(stderr, "Error storing result set: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    bool found = (mysql_num_rows(result) != 0);

    mysql_free_result(result);

    return found;
}

bool insert_chatroom(MYSQL *con, const char *creator, const char *group_name) {
    if (find_chatroom(con, group_name)) {
        printf("El grupo '%s' ya existe.\n", group_name);
        return false;
    }

    char insert_query[1024];
    sprintf(insert_query, "INSERT INTO chatrooms (creator, group_name) VALUES ('%s', '%s')", creator, group_name);

    if (mysql_query(con, insert_query)) {
        fprintf(stderr, "Error inserting row into grupos table: %s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    printf("Grupo '%s' creado exitosamente!\n", group_name);
    return true;
}


