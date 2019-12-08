#include "Input.h"

#include <sstream>
#include <iostream>

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

void Input::init()
{
	Input::keys[SDLK_w] = InputKey::FORWARD;
	Input::keys[SDLK_s] = InputKey::BACKWARD;
	Input::keys[SDLK_a] = InputKey::LEFT;
	Input::keys[SDLK_d] = InputKey::RIGHT;
	Input::keys[SDLK_r] = InputKey::AIM_UP;
	Input::keys[SDLK_f] = InputKey::AIM_DOWN;
	Input::keys[SDLK_q] = InputKey::AIM_LEFT;
	Input::keys[SDLK_e] = InputKey::AIM_RIGHT;
	Input::keys[SDLK_SPACE] = InputKey::SHOOT;
}

void Input::KeyboardDown(SDL_KeyboardEvent& e)
{
	std::map<SDL_Keycode, InputKey>::iterator it = Input::keys.find(e.keysym.sym);
	if (it != (Input::keys).end())
	{
		InputKey k = it->second;
		if (values[k] == 0)
			values[k] = 1;
	}
}

void Input::KeyboardUp(SDL_KeyboardEvent& e)
{
	std::map<SDL_Keycode, InputKey>::iterator it = Input::keys.find(e.keysym.sym);
	if (it == (Input::keys).end())
		return;
	
	InputKey k = Input::keys[e.keysym.sym];
	//std::cout << it->first << " released" << std::endl;
	if (values[k] == 2)
		values[k] = -1;
}

void Input::Update()
{
	for (std::map<Input::InputKey, int>::iterator it = Input::values.begin(); it != Input::values.end(); it++)
	{
		if (it->second == -1 || it->second == 1)
			Input::values[it->first] = (it->second) + 1;
	}
}
