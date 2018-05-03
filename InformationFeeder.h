#ifndef __H__InformationFeeder
#define __H__InformationFeeder

#include "NeuralNetworkInput.h"
#include "ChemicalContainer.h"
#include "Cell.h"

class Cell;
class DNA;
class NeuralNetwork;


//everything above these values will just be considered as 1.0f by the network
#define Max_Amount_Of_Substances 0.0001f
#define Max_ATP 2.0f

class InformationFeeder : public NeuralNetworkInput //this feeds the neural net of a cell with informations
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;

	//int temperatureInputNode = 0;
	int ATPInputNode = 0;
	int containsInputNode[contains_amount];

public:
	InformationFeeder(Cell* parentCell, DNA* dna, int startingPos);

	void InputValuesToNeuralNetwork();

	int GetType() { return Type_InformationFeeder; }

	~InformationFeeder();
};

#endif