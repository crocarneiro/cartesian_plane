#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include "./constants.h"

int program_is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int last_frame_time = 0;

int scale = 10;
int origin_y = 0;
int origin_x = 0;

int initialize_window(void)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initialiazing SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_BORDERLESS
	);

	if(!window)
	{
		fprintf(stderr, "Error creating SDL Window. Give up\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer)
	{
		fprintf(stderr, "Error creating SDL Renderer. Give up\n");
		return false;
	}

	return true;
}

void process_input()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type)
	{
		case SDL_QUIT:
			program_is_running = false;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					fprintf(stdout, "ESC pressed. Exiting...\n");
					program_is_running = false;
					break;
				case SDLK_KP_PLUS:
					scale += 1;
					fprintf(stdout, "Zoom out. Scale %d.\n", scale);
					break;
				case SDLK_KP_MINUS:
					if(scale == 1)
					{
						fprintf(stdout, "Scale is at its minimum. You cannot zoom in anymore. Scale %d.\n", scale);
					}
					else
					{
						scale -= 1;
						fprintf(stdout, "Zoom in. Scale %d.\n", scale);
					}
					break;
				case SDLK_UP:
					origin_y += 1;
					break;
				case SDLK_DOWN:
					origin_y -= 1;
					break;
				case SDLK_LEFT:
					origin_x += 1;
					break;
				case SDLK_RIGHT:
					origin_x -= 1;
					break;
			}

			break;
	}
}

void setup()
{
}

void update()
{
	// Waste time until the frame target is reached.
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	if(time_to_wait > 0 && time_to_wait != FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);

	//Get delta time factor converted to seconds to be used to update my objects.
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	last_frame_time = SDL_GetTicks();
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int i, distance;
	for(i = scale * (-1), distance = 0; i <= scale; i++, distance += PLANE_SIZE / (scale * 2))
	{
		if(i == origin_y)
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(renderer, 194, 58, 135, 255);

		// Horizontal line
		int real_plane_size = (PLANE_SIZE / (scale * 2)) * scale * 2;
		int result = SDL_RenderDrawLine(renderer,
			PLANE_X_POS,
			PLANE_Y_POS + distance,
			PLANE_X_POS + real_plane_size,
			PLANE_Y_POS + distance
		);
		if(result < 0)
		{
			fprintf(stderr, "Error drawing line: %s.\n", SDL_GetError());
			exit(1);
		}

		if(i == origin_x)
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(renderer, 194, 58, 135, 255);
		// Vertical line
		result = SDL_RenderDrawLine(renderer,
			PLANE_X_POS + distance,
			PLANE_Y_POS,
			PLANE_X_POS + distance,
			PLANE_Y_POS + real_plane_size
		);
		if(result < 0)
		{
			fprintf(stderr, "Error drawing line: %s.\n", SDL_GetError());
			exit(1);
		}
	}

	SDL_RenderPresent(renderer);
}

void destroy_window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	program_is_running = initialize_window();

	setup();
	while(program_is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
