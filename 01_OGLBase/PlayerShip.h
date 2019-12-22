#pragma once

#include "Ship.h"

#include "GameManager.h"

class PlayerShip : public Ship
{
public:
	PlayerShip();
	virtual void ComputeAction() final;
	virtual void AfterUpdate() final;
private:
	glm::vec2 cameraPos;
	double targetDistance;
	double distance;
	double cameraSpeed;
};

