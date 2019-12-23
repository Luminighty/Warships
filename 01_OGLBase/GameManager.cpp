#include "GameManager.h"

/*
glm::mat4 GameManager::m_matWorld = {};
glm::vec3 GameManager::eye = { 0, 0, 0 };
glm::mat4 GameManager::m_matView = {};
glm::mat4 GameManager::m_matProj = {};
*/
glm::mat4 GameManager::viewProj = glm::mat4(1.0f);
std::map<const char*, ProgramObject*> GameManager::programs = {};
std::map<const char*, Texture2D*> GameManager::textures = {};

float GameManager::tileSize = 50.0f;
int   GameManager::mapSize  = 3;

float GameManager::fullMapSize()
{
	return mapSize * tileSize;
}
