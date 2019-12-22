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
	virtual ~MyMesh() {}
	MyMesh();
	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void bind() = 0;
	typedef std::map< const char*, MyMesh*> MeshMap;
	static MeshMap meshes;
};


class ObjMesh : public MyMesh
{
public:
	ObjMesh(const char*);
	virtual void init() final;
	virtual void draw() final;
	virtual void bind() final;

private:
	Mesh* mesh;
	const char* fileName;
};

class VaoMesh : public MyMesh
{
public:
	VaoMesh(int, int);
	virtual void init();
	virtual void draw() final;
	virtual void bind() final;

protected:
	virtual std::vector<Vertex> getVertices();
	virtual std::vector<int> getIndices();
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