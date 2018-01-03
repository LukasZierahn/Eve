#ifndef __H__NeuralNetwork
#define __H__NeuralNetwork

#include "Include.h"
class DNA;

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

	int InputNodesMax = 0;

public:
	NeuralNetwork();

	int GetOutputLayerCount() { return outputLayerCount; }
	float GetOutputNode(int index) { return outputLayer[index]; }

	int GetHiddenLayerCount() { return hiddenLayerCount; }

	int GetInputLayerCount() { return inputLayerCount; }
	float GetInputNode(int index) { return inputLayer[index]; }

	void SetInputNodes(float* arr, int paramCount, bool compute);
	void ComputeResult();

	void BuildFromDNA(DNA* dna);

	~NeuralNetwork();
};

#endif