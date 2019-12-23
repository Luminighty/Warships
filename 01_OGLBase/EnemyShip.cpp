#include "EnemyShip.h"

float EnemyShip::mapEdgeDistance = 50.0f;
float EnemyShip::rotationDifference = 0.1f;

EnemyShip::EnemyShip(glm::vec3 pos) : Ship(10)
{
	position = pos;
	rotateTowards = 0;
	calcSteerWay();
}

void EnemyShip::ComputeAction()
{

	if (SDL_fabs(rotateTowards - rotation.y) > rotationDifference) {
		steerWay = calcSteerWay();
		Steer(steerWay);
		//std::cout << "steer " << rotateTowards << " " << rotation.y << std::endl;
		return;
	}
	moveTime -= deltaTime;
	//std::cout << "move " << moveTime << std::endl;
	Accelerate(1.0f);
	// next action
	if (moveTime > 0)
		return;
	moveTime = 5.0f;

	if (isTooCloseToEdge()) {
		rotateTowards = calcRotation();
		steerWay = calcSteerWay();
	}
	int action = rand() % 100;
	//std::cout << "action " << action << std::endl;
	if (action < 70) {
		rotateTowards = calcRotation();
		steerWay = calcSteerWay();
	}
}


void EnemyShip::AfterUpdate()
{
}

float EnemyShip::calcSteerWay()
{
	float dif = SDL_fabs(rotateTowards - rotation.y);
	if (dif > M_PI)
		dif *= -1;
	if (dif / deltaTime > 1.0f)
		dif = 1.0f;
	return (rotateTowards > rotation.y) ? -dif : dif;
}

float EnemyShip::calcRotation()
{
	glm::vec3 toPoint = getNewRotation() - position;
	return SDL_atan2(toPoint.x, toPoint.z);
}

glm::vec3 EnemyShip::getNewRotation()
{
	float max = GameManager::mapSize * GameManager::tileSize - mapEdgeDistance;
	float x = ((float)rand()) / max * (max - mapEdgeDistance);
	float z = ((float)rand()) / max * (max - mapEdgeDistance);
	return glm::vec3(x, 0, z);
}

bool EnemyShip::isTooCloseToEdge()
{
	float mapSize = GameManager::mapSize * GameManager::tileSize;
	return SDL_fabs(position.x) > mapSize - mapEdgeDistance ||
		SDL_fabs(position.z) > mapSize - mapEdgeDistance;
}
