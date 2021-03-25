#include "Input.h"

Input::Input() : keystate{ SDL_GetKeyboardState(NULL) }
{
	keyToCodeMapping[Key::Up] = SDL_SCANCODE_UP;
	keyToCodeMapping[Key::Down] = SDL_SCANCODE_DOWN;
	keyToCodeMapping[Key::Left] = SDL_SCANCODE_LEFT;
	keyToCodeMapping[Key::Right] = SDL_SCANCODE_RIGHT;
	keyToCodeMapping[Key::Esc] = SDL_SCANCODE_ESCAPE;
}

bool Input::IsKeyDown(Key keyName)
{
	return keystate[keyToCodeMapping[keyName]];
}
