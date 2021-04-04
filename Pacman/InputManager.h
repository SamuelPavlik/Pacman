#pragma once

#include <SDL_keyboard.h>

#include <unordered_map>

class InputManager
{
public:
	enum class Key
	{
		None = 0,
		Left = 1,
		Right = 2,
		Up = 3,
		Down = 4,
		Esc = 5,
		Enter = 6
	};

	InputManager();
	bool IsKeyPressed(Key keyName);

private:
	std::unordered_map<Key, SDL_Scancode> keyToCodeMapping;
	const Uint8* keystate;
};

