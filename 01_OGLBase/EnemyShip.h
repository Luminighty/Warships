#pragma once
#include "Ship.h"
class EnemyShip :
	public Ship
{
public:
	EnemyShip(glm::vec3 pos);
	virtual void ComputeAction() final;
	virtual void AfterUpdate() final;
};

