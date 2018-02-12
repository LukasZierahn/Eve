#ifndef __H__NeuralNetwork
#define __H__NeuralNetwork

#include "Include.h"
class DNA;

class NeuralNetwork
{
	int inputLayerCount = 0;
	float* inputLayerConstantValue;
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
	float GetUnsignedOutputNode(int index) { return (outputLayer[index] + 1) / 2; }

	int GetHiddenLayerCount() { return hiddenLayerCount; }

	int GetInputLayerCount() { return inputLayerCount; }
	float GetInputNode(int index) { return inputLayer[index]; }

	void SetInputNodes(float* arr, int paramCount, bool compute);
	void SetInputNode(int pos, float value) { inputLayer[pos] = value; }
	void ComputeResult();

	void BuildFromDNA(DNA* dna, int startpos);

	~NeuralNetwork();
};

#endif