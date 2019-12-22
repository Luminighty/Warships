#include "Input.h"

#include <sstream>
#include <iostream>

Input::KeyValueMap Input::values = {};
Input::KeyCodeKeyMap Input::keys = {
	{SDLK_w,     InputKey::FORWARD   },
	{SDLK_s,     InputKey::BACKWARD  },
	{SDLK_a,     InputKey::LEFT      },
	{SDLK_d,     InputKey::RIGHT     },
	{SDLK_r,     InputKey::AIM_UP    },
	{SDLK_f,     InputKey::AIM_DOWN  },
	{SDLK_q,     InputKey::AIM_LEFT  },
	{SDLK_e,     InputKey::AIM_RIGHT },
	{SDLK_LCTRL, InputKey::SHOOT     }
};

const char* Input::getEnumName(Input::InputKey key)
{
	switch (key)
	{
	case Input::InputKey::FORWARD:
		return "FORWARD";
	case Input::InputKey::BACKWARD:
		return "BACKWARD";
	case Input::InputKey::LEFT:
		return "LEFT";
	case Input::InputKey::RIGHT:
		return "RIGHT";
	case Input::InputKey::AIM_UP:
		return "AIM_UP";
	case Input::InputKey::AIM_DOWN:
		return "AIM_DOWN";
	case Input::InputKey::AIM_LEFT:
		return "AIM_LEFT";
	case Input::InputKey::AIM_RIGHT:
		return "AIM_RIGHT";
	case Input::InputKey::SHOOT:
		return "SHOOT";
	default:
		return "NONE";
	}
}

bool Input::isPressed(Input::InputKey key)
{
	return Input::values[key] == 1;
}

bool Input::isHeld(Input::InputKey key)
{
	return Input::values[key] >= 1;
}

bool Input::isReleased(Input::InputKey key)
{
	return Input::values[key] == -1;
}


void Input::KeyboardDown(SDL_KeyboardEvent& e)
{
	std::map<SDL_Keycode, InputKey>::iterator it = Input::keys.find(e.keysym.sym);
	if (it != (Input::keys).end())
	{
		InputKey k = it->second;
		if (values[k] == 0) {
			values[k] = 1;
			if (Input::DEBUG_KEYS)
			std::cout << Input::getEnumName(it->second) << " pressed" << std::endl;
		}
	}
}

void Input::KeyboardUp(SDL_KeyboardEvent& e)
{
	std::map<SDL_Keycode, InputKey>::iterator it = Input::keys.find(e.keysym.sym);
	if (it == (Input::keys).end())
		return;
	
	InputKey k = Input::keys[e.keysym.sym];
	if (values[k] == 2) {
		values[k] = -1;
		if (Input::DEBUG_KEYS)
		std::cout << Input::getEnumName(it->second) << " released" << std::endl;
	}
}

void Input::Update()
{
	for (std::map<Input::InputKey, int>::iterator it = Input::values.begin(); it != Input::values.end(); it++)
	{
		if (it->second == -1 || it->second == 1)
			Input::values[it->first] = (it->second) + 1;
	}
}
