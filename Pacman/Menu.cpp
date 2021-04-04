#include "Menu.h"
#include "InputManager.h"
#include "Drawer.h"

#include <string>

Menu::Menu() :
	playFont(BOLD_HUD_FONT),
	quitFont(HUD_FONT),
	released(true) {}

MenuState Menu::Update(InputManager& inputManager)
{
	if (inputManager.IsKeyPressed(InputManager::Key::Down) 
		|| inputManager.IsKeyPressed(InputManager::Key::Up))
	{
		if (released)
		{
			const char* temp = playFont;
			playFont = quitFont;
			quitFont = temp;
			released = false;
		}
	}
	else
	{
		released = true;
	}

	if (inputManager.IsKeyPressed(InputManager::Key::Enter))
	{
		if (playFont == BOLD_HUD_FONT)
		{
			return MenuState::Play;
		}
		else
		{
			return MenuState::Quit;
		}
	}
	
	return MenuState::None;
}

void Menu::Draw(Drawer& drawer)
{
	drawer.DrawText(START_TITLE.c_str(), playFont, START_X, START_Y);
	drawer.DrawText(QUIT_TITLE.c_str(), quitFont, QUIT_X, QUIT_Y);
}
