cmake_minimum_required(VERSION 3.22)
project(PimenSlack C)

set(CMAKE_C_STANDARD 17)
set(MYSQL_LIBRARY /usr/lib/x86_64-linux-gnu/libmysqlclient.so)
set(MYSQL_INCLUDE_DIR /usr/include/mysql)
set(CJSON_LIBRARY /usr/lib/x86_64-linux-gnu/libcjson.so)
set(CJSON_INCLUDE_DIR /usr/include/cjson)

file(GLOB LIBRARY_SOURCES "src/libraries/*.c")

add_executable(Server src/server/server.c ${LIBRARY_SOURCES})
target_link_libraries(Server ${MYSQL_LIBRARY} ${CJSON_LIBRARY})
target_include_directories(Server PRIVATE src/libraries ${MYSQL_INCLUDE_DIR} ${CJSON_INCLUDE_DIR})

add_executable(Client src/client/client_testing.c)
target_link_libraries(Client ${MYSQL_LIBRARY} ${CJSON_LIBRARY})
target_include_directories(Client PRIVATE src/libraries ${MYSQL_INCLUDE_DIR} ${CJSON_INCLUDE_DIR})