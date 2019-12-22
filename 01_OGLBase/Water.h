#pragma once


#include "GameObject.h"
class Water : public GameObject
{
public:
	Water(int x, int y) : GameObject("plane", "water", "water") {
		float tile = GameManager::tileSize;
		this->position = glm::vec3(x * tile, 0, y * tile);
		this->size = glm::vec3(50, 1, 50);
	}
};

