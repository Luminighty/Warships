#pragma once

#include <vector>
#include <utility> 

#include <SDL.h>

class SRandom
{
public:
	SRandom();
	SRandom(float min, float max, int pointC, float edge);
	float GetValue(float value);
	void printValues(int c);

private:
	typedef std::pair<float, float> Point;
	std::vector<Point> points;
	float frand(float, float);
};

