mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
all:
	gcc ./src/*.c -o cartesian -Wall -std=c99 -lSDL2 -Wl,-rpath,/usr/local/lib -LLIBDIR /usr/local/lib/libSDL2_ttf.so
windows:
	gcc -I$(mkfile_dir)include ./src/*.c -o cartesian -Wall -L$(mkfile_dir)lib -lSDL2 -std=c99 -Wl,-rpath,$(mkfile_dir)lib -LLIBDIR $(mkfile_dir)lib/SDL2.dll -D IS_WINDOWS -D SDL_MAIN_HANDLED
run:
	./cartesian
clean:
	- rm *.o
	- rm cartesian
