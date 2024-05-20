cmake_minimum_required(VERSION 3.22)
project(PimenSlack C)

set(CMAKE_C_STANDARD 17)
set(MYSQL_LIBRARY /usr/lib/x86_64-linux-gnu/libmysqlclient.so.21)
set(MYSQL_INCLUDE_DIR /usr/include/mysql)

file(GLOB LIBRARY_SOURCES "src/libraries/*.c")

add_executable(Server src/server/server.c ${LIBRARY_SOURCES})
target_link_libraries(Server ${MYSQL_LIBRARY})
target_include_directories(Server PRIVATE ${MYSQL_INCLUDE_DIR})

add_executable(Client src/client/client.c)
target_include_directories(Server PRIVATE src/libraries)