#pragma once

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
	Drawer(SDL_Window* window, SDL_Renderer* renderer);
	~Drawer();

	void Draw(const char* image, int cellX = 0, int cellY = 0);
	void DrawText(const char* text, const char* fontFile, int x, int y);
	bool AddResource(const char* name);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	ResourceMap resourceMap;
};