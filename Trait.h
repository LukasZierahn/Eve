#ifndef __H__Trait
#define __H__Trait

#include "include.h"

class Trait
{
public:
	Trait();

	virtual float Tick(int t) = 0;
	virtual string GetOutputString() = 0;

	virtual float GetATPBuildingCost() = 0;

	~Trait();
};

#endif