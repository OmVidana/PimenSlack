build:
	gcc -Wall -std=c17 -I /usr/include/mysql  ./src/client.c `sdl2-config --libs --cflags` -lm -o client -lSDL2_ttf -lbsd `pkg-config --cflags --libs gtk+-3.0` | ./client
run:
	./client

build-server:
	gcc -Wall -std=c17 -I /usr/include/mysql server.c ./src/mysql_functions.c `sdl2-config --libs --cflags` -lm -o server -lSDL2_ttf -lmysqlclient -g | ./server

run-server:
	./server

clean:
	rm client