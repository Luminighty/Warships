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
		this->values = SRandom(0.8f, 2.0f, 10, 1);
	}

protected:

	virtual glm::vec3 CalcPoint(float u, float v) final {
		
		float x = u * size;
		float value = SDL_sin(u * 20) + 1;
		value *= values.GetValue(u);
		value += 1;
		float y = (value * v);
		if (u < 0.05f || u > 0.95f) {
			float per = ((u < 0.05f) ? u : 1 - u) / 0.05f;
			y = v * 2;
		}
		float z = (0.5f - v) * 10;
		return glm::vec3(x, y, z);
	};

private:
	float size;
	SRandom values;

};

