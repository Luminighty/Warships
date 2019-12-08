#pragma once

#include "MyMesh.h"

#include <string.h>

class SphereMesh : public VaoMesh
{

public:
	static const char* meshName;
	SphereMesh() : VaoMesh(SphereMesh::meshName, 10, 10) {}
protected:
	virtual glm::vec3 CalcPoint(float u, float v) final {
		u *= float(2 * M_PI);
		v *= float(M_PI);
		float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
		float r = 2;

		return glm::vec3(r * cu * sv, r * cv, r * su * sv);
	};
};

const char* SphereMesh::meshName = "SPHERE";