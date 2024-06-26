//
// Created by patom on 06/05/2024.
//

#ifndef PIMENSLACK_MYSQL_FUNCTIONS_H
#define PIMENSLACK_MYSQL_FUNCTIONS_H

#include <mysql/mysql.h>
#include <stdbool.h>

MYSQL *connect_and_create_database();

void create_table(MYSQL *con, const char *table_name, const char* columns[]);

void create_all_tables(MYSQL *con);

int insert_row(MYSQL *con, const char *table_name, const char *data[][2]);

uint8_t find_user(MYSQL *con, const char *username, const char *password);

#endif //PIMENSLACK_MYSQL_FUNCTIONS_H