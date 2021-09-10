#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int program_is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int last_frame_time = 0;

int scale = 10;

int initialize_window(void)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initialiazing SDL.\n");
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
			if(event.key.keysym.sym == SDLK_ESCAPE)
				program_is_running = false;
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

	SDL_SetRenderDrawColor(renderer, 194, 58, 135, 255);

	// Horizontal line
	int result = SDL_RenderDrawLine(renderer,
		PLANE_X_POS,
		PLANE_Y_POS,
		PLANE_X_POS + PLANE_WIDTH,
		PLANE_Y_POS
	);
	if(result < 0)
	{
		fprintf(stderr, "Error drawing line: %s.\n", SDL_GetError());
		exit(1);
	}

	// Verticla line
	result = SDL_RenderDrawLine(renderer,
		PLANE_X_POS,
		PLANE_Y_POS,
		PLANE_X_POS,
		PLANE_Y_POS + PLANE_HEIGHT
	);
	if(result < 0)
	{
		fprintf(stderr, "Error drawing line: %s.\n", SDL_GetError());
		exit(1);
	}

	SDL_RenderPresent(renderer);
}

void destroy_window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main()
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
