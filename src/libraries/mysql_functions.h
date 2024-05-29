//
// Created by patom on 06/05/2024.
//

#ifndef PIMENSLACK_MYSQL_FUNCTIONS_H
#define PIMENSLACK_MYSQL_FUNCTIONS_H

#include <mysql/mysql.h>
#include <stdbool.h>

typedef struct {
    char ***rows;
    unsigned int num_rows;
    unsigned int num_fields;
    char **field_names;
} QueryResult;


MYSQL *connect_and_create_database();

void create_table(MYSQL *con, const char *table_name, const char *columns[]);

void create_all_tables(MYSQL *con);

int insert_row(MYSQL *con, const char *table_name, const char *data[][2]);

QueryResult *fetch_records(MYSQL *con, const char *table_name, const char *conditions);

void free_query_result(QueryResult *query_result);

uint8_t find_user(MYSQL *con, const char *username, const char *password);

uint8_t find_chatroom(MYSQL *con, const char *name, const char *admin_id);

QueryResult *fetch_chatrooms(MYSQL *con, const char *conditions);

#endif //PIMENSLACK_MYSQL_FUNCTIONS_H