#include "Ship.h"
#include "MyApp.h"

#include <sstream>

float Ship::maxRotationSpeed = 1.0f;
float Ship::minStep = 0.01f;
float Ship::maxSpeed = 5.0f;
float Ship::minAimY = 20 * (M_PI / 180.0f);
float Ship::maxAimY = 60 * (M_PI / 180.0f);
glm::vec2 Ship::aimSpeed = { 1.5f, 1.0f };

Ship::Ship(int health) 
: GameObject("ship", "default", "ship_texture")
{
	addTag("ship");

	this->angle = 0.0f;
	this->health = health;
	this->aim_y = 30.0f * (M_PI / 180.0f);
	this->aim_x = M_PI;
	this->deadTime = 0;


	this->cannonBottom = new FollowObject("cannonbottom", "default", "ship_empty");
	this->cannonTop = new FollowObject("cannontop", "default", "ship_empty");
	cannonTop->size = glm::vec3(0.25f, 1.0f, 0.25f);
	cannonBottom->size = glm::vec3(0.5f, 0.25f, 0.5f);

	cannonTop->position = cannonOffset - glm::vec3(0, 0.1f, 0);
	cannonBottom->position = cannonOffset;

	cannonTop->Follow(this);
	cannonBottom->Follow(this);

	cannonTop->rotation = glm::vec3(aim_y, -aim_x, 0);
	cannonBottom->rotation = glm::vec3(0, -aim_x, 0);


	initObject(cannonBottom);
	initObject(cannonTop);
	InitCollisions();
}

glm::mat4 Ship::Animate()
{
	if (isDead()) {
		float time = (SDL_GetTicks() - deadTime) / 5000.0f;
		float y = -time * 3.0f;
		if (y < -5.0f) {
			Destroy();
			cannonTop->Destroy();
			cannonBottom->Destroy();
		}


		return glm::translate(glm::vec3(0, y, 0)) *
			glm::rotate(-time, glm::vec3(1, 0, 0));
	}


	float angle = SDL_cos(SDL_GetTicks() / 1000.0f) * 0.1f;
	return glm::rotate(angle, glm::vec3(1, 0, 0));
}

void Ship::TakeDamage(int damage)
{
	health -= damage;
	std::cout << getId() << " took " << damage << "damage (" << health << " remained)" << std::endl;
	if (isDead())
		Die();
}

void Ship::Die()
{
	health = 0;
	deadTime = SDL_GetTicks();
}

void Ship::CheckCollision()
{
	for (std::vector<GameObject*>::iterator it = CMyApp::gameObjects.begin(); it != CMyApp::gameObjects.end(); it++)
	{
		GameObject* o = *it;
		if (o == nullptr)
			continue;
		if (!o->hasTag("missile"))
			continue;
		Missile* m = (Missile*)o;
		if (m->owner == getId())
			continue;

		if (isCollide(this, m)) {
			TakeDamage(m->getDamage());
			m->Destroy();
		}
	}


	// Edge of the map
	float edge = GameManager::mapSize * GameManager::tileSize - 5.0f;
	if (SDL_fabs(position.x) > edge || SDL_fabs(position.z) > edge)
		Die();
}

void Ship::InitCollisions()
{
	addCollisionSphere(0, 0, 0.5f, 1.5f);
	addCollisionSphere(0, 1.0f, -0.5f, 1.0f);
	addCollisionSphere(0, 0, -2.5f, 1.5f);
	addCollisionSphere(0, 0, -5.0f, 1.5f);
}


void Ship::Update()
{
	if (isDead())
		return;
	ComputeAction();
	Move();
	CheckCollision();

	if (SDL_fabs(rotationSpeed) > minStep) {
		angle += rotationSpeed * deltaTime;
		rotationSpeed -= ((rotationSpeed > 0) ? 1 : -1) * deltaTime * 0.7f;
	}
	if (SDL_fabs(speed) > minStep) {
		speed -= ((speed > 0) ? 1 : -1) * deltaTime;
	}

	//std::cout << SDL_fabs(speed) << std::endl;
	rotation.y = angle;

	AfterUpdate();

}

void Ship::Move()
{
	if (SDL_fabs(speed) > minStep) {
		float x = -SDL_sin(angle) * deltaTime * speed;
		float z = -SDL_cos(angle) * deltaTime * speed;

		position.x += x;
		position.z += z;
	}
}

void Ship::Steer(float x)
{
	rotationSpeed -= x * deltaTime;
	if (SDL_fabs(rotationSpeed) >= maxRotationSpeed)
		rotationSpeed = ((rotationSpeed > 0) ? 1 : -1)* maxRotationSpeed;
	if (SDL_fabs(speed) > minStep)
		speed -= ((speed > 0) ? 1 : -1) * deltaTime * 0.7f;
}

void Ship::Accelerate(float x)
{
	float accSpeed = (x * angle > 0) ? 1 : 2;
	speed += x * accSpeed * deltaTime * 1.75f;
	float signedMax = (speed > 0) ? maxSpeed : -maxSpeed;
	if (SDL_fabs(speed) >= maxSpeed) {
		speed = signedMax;
	}
}

void Ship::Aim(float x, float y)
{
	aim_x += x * deltaTime * aimSpeed.x;
	aim_y += y * deltaTime * aimSpeed.y;
	if (aim_y >= maxAimY)
		aim_y = maxAimY;
	if (aim_y <= minAimY)
		aim_y = minAimY;

	cannonTop->rotation = glm::vec3(aim_y, -aim_x, 0);
	cannonBottom->rotation = glm::vec3(0,-aim_x,0);
}

void Ship::Shoot()
{
	float cosX = SDL_cos(aim_x);
	float sinX = SDL_sin(aim_x);
	float cosY = SDL_cos(aim_y);
	float sinY = SDL_sin(aim_y);

	glm::mat4 world = getWorld();
	Missile* missile = new Missile(world, missileOffset, { -sinX * sinY,  cosY, cosX * sinY });
	
	missile->owner = getId();
	initObject(missile);
}

void Ship::AfterUpdate() {}

bool Ship::isDead()
{
	return health <= 0;
}