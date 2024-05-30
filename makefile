all	: maze_generator.o
	gcc -Wall -o maze_generator maze_generator.c

maze_generator.o: maze_generator.c

%.o: %.c

clean:
	rm a.out *.o