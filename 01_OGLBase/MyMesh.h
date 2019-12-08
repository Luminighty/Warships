#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>

#include <string.h>
#include "Includes/ObjParser_OGL3.h"


#include "includes/VertexArrayObject.h"
#include "Includes/BufferObject.h"

#include <map>
#include <exception>

#include "Vertex.h"

class MyMesh
{
public:
	MyMesh(const char* meshName);
	virtual void init() = 0;
	virtual void draw() = 0;
	static std::map<const char*, MyMesh*> meshes;
};


class ObjMesh : MyMesh
{
public:
	ObjMesh(const char*, const char*);
	virtual void init() final;
	virtual void draw() final;

private:
	Mesh* mesh;
	const char* fileName;

};

class VaoMesh : MyMesh
{
public:
	VaoMesh(const char*, int, int);
	virtual void init();
	virtual void draw() final;

protected:
	virtual glm::vec3 CalcPoint(float, float);
	virtual glm::vec3 CalcNorm(float, float);
	virtual glm::vec2 CalcText(float, float);

	VertexArrayObject m_Vao;
	IndexBuffer m_Indices;
	ArrayBuffer m_VertexBuffer;
	GLuint indicesCount;

	int M;
	int N;

};