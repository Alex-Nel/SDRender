Program: main.c structures.o render.o
	gcc -g main.c structures.o render.o -o Program -I./include -L./lib -lSDL3
	rm structures.o
	rm render.o

	# Main render command
	# gcc main.c -o Program (-Llib -Iinclude) -lSDL3

structures.o: structures.c 
	gcc -c structures.c -Iinclude -Llib -lSDL3

render.o: render.c
	gcc -c render.c -Iinclude -Llib -lSDL3