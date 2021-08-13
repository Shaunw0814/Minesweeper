#pragma once
#include <random>
class Random
{
	static std::mt19937 random;
public:
	static int x;
	static int Int(int min, int max);

};

