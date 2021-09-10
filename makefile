all:
	gcc ./src/*.c -o cartesian -Wall -std=c99 -lSDL2

run:
	./cartesian

clean:
	- rm *.o
	- rm cartesian
