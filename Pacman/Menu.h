#pragma once

#include "Constants.h"

class InputManager;
class Drawer;

enum class MenuState
{
	None,
	Play,
	Quit
};

class Menu
{
public:
	Menu();

	MenuState Update(const InputManager& inputManager);
	void Draw(Drawer& drawer);

private:
	const char* playFont;
	const char* quitFont;
	bool released;
};

