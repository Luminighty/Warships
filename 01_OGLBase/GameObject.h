#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "MyMesh.h"

enum GameObjectType
{
	WATER, SHIP
};

class GameObject
{

public:
	GameObject();
	virtual void Update() = 0;
	virtual void Render();

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 size;

};

