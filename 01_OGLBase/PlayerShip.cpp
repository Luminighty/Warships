#include "PlayerShip.h"

#include "Input.h"

#include "MyApp.h"

PlayerShip::PlayerShip() : Ship(10)
{
	addTag("player");

	distance = 10; 
	targetDistance = distance;
	cameraSpeed = 1;
	cameraPos = { M_PI / 2, 1 };
	AfterUpdate();
}

void PlayerShip::ComputeAction()
{
	if (Input::isHeld(Input::InputKey::FORWARD))
		Accelerate(1.0f);
	else if (Input::isHeld(Input::InputKey::BACKWARD))
		Accelerate(-1.0f);

	if (Input::isHeld(Input::InputKey::LEFT))
		Steer(-1.0f);
	else if (Input::isHeld(Input::InputKey::RIGHT))
		Steer(1.0f);

	if (Input::isHeld(Input::InputKey::AIM_UP))
		Aim(0.0f, -1.0f);
	else if (Input::isHeld(Input::InputKey::AIM_DOWN))
		Aim(0.0f, 1.0f);

	if (Input::isHeld(Input::InputKey::AIM_RIGHT))
		Aim( 1.0f, 0.0f);
	else if (Input::isHeld(Input::InputKey::AIM_LEFT))
		Aim(-1.0f, 0.0f);

	if (Input::isPressed(Input::InputKey::SHOOT))
		Shoot();
}

void PlayerShip::AfterUpdate()
{
}