#include "Missile.h"

Missile::Missile(glm::mat4 world, glm::vec3 pos, glm::vec3 to) : GameObject("cannontop", "default", "ship_empty")
{
	addTag("missile");
	parentWorld = world;
	this->velocity = to * baseSpeed;
	this->position = pos;
	this->size = { 0.15f, 1.0f, 0.15f };

	this->rotation = glm::vec3(0, 0, 0);
	this->damage = rand() % 4 + 2;
	//std::cout << "Missile created with " << damage << " damage" << std::endl;
	setRotation();
	InitCollisions();
}

void Missile::InitCollisions()
{
	addCollisionSphere(0, 0.1f, 0, 0.2f);
	addCollisionSphere(0, 0.9f, 0, 0.2f);
}

void Missile::Update()
{
	velocity.y -= gravity * deltaTime;
	position += velocity * deltaTime;

	setRotation();

	if (position.y < -1)
		Destroy();
}

glm::mat4 Missile::getWorld()
{
	return parentWorld * GameObject::getWorld();
}

int Missile::getDamage()
{
	return damage;
}

void Missile::setRotation()
{

	glm::vec2 norm = glm::vec2(velocity.x + velocity.z, velocity.y);
	norm = norm / (float)norm.length();

	double rotX = SDL_atan2(norm.x, norm.y);
	float rotY = SDL_atan2(velocity.x, velocity.z);

	rotation.x = SDL_fabs(rotX);
	rotation.y = rotY;
}

