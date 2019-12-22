#include "SRandom.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>

SRandom::SRandom()
{
	srand(time(NULL));
}

float SRandom::GetValue(float value)
{
	/*
	if (value <= 0.0f)
		return left;
	if (value >= 1.0f)
		return right;
	int beforePoint = 0;
	while (points[beforePoint].first > value)
		beforePoint++;

	Point before = points[beforePoint];
	Point next = points[beforePoint + 1];
	if (before.first == value)
		return before.second;

	float middle = (next.first - before.first) / 2.0f;
	int sign = (before.second < next.second) ? 1 : -1;
	if (middle > value) {
		// előzőtől a középig
		float x = value - before.first;
		float y = (x * gravity);
		y = sign * (y * y) + before.second;
		return y;
	}
	else {
		// Középtől a következőig
		float x = next.first - value;

		float y = (x * gravity);
		y = sign * (y * y) + next.second;
		return y;
	}
	*/

	if (value <= 0.0f)
		return left;
	if (value >= 1.0f)
		return right;

	int beforePoint = 0;
	while (points[beforePoint].first > value)
		beforePoint++;

	Point before = points[beforePoint];
	Point next = points[beforePoint + 1];
	float distance = next.first - before.first;

	return ((next.second - before.second) / distance) * (value - before.first);
}

void SRandom::SetMin(float min) { this->min = min; }

void SRandom::SetMax(float max) { this->max = max; }

void SRandom::SetLeftValue(float left) { this->left = left; }

void SRandom::SetRightValue(float right) { this->right = right; }

void SRandom::SetGravity(float gravity) { this->gravity = gravity;}

void SRandom::Init(int iteration)
{
	int precision = 10000000;
	points.push_back({ 0, left });
	points.push_back({ 0.5f, frand(min, max) });
	points.push_back({ 1, right });

	for (int i = 0; i < iteration; i++)
	{
		int size = points.size();
		for (int j = 1; j < points.size(); j+=2) {
			Point r = points[j];
			Point l = points[j - 1];
			float min = (r.second > l.second) ? l.second : r.second;
			float dif = SDL_fabs(r.second - l.second);

			float newValue = frand(min, min + dif);
			float newX = (r.first + l.first) / 2.0f;
			points.insert(points.begin() + j - 1, { newX, newValue });
		}
	}
	/*
	std::cout << "POINTS: " << std::endl;
	for (int i = 0; i < points.size(); i++)
	{
		std::cout << points[i].second << std::endl;
	}*/

	/*
	float x = 0;
	Point last = std::pair<float, float>(x, left);
	int precision = 1000;

	int difference = (max - min) * precision;
	points.push_back(last);
	std::cout << "INIT" << std::endl;
	while (true)
	{
		std::cout << last.first << "\t" << last.second << std::endl;
		float next = (rand() % difference / (float)precision) + min;
		float distance = CalcDistance(last.second, next);
		float newX = x + distance;
		std::cout << "newX: " << newX << std::endl;
		if (newX + CalcDistance(next, right) > 1)
			break;
		x = newX;
		last = std::pair<float, float>(newX, next);
		points.push_back(last);
	}

	points.push_back(std::pair<float, float>(1, right));
	*/
}

float SRandom::CalcDistance(float from, float to)
{
	float middle = SDL_fabs(to - from) / 2.0f;
	return (SDL_sqrt(middle) / gravity) * 2.0f;
}

float SRandom::frand(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}
