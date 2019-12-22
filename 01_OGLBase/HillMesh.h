#pragma once

#include "MyMesh.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "SRandom.h"

class HillMesh : public VaoMesh
{
public:
	HillMesh(float size) : VaoMesh(50, 50) {
		this->size = size;
		this->values = SRandom();
		
		values.SetGravity(20.0f);
		values.SetLeftValue(5.0f);
		values.SetRightValue(5.0f);
		values.SetMin(0.0f);
		values.SetMax(10.0f);
		
		values.Init(5);
	}

protected:

	virtual glm::vec3 CalcPoint(float u, float v) final {
		
		float x = u * size;
		float value = values.GetValue(u);
		float y = (value * v); // +(u * (1 - v));
		float z = 0;
		//std::cout << u << "\t" << v << std::endl;
		return glm::vec3(x, y, z);
	};

	/*
	virtual glm::vec3 CalcNorm(float u, float v) final {
		return glm::normalize(glm::vec3(u, v, 0));
	}*/

	/*
	virtual std::vector<Vertex> getVertices() final {
		std::vector<Vertex> vertices;
		for (int x = 0; x <= N; x++) {
			for (int y = 0; y <= M; y++) {
				float u = x / (float)N;
				float v = y / (float)M;

				vertices.push_back({ CalcPoint(u, v), CalcNorm(u, v) , CalcText(u, v) });
			}
		}
		return vertices;
	}*/

private:
	float size;
	SRandom values;

};

