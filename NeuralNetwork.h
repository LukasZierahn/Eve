#ifndef __H__NeuralNetwork
#define __H__NeuralNetwork

#include "Include.h"
#include "World.h"

class DNA;

#define Max_Input_Nodes 5
#define Max_Hidden_Nodes 5
#define Max_Ouput_Nodes 5

#define ATP_Cost_Factor_NN 0.0001f

class NeuralNetwork
{
	int inputLayerCount = 0;
	float* inputLayerConstantValue;
	int* inputLayerInputSource;
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
	float GetOutputNode(int index) { return outputLayer[index]; } //#here
	//float GetOutputNode(int index) { return 1.0f; }
	float GetUnsignedOutputNode(int index) { return (outputLayer[index] + 1) / 2; }

	int GetHiddenLayerCount() { return hiddenLayerCount; }

	int GetInputLayerCount() { return inputLayerCount; }
	float GetInputNode(int index) { return inputLayer[index]; }

	void SetInputNodes(float* arr, int paramCount, bool compute);
	void SetInputNode(int pos, float value, int source = -1) { inputLayer[pos] = value; inputLayerInputSource[pos] = source; }
	float ComputeResult();

	void BuildFromDNA(DNA* dna, int startpos);

	void AddSourcesToCellInformation(CellInformation* cellInfo);

	~NeuralNetwork();
};

#endif