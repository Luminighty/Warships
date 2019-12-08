#include "MyMesh.h"


MyMesh::MyMesh(const char* meshName)
{
	auto contains = meshes.find(meshName);
	if (contains == meshes.end())
		throw std::exception("Mesh name is already set exception");
	meshes[meshName] = this;
}

ObjMesh::ObjMesh(const char* meshName, const char* fileName)
: MyMesh(meshName)
{
	this->fileName = fileName;
	mesh = nullptr;
}

void ObjMesh::init()
{
	mesh = ObjParser::parse(fileName);
	mesh->initBuffers();
}

void ObjMesh::draw()
{
	mesh->draw();
}

VaoMesh::VaoMesh(const char* meshName, int vertNCount, int vertMCount)
: MyMesh(meshName)
{
	indicesCount = -1;
	N = vertNCount;
	M = vertMCount;
}

void VaoMesh::init()
{
	std::vector<Vertex> vertices;

	for (int x = 0; x <= N; x++) {
		for (int y = 0; y <= M; y++) {
			float u = x / (float)N;
			float v = y / (float)M;

			vertices.push_back({ CalcPoint(u, v), CalcNorm(u, v) , CalcText(u, v) });
		}
	}

	std::vector<int> indices;
	int index = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			indices[6 * i + j * 3 * 2 * N + 5] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * N + 4] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * N + 3] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * N + 2] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * N + 1] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * N + 0] = (i)+(j + 1) * (N + 1);
		}
	}
	indicesCount = indices.size();

	m_VertexBuffer.BufferData(vertices);

	m_Indices.BufferData(indices);

	m_Vao.Init(
		{
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_VertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_VertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_VertexBuffer },
		},
		m_Indices
	);
}

glm::vec2 VaoMesh::CalcText(float u, float v)
{
	return glm::vec2(u, v);
}

glm::vec3 VaoMesh::CalcNorm(float u, float v)
{
	glm::vec3 du = CalcPoint(u + 0.01, v) - CalcPoint(u - 0.01, v);
	glm::vec3 dv = CalcPoint(u, v + 0.01) - CalcPoint(u, v - 0.01);

	return glm::normalize(glm::cross(du, dv));
}

glm::vec3 VaoMesh::CalcPoint(float u, float v)
{
	return glm::vec3(u, 0, v);
}

void VaoMesh::draw()
{
	m_Vao.Bind();
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
}

