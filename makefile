Program: main.c structures.o render.o
	gcc main.c structures.o render.o -o Program -lSDL3
	rm structures.o
	rm render.o
	# gcc main.c -o Program (-Llib -Iinclude) -lSDL3

structures.o: structures.c 
	gcc -c structures.c

render.o: render.c
	gcc -c render.c