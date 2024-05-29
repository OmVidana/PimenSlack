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

void create_adjective_and_noun_tables(MYSQL *con);

char *generate_nickname(MYSQL *con);

int insert_row(MYSQL *con, const char *table_name, const char *data[][2]);

int insert_row_channels_users(MYSQL *con, const char *channel_id, const char *user_id, const char *status);

int insert_row_channels_messages(MYSQL *con, const char *channel_id, const char *message_id);

QueryResult *fetch_records(MYSQL *con, const char *table_name, const char *conditions);

void free_query_result(QueryResult *query_result);

uint8_t find_user(MYSQL *con, const char *username, const char *password);

uint8_t find_chatroom(MYSQL *con, const char *name, const char *admin_id);

QueryResult *fetch_users(MYSQL *con, const char *conditions);

QueryResult *fetch_chatrooms(MYSQL *con, const char *conditions);

QueryResult *fetch_chatroom_users(MYSQL *con, const char *conditions);

QueryResult *fetch_messages(MYSQL *con, const char *conditions);

QueryResult *fetch_channel_messages(MYSQL *con, const char *conditions);

int register_user(MYSQL *con, const char *name, const char *password, const char *status);

uint8_t login(MYSQL *con, const char *username, const char *password);

int create_channel(MYSQL *con, const char *chatroom_name, int user_id);

int create_message(MYSQL *con, const char *message, int user_id, int channel_id);

#endif //PIMENSLACK_MYSQL_FUNCTIONS_H