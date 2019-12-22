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

#include "GameManager.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

	static void DestroyGameObject(GameObject*);
	static void AddGameObject(GameObject*);

	gCamera camera;

	static std::vector<GameObject*> gameObjects;


protected:
	// shaderekhez szükséges változók
	//ProgramObject*		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader

	/*
	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO
	*/
	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		


	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;

	static std::vector<GameObject*> toDestroy;
	static std::vector<GameObject*> toAdd;
	PlayerShip* player;

	
	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitSkyBox();
	void InitMeshes();
	void InitGameObjects();
	void RenderSkyBox();
	void AddShader(const char*, const char*, const char*);
	void AddTexture(const char*, const char*);
	void ClearGameObjects();
	void AddGameObjects();

	bool isPaused = false;
};

