#pragma once

#include <SDL_keyboard.h>

#include <unordered_map>

class Input
{
public:
    enum class Key
    {
        None = 0,
        Left = 1,
        Right = 2,
        Up = 3,
        Down = 4,
        Esc = 5
    };
    Input();
    bool IsKeyDown(Key keyName); // Returns true if the key was just pressed.
    //void Update();
    //bool IsKeyPressed(std::string keyName); // Returns true if the key is pressed.
    //bool IsKeyUp(std::string keyName); // Returns true if the key was just released.

private:
    std::unordered_map<Key, SDL_Scancode> keyToCodeMapping;
    const Uint8* keystate;
};

