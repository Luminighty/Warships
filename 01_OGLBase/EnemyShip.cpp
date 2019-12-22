#include "EnemyShip.h"

EnemyShip::EnemyShip(glm::vec3 pos) : Ship(20)
{
	position = pos;
}

void EnemyShip::ComputeAction()
{
}

void EnemyShip::AfterUpdate()
{
}
