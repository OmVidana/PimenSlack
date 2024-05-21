//
// Created by patom on 06/05/2024.
//

#ifndef PIMENSLACK_MYSQL_FUNCTIONS_H
#define PIMENSLACK_MYSQL_FUNCTIONS_H

#include <mysql/mysql.h>
#include <stdbool.h>

MYSQL *connect_to_mysql();

void create_database(MYSQL *con);

void use_database(MYSQL *con);

void create_table(MYSQL *con);

void create_table1(MYSQL *con, const char *table_name, const char *fields);

int insert_user(MYSQL *con, const char *username, const char *password);

bool find_user(MYSQL *con, const char *username, const char *password);

bool find_chatroom(MYSQL *con, const char *group_name);

void insert_chatroom(MYSQL *con, const char *creator, const char *group_name);

#endif //PIMENSLACK_MYSQL_FUNCTIONS_H
