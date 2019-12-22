#include "FollowObject.h"

FollowObject::FollowObject(const char* meshName, const char* shader, const char* texture)
	: GameObject(meshName, shader, texture)
{
	follow = nullptr;
}

void FollowObject::Follow(GameObject* newFollow)
{
	follow = newFollow;
}

glm::mat4 FollowObject::getWorld()
{
	return ((follow == nullptr) ? glm::mat4(1.0f) : follow->getWorld()) * GameObject::getWorld();
}
