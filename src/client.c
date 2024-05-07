#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "mysql_functions.h"
#include "mysdl_functions.h"

int main(int argc, char *argv[]) {
    init_mysdl();
    main_loop();

//    MYSQL *con = connect_to_mysql();
//    create_database(con);
//    use_database(con);
//    create_table(con);
//
//    insert_user(con, "Alice");
//    insert_user(con, "Bob");
//    insert_user(con, "Charlie");
//
//    find_user(con, "Bob");
//    find_user(con, "David");
//
//    mysql_close(con);
//    return EXIT_SUCCESS;
}