#pragma once
#include "GameObject.h"
class FollowObject :
	public GameObject
{
public:
	FollowObject(const char*, const char*, const char*);
	void Follow(GameObject*);

	virtual glm::mat4 getWorld();

private:
	GameObject* follow;

};

