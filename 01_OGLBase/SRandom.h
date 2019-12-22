#pragma once

#include <vector>
#include <utility> 

#include <SDL.h>

class SRandom
{
public:
	SRandom();
	float GetValue(float value);
	void SetMin(float);
	void SetMax(float);
	void SetLeftValue(float);
	void SetRightValue(float);
	void SetGravity(float);
	void Init(int);

private:
	float CalcDistance(float, float);
	typedef std::pair<float, float> Point;
	std::vector<Point> points;
	float gravity;
	float left;
	float right;
	float min;
	float max;
	float frand(float, float);

};

