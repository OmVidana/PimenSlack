cmake_minimum_required(3.22.1)
project(PimenSlack)

set(CMAKE_C_STANDARD 17)



add_executable(server server.c)
add_executable(mysql_functions mysql_functions.c)
