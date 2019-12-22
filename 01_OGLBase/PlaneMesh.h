#pragma once
#include "MyMesh.h"
class PlaneMesh :public VaoMesh
{
public:
	PlaneMesh() : VaoMesh(100, 100) {}
protected:
	virtual glm::vec3 CalcPoint(float u, float v) final {
		return glm::vec3(v, 0, u);
	};
};

