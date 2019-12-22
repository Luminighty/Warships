#include "GameObject.h"

#include "MyApp.h"
#include <iostream>
#include <string>

#include <exception>

float GameObject::deltaTime = SDL_GetTicks() / 1000.0f;

void GameObject::initObject(GameObject* obj)
{
	CMyApp::AddGameObject(obj);
}

void GameObject::Destroy()
{
	CMyApp::DestroyGameObject(this);
}

GameObject::GameObject(const char* meshName, const char* shader, const char* texture)
{
	auto meshContains = MyMesh::meshes.find(meshName);
	if (meshContains == MyMesh::meshes.end())
		throw std::exception(("Mesh " + std::string(shader) + " not found").c_str());

	auto shaderContains = GameManager::programs.find(shader);
	if (shaderContains == GameManager::programs.end())
		throw std::exception(("Shader " + std::string(shader) + " not found").c_str());

	auto textureContains = GameManager::textures.find(texture);
	if (textureContains == GameManager::textures.end())
		throw std::exception(("Texture " + std::string(shader) + " not found").c_str());

	this->mesh = MyMesh::meshes[meshName];
	this->shader = GameManager::programs[shader];
	this->texture = GameManager::textures[texture];
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	size     = glm::vec3(1, 1, 1);
	this->id = lastId++;
}

GameObject::~GameObject()
{
	
}

void GameObject::Render()
{
	glm::mat4 world = getWorld();

	mesh->bind();
	shader->Use();
	SetShaderValues(world);
	mesh->draw();

	if (debugCollision) {
		MyMesh* m = MyMesh::meshes["sphere"];
		m->bind();
		for (std::vector<glm::vec4>::iterator it = collisionSpheres.begin(); it != collisionSpheres.end(); it++) {
			glm::vec4 col = *it;
			glm::vec4 pos4 = world * glm::vec4(col.x, col.y, col.z, 1.0f);
			glm::vec3 pos = { pos4.x, pos4.y, pos4.z };
			float r = col.w;

			glm::mat4 newWorld = glm::translate(pos) * glm::scale(glm::vec3(r, r, r));
			SetShaderValues(newWorld);
			m->draw();
		}
	}
	//shader->Unuse();
}

void GameObject::SetShaderValues(glm::mat4 world)
{
	shader->SetTexture("texImage", 0, *texture);
	shader->SetUniform("MVP", GameManager::viewProj * world);
	shader->SetUniform("world", world);
	shader->SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	shader->SetUniform("time", (int)SDL_GetTicks());
}

glm::mat4 GameObject::getWorld()
{
	return glm::translate(position) *
		glm::rotate(rotation.z, glm::vec3(0, 0, 1)) *
		glm::rotate(rotation.y, glm::vec3(0, 1, 0)) *
		glm::rotate(rotation.x, glm::vec3(1, 0, 0)) *
		Animate() *
		glm::scale(size)
		;
}

void GameObject::Update() {}

long GameObject::lastId = 0;

glm::mat4 GameObject::Animate() { return glm::mat4(1.0f); }

bool GameObject::isCollide(GameObject* a, GameObject* b)
{
	glm::mat4 worldA = a->getWorld();
	glm::mat4 worldB = b->getWorld();
	for (std::vector<glm::vec4>::iterator itA = a->collisionSpheres.begin(); itA != a->collisionSpheres.end(); itA++)
		for (std::vector<glm::vec4>::iterator itB = b->collisionSpheres.begin(); itB != b->collisionSpheres.end(); itB++) {
			glm::vec4 colA = *itA;
			glm::vec4 colB = *itB;

			glm::vec4 pos4A = worldA * glm::vec4(colA.x, colA.y, colA.z, 1.0f);
			glm::vec4 pos4B = worldB * glm::vec4(colB.x, colB.y, colB.z, 1.0f);
			glm::vec3 posA = { pos4A.x, pos4A.y, pos4A.z };
			glm::vec3 posB = { pos4B.x, pos4B.y, pos4B.z };
			float rangA = colA.w;
			float rangB = colB.w;			

			float distance = glm::distance(posA, posB);
			if (distance < rangA + rangB)
				return true;
		}
	return false;
}

long GameObject::getId() { return id; }

void GameObject::addCollisionSphere(float x, float y, float z, float range)
{
	collisionSpheres.push_back(glm::vec4(x, y, z, range));
}

void GameObject::addTag(const char* tag)
{
	tags.push_back(tag);
}

bool GameObject::removeTag(const char* tag)
{
	for (unsigned int i = 0; i < tags.size(); i++) {
		if (tags[i] == tag) {
			tags[i] = tags[tags.size() - 1];
			tags.pop_back();
			return true;
		}
	}
	return false;
}
bool GameObject::hasTag(const char* tag)
{
	for (unsigned int i = 0; i < tags.size(); i++)
		if (tags[i] == tag)
			return true;

	return false;
}

bool operator==(const GameObject& lhs, const GameObject& rhs) { return lhs.id == rhs.id; }
