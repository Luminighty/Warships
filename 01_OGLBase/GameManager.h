#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

#include "GameObject.h"
#include "PlayerShip.h"
#include "Input.h"
#include <vector>

class GameManager
{
public:
	/*
	static glm::mat4 m_matWorld;
	static glm::mat4 m_matView;
	static glm::mat4 m_matProj;

	static glm::vec3 eye;
	*/
	static glm::mat4 viewProj;

	static std::map<const char*, ProgramObject*> programs;
	static std::map<const char*, Texture2D*> textures;
	static float tileSize;
	static int mapSize;

private:
	GameManager();
};

