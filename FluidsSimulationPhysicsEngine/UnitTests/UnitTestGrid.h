#pragma once
#include <random>

class UnitTestGrid
{
	std::random_device randomDevice;
	std::mt19937 randomEngine;

public:
	UnitTestGrid();
	void run();
	~UnitTestGrid();
	void addRandomBodies();
};

