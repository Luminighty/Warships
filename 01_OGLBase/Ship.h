#pragma once

#include "GameObject.h"
#include "FollowObject.h"
#include "Missile.h"

class Ship : public GameObject
{
public:
	Ship(int);
	bool isDead();
protected:
	virtual void AfterUpdate();
	virtual void ComputeAction() = 0;
	virtual glm::mat4 Animate() final;
	void Steer(float);
	void Accelerate(float);
	void Aim(float, float);
	void Shoot();
private:
	void InitCollisions();
	void Move();
	void Update();
	void TakeDamage(int);
	void Die();
	void CheckCollision();

	int deadTime = 0;
	int health;
	float speed = 0;
	float rotationSpeed = 0;
	float angle;
	float aim_x;
	float aim_y;

	static float maxRotationSpeed;
	static float maxSpeed;
	static float minAimY;
	static float maxAimY;
	static float minStep;
	static glm::vec2 aimSpeed;

	FollowObject* cannonTop;
	FollowObject* cannonBottom;
	glm::vec3 cannonOffset = glm::vec3(0.0f, 1.0f, -3.0f);;
	glm::vec3 missileOffset = glm::vec3(0, 0.9f,  -3.0f);
};

