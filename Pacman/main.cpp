#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"
#include "Pacman.h"
#include "Drawer.h"

#include <iostream>

int main(int argc, char **argv)
{
	/* initialize SDL */
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		assert(0 && "Failed to initialize video!");
		exit(-1);
	}
	
	SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(!window)
	{
		assert(0 && "Failed to create window!");
		exit(-1);
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() == -1)
	{
		assert(0 && "Failed to create ttf!");
		exit(-1);
	}

	if (!window)
		return -1;
	auto drawer = Drawer(window, renderer);
	auto pacman = Pacman::Create(drawer);

	float lastFrame = (float) SDL_GetTicks() * 0.001f;
	SDL_Event event;
	while (SDL_PollEvent(&event) >= 0)
	{
		float currentFrame = (float) SDL_GetTicks() * 0.001f;
		float elapsedTime = currentFrame - lastFrame;

		if (!pacman->Update(elapsedTime))
			break;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		pacman->Draw();
		
		lastFrame = currentFrame;		

		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

