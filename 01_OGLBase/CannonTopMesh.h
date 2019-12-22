#pragma once
#include "MyMesh.h"
class CannonTopMesh : public VaoMesh
{
public:
	CannonTopMesh() : VaoMesh(20, 20) {}
private:

protected:
	virtual glm::vec3 CalcPoint(float u, float v) {

		float degree = u * 360.0f;
		float rad = degree * (M_PI / 180.0f);

		float x = SDL_sin(rad);
		float z = SDL_cos(rad);
		return glm::vec3(x, v, z);
	}

	virtual std::vector<int> getIndices() {
		std::vector<int> indices = VaoMesh::getIndices();
		
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				indices.push_back((i)+(j) * (N + 1));
				indices.push_back((i + 1) + (j) * (N + 1));
				indices.push_back((i)+(j + 1) * (N + 1));

				indices.push_back((i + 1) + (j) * (N + 1));
				indices.push_back((i + 1) + (j + 1) * (N + 1));
				indices.push_back((i)+(j + 1) * (N + 1));
			}
		}

		return indices;
	}

};

