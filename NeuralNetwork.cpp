#include "NeuralNetwork.h"



NeuralNetwork::NeuralNetwork(int hiddenCount)
{
	inputLayerCount = Number_of_input_Nodes;
	inputLayer = new float[Number_of_input_Nodes];
	for (int i = 0; i < Number_of_input_Nodes; i++)
	{
		inputLayer[i] = 0.0f;
	}	
	
	hiddenLayerCount = hiddenCount;
	hiddenLayer = new float[hiddenCount];
	for (int i = 0; i < hiddenCount; i++)
	{
		hiddenLayer[i] = 0.0f;
	}

	outputLayerCount = Number_of_output_Nodes;
	outputLayer = new float[Number_of_output_Nodes];
	for (int i = 0; i < Number_of_output_Nodes; i++)
	{
		outputLayer[i] = 0.0f;
	}


	inpToHiddenConCount = Number_of_input_Nodes * hiddenCount;
	inpToHiddenCon = new float[inpToHiddenConCount];
	for (int i = 0; i < inpToHiddenConCount; i++)
	{
		inpToHiddenCon[i] = 0.0f;
	}

	hiddenToOutConCount = hiddenCount * Number_of_output_Nodes;
	hiddenToOutCon = new float[hiddenToOutConCount];
	for (int i = 0; i < hiddenToOutConCount; i++)
	{
		hiddenToOutCon[i] = 0.0f;
	}
}

void NeuralNetwork::SetInputNodes(float* arr, int paramCount)
{
	SetInputNodesMax = max(paramCount, Number_of_input_Nodes);
	for (int i = 0; i < SetInputNodesMax; i++)
	{
		inputLayer[i] = arr[i];
	}
}


NeuralNetwork::~NeuralNetwork()
{
	delete[] inputLayer, hiddenLayer, outputLayer;
	delete[] inpToHiddenCon, hiddenToOutCon;
}
