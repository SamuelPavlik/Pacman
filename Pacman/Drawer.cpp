#include "Drawer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_render.h"

Drawer::Drawer(SDL_Window* window, SDL_Renderer* renderer) : 
	window(window), 
	renderer(renderer) {}

Drawer::~Drawer()
{
	for (auto elem : resourceMap)
	{
		SDL_DestroyTexture(std::get<0>(elem.second));
		SDL_FreeSurface(std::get<1>(elem.second));
	}
}

void Drawer::Draw(const char* name, int cellX, int cellY)
{
	auto resourceIt = resourceMap.find(name);
	if (resourceIt == resourceMap.end())
		return;

	Resource resource = resourceIt->second;

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = std::get<1>(resource)->w;
    sizeRect.h = std::get<1>(resource)->h;

    SDL_Rect posRect ;
    posRect.x = cellX;
    posRect.y = cellY;
	posRect.w = std::get<1>(resource)->w;
	posRect.h = std::get<1>(resource)->h;

	SDL_RenderCopy(renderer, std::get<0>(resource), &sizeRect, &posRect);
}

void Drawer::DrawText(const char* text, const char* fontFile, int x, int y)
{
	TTF_Font* font=TTF_OpenFont(fontFile, 24);

	SDL_Color fg={255,0,0,255};
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect ;
    posRect.x = x;
    posRect.y = y;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(renderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}

bool Drawer::AddResource(const char* name)
{
	if (resourceMap.find(name) == resourceMap.end())
	{
		SDL_Surface* surface = IMG_Load(name);
		if (!surface)
			return false;

		auto texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture)
			return false;

		resourceMap[name] = Resource(texture, surface);
	}
	return true;
}
