#pragma once

#include <map>

#include <SDL.h>
#include <SDL_opengl.h>


class Input
{
public:	
	enum class InputKey
	{
		FORWARD, 
		BACKWARD, 
		LEFT, 
		RIGHT,
		AIM_UP, 
		AIM_DOWN, 
		AIM_LEFT, 
		AIM_RIGHT,
		SHOOT
	};

	static bool isPressed(InputKey);
	static bool isHeld(InputKey);
	static bool isReleased(InputKey);
	static void KeyboardDown(SDL_KeyboardEvent&);
	static void KeyboardUp(SDL_KeyboardEvent&);
	static void Update();
	static const char* getEnumName(Input::InputKey);

private:
	typedef std::map<InputKey, int> KeyValueMap;
	typedef std::map<SDL_Keycode, InputKey> KeyCodeKeyMap;
	static KeyValueMap values;
	static KeyCodeKeyMap keys;
	static const bool DEBUG_KEYS = false;
};