#ifndef DRAWER_H
#define DRAWER_H

#include <memory>
#include <unordered_map>
#include <tuple>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;

using Resource = std::tuple<SDL_Texture*, SDL_Surface*>;
using ResourceMap = std::unordered_map<const char*, Resource>;

class Drawer
{
public:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	~Drawer();

	void Draw(const char* anImage, int aCellX = 0, int aCellY = 0);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY);
	bool AddResource(const char* name);

private:
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
	ResourceMap resourceMap;
};

#endif // DRAWER_H