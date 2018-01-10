#ifndef __H__Flagellum
#define __H__Flagellum

#include "Trait.h"
#include "Cell.h"

class DNA;
class NeuralNetwork;
class Cell;

class Flagellum : public Trait
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;

	//these are given in um per second and uq per second^2
	float accelSpeed = 0.0f;
	float maxSpeed = 0.0f;

	float volume = 0.0f;

	float energyRequierement = 0.0f;

	int XNeuralNetNode = 0;
	int YNeuralNetNode = 0;
	int ZNeuralNetNode = 0;

public:
	Flagellum(Cell* parentCell, DNA* dna, int startpos);

	void InputValuesToNeuralNetwork() {}

	float Tick(int t);
	string GetOutputString();
	float GetATPBuildingCost() { return energyRequierement  * 100.0f; }

	~Flagellum();
};

#endif