all:
	gcc ./src/*.c -o cartesian -Wall -std=c99 -lSDL2 -Wl,-rpath,/usr/local/lib -LLIBDIR /usr/local/lib/libSDL2_ttf.so

run:
	./cartesian

clean:
	- rm *.o
	- rm cartesian
