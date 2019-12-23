#include "SRandom.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>

SRandom::SRandom()
{
}

SRandom::SRandom(float min, float max, int pointC, float edge)
{

	points.push_back({ 0, edge });
	for (int i = 1; i < pointC; i++) {
		float x = i / (float)pointC;
		float y = frand(min, max);
		points.push_back({ x, y });
		std::cout << "x: " << x << "; y: " << y << std::endl;
	}
	points.push_back({ 1, edge });
	printValues(20);
}

float SRandom::GetValue(float value)
{
	/*
	if (value <= 0.0f || value >= 1.0f)
		return points[0].second;
		*/
	int i = 0;
	while (i < points.size() && value >= points[i].first)
		i++;
	if (i == points.size())
		return points[0].second;
	if (i == 0)
		return points[0].second;
	Point before = points[i-1];
	Point after = points[i];
	Point dif = { after.first - before.first, after.second - before.second };
	float m = dif.second / dif.first;

	float y = m * (value - before.first) + before.second;
	//std::cout << "res: " << res << std::endl;
	return y;
}

void SRandom::printValues(int c)
{
	std::cout << "SRandom::printValues(" << c << ")" << std::endl;
	for (int i = 0; i <= c; i++) {
		float x = i / (float)c;
		std::cout << "x: " << x << "; y: " << GetValue(x) << std::endl;
	}
}

float SRandom::frand(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}
