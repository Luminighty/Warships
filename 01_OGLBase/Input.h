#pragma once

#include <map>

#include <SDL.h>
#include <SDL_opengl.h>


class Input
{
public:	
	enum class InputKey
	{
		FORWARD, BACKWARD, LEFT, RIGHT,
		AIM_UP, AIM_DOWN, AIM_LEFT, AIM_RIGHT,
		SHOOT
	};

	static bool isPressed(InputKey);
	static bool isHeld(InputKey);
	static bool isReleased(InputKey);
	static void init();
	static void KeyboardDown(SDL_KeyboardEvent&);
	static void KeyboardUp(SDL_KeyboardEvent&);
	static void Update();

private:
	static std::map<InputKey, int> values;
	static std::map<SDL_Keycode, InputKey> keys;
};

