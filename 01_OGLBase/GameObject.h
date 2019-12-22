#pragma once

#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

#include "MyMesh.h"

#include <set>

class GameObject
{
public:
	GameObject(const char* meshName, const char* shader, const char* texture);
	~GameObject();
	void Destroy();
	virtual void Update();
	virtual void Render();
	virtual void SetShaderValues(glm::mat4 world);
	virtual glm::mat4 getWorld();

	long getId();
	void addCollisionSphere(float x, float y, float z, float range);
	void addTag(const char*);
	bool removeTag(const char*);
	bool hasTag(const char*);

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 size;

	friend bool operator==(const GameObject&, const GameObject&);

	static float deltaTime;
	static void initObject(GameObject*);
protected:
	virtual glm::mat4 Animate();
	MyMesh* mesh;
	ProgramObject* shader;
	Texture2D* texture;

	static bool isCollide(GameObject*, GameObject*);

private:
	std::vector<const char*> tags;
	long id;
	const static bool debugCollision = false;
	static long lastId;
	std::vector<glm::vec4> collisionSpheres;
};