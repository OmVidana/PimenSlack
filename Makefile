build:
	gcc -Wall -std=c17 -I /usr/include/mysql  ./src/client.c `sdl2-config --libs --cflags` -lm -o client -lSDL2_ttf -lbsd `pkg-config --cflags --libs gtk+-3.0`
run:
	./client

build-server:
	gcc -Wall -std=c17 -I /usr/include/mysql -I ./src/libraries/ -I /usr/include/cJSON ./src/libraries/mysql_functions.c ./src/server/server.c  `sdl2-config --libs --cflags` -lm -o server -lSDL2_ttf -lmysqlclient -g -lcjson

run-server:
	./server

clean:
	rm client