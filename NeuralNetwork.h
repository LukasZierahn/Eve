#ifndef __H__NeuralNetwork
#define __H__NeuralNetwork

#include "Include.h"

#define Number_of_input_Nodes 6
#define Number_of_output_Nodes 2

class NeuralNetwork
{
	int inputLayerCount = 0;
	float* inputLayer;

	int hiddenLayerCount = 0;
	float* hiddenLayer;

	int outputLayerCount = 0;
	float* outputLayer;

	int inpToHiddenConCount = 0;
	float* inpToHiddenCon;

	int hiddenToOutConCount = 0;
	float* hiddenToOutCon;

	int SetInputNodesMax = 0;
public:
	NeuralNetwork(int hiddenCount);

	void SetInputNodes(float* arr, int paramCount);
	float GetOutputNode(int index) { return outputLayer[index]; }

	~NeuralNetwork();
};

#endif