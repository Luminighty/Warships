#pragma once
#include "Ship.h"
#include "GameManager.h"
#include <random>

class EnemyShip :
	public Ship
{
public:
	EnemyShip(glm::vec3 pos);
	virtual void ComputeAction() final;
	virtual void AfterUpdate() final;
private:
	float calcSteerWay();
	float calcRotation();
	glm::vec3 getNewRotation();
	bool isTooCloseToEdge();
	float moveTime;
	float steerWay = 0.0f;
	float rotateTowards;
	static float rotationDifference;
	static float mapEdgeDistance;
};

