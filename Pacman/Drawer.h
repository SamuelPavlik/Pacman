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

using Resource = std::tuple<std::shared_ptr<SDL_Texture>, int, int>;
using ResourceMap = std::unordered_map<const char*, Resource>;

class Drawer
{
public:
	static std::shared_ptr<Drawer> Create(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	~Drawer(void);

	void Draw(const char* anImage, int aCellX = 0, int aCellY = 0);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY);
	void AddResource(const char* name);

private:
	bool Init();
	
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
	SDL_Surface* world;
	ResourceMap resourceMap;
};

#endif // DRAWER_H