all: build run valgrind
build:
	gcc -std=c11 -o quadtree structs.h functions.c main.c -Wall -Wextra
run:
	./quadtree
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./quadtree
clean:
	rm quadtree
zip:
	zip tema1.zip *.c *.h
