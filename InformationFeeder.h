#ifndef __H__InformationFeeder
#define __H__InformationFeeder

#include "NeuralNetworkInput.h"
#include "ChemicalContainer.h"

class Cell;
class DNA;
class NeuralNetwork;

//everything above these values will just be considered as 1.0f by the network
#define Max_Amount_Of_Substances 10.0f
#define Max_Temperature 50.0f
#define Max_ATP 50.0f

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

	~InformationFeeder();
};

#endif