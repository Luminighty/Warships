#pragma once
#include "GameObject.h"
#include <random>

class Missile :
	public GameObject
{
public:
	Missile(glm::mat4, glm::vec3, glm::vec3);
	virtual void Update();
	virtual glm::mat4 getWorld();
	long owner;
	int getDamage();

private:
	void InitCollisions();
	void setRotation();
	glm::vec3 velocity;
	glm::mat4 parentWorld;
	float baseSpeed = 20.0f;
	float gravity = 12.0f;
	int damage;
};

