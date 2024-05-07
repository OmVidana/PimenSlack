build:
	gcc -Wall -std=c17 -I /usr/include/mysql ./src/*.c `sdl2-config --libs --cflags` -lm -o client -lSDL2_ttf -lmysqlclient

run:
	./client

clean:
	rm client