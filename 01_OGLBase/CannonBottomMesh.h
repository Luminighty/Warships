#pragma once
#include "MyMesh.h"
class CannonBottomMesh : public VaoMesh
{
public:
	CannonBottomMesh(int N) : VaoMesh(N,0) {}
protected:

	virtual std::vector<Vertex> getVertices() final {
		std::vector<Vertex> vertices;

		for (int i = 0; i < N; i++)
		{
			float degree = i * (360.0f / N);
			float rad = degree * (M_PI / 180.0f);

			float x = SDL_sin(rad);
			float z = SDL_cos(rad);

			vertices.push_back({ {x, 0.5f ,z}, {x, 0.5f, z}, {x, z} });
			vertices.push_back({ {x, -0.5f ,z}, {x, -0.5f, z}, {x, z} });
		}

		return vertices;
	}

	virtual std::vector<int> getIndices() final {
		std::vector<int> indices;

		for (int i = 0; i < N; i++)
		{
			if (i >= 2) {
				// top
				indices.push_back(0);
				indices.push_back((i - 1) * 2);
				indices.push_back(i * 2);

				// bottom
				indices.push_back(i * 2 + 1);
				indices.push_back((i - 1) * 2 + 1);
				indices.push_back(1);
			}

			// side
			indices.push_back(i * 2);
			indices.push_back((i * 2 + 1) % (N*2));
			indices.push_back((i * 2 + 2) % (N*2));

			indices.push_back((i * 2 + 2) % (N*2));
			indices.push_back((i * 2 + 1) % (N*2));
			indices.push_back((i * 2 + 3) % (N*2));
		}

		return indices;
	}
};

