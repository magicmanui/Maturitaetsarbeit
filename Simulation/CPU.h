#pragma once
#include <vector>
#include "IO.h"
using namespace std;

class CPU
{
public:
	IO io;
	uint8_t a;
	uint8_t b;
	uint16_t counter;
	uint8_t clusterSelect;
	uint8_t flags;
	vector<uint8_t> RAM;
	int clock_speed;
	CPU();
	CPU(int);
	CPU(vector<uint8_t>);
	vector<int> step();

	
};

