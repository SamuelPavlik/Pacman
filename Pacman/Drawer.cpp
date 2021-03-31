#include "Drawer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_render.h"

Drawer::Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer)
: myWindow(aWindow)
, myRenderer(aRenderer)
{
}

Drawer::~Drawer()
{
	for (auto elem : resourceMap)
	{
		SDL_DestroyTexture(std::get<0>(elem.second));
		SDL_FreeSurface(std::get<1>(elem.second));
	}
}

void Drawer::Draw(const char* name, int aCellX, int aCellY)
{
	Resource resource = resourceMap[name];

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = std::get<1>(resource)->w;
    sizeRect.h = std::get<1>(resource)->h;

    SDL_Rect posRect ;
    posRect.x = aCellX;
    posRect.y = aCellY;
	posRect.w = std::get<1>(resource)->w;
	posRect.h = std::get<1>(resource)->h;

	SDL_RenderCopy(myRenderer, std::get<0>(resource), &sizeRect, &posRect);
}

void Drawer::DrawText(const char* aText, const char* aFontFile, int aX, int aY)
{
	TTF_Font* font=TTF_OpenFont(aFontFile, 24);

	SDL_Color fg={255,0,0,255};
	SDL_Surface* surface = TTF_RenderText_Solid(font, aText, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect ;
    posRect.x = aX;
    posRect.y = aY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);
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

		auto texture = SDL_CreateTextureFromSurface(myRenderer, surface);
		if (!texture)
			return false;

		resourceMap[name] = Resource(texture, surface);
	}
	return true;
}
