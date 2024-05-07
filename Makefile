build:
	gcc -Wall -std=c17 -I /usr/include/mysql ./src/*.c `sdl2-config --libs --cflags` -lm -o client -lSDL2_ttf -lmysqlclient

run:
	./client

build-server:
	gcc -Wall -std=c17 -I /usr/include/mysql server.c `sdl2-config --libs --cflags` -lm -o server -lSDL2_ttf -lmysqlclient

run-server:
	./server

clean:
	rm client