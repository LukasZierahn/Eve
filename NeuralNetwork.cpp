#include "NeuralNetwork.h"
#include "DNA.h"

NeuralNetwork::NeuralNetwork()
{
}


void NeuralNetwork::SetInputNodes(float* arr, int paramCount, bool compute)
{
	InputNodesMax = max(paramCount, inputLayerCount);
	for (int i = 0; i < InputNodesMax; i++)
	{
		inputLayer[i] = arr[i];
	}

	if (compute)
	{
		ComputeResult();
	}
}

void NeuralNetwork::ComputeResult()
{
	for (int i = 0; i < hiddenLayerCount; i++)
	{
		hiddenLayer[i] = 0.0f;
		for (int j = 0; j < inputLayerCount; j++)
		{
			hiddenLayer[i] += inpToHiddenCon[j + i * inputLayerCount] * inputLayer[j];
		}
		hiddenLayer[i] = hiddenLayer[i] / inputLayerCount;
	}

	for (int i = 0; i < outputLayerCount; i++)
	{
		outputLayer[i] = 0.0f;
		for (int j = 0; j < hiddenLayerCount; j++)
		{
			outputLayer[i] += hiddenToOutCon[j + i * hiddenLayerCount] * hiddenLayer[j];
		}
		outputLayer[i] = outputLayer[i] / hiddenLayerCount;
	}
}


void NeuralNetwork::BuildFromDNA(DNA* dna, int startpos)
{

	inputLayerCount = max(dna->GetCharacter(startpos) / 3, 1);
	inputLayer = new float[inputLayerCount];
	for (int i = 0; i < inputLayerCount; i++)
	{
		inputLayer[i] = 0.0f;
	}

	outputLayerCount = max(dna->GetCharacter(startpos + 1) / 3, 1);
	outputLayer = new float[outputLayerCount];
	for (int i = 0; i < outputLayerCount; i++)
	{
		outputLayer[i] = 0.0f;
	}


	hiddenLayerCount = dna->GetCharacter(startpos + 2) / 3;
	hiddenLayer = new float[hiddenLayerCount];
	for (int i = 0; i < hiddenLayerCount; i++)
	{
		hiddenLayer[i] = 0.0f;
	}


	inpToHiddenConCount = inputLayerCount * hiddenLayerCount;
	inpToHiddenCon = new float[inpToHiddenConCount];
	for (int i = 0; i < inpToHiddenConCount; i++)
	{
		inpToHiddenCon[i] = dna->GetCharacter(i + startpos + 3) * 1.0f / UniqCharsInDNA;
	}


	hiddenToOutConCount = hiddenLayerCount * outputLayerCount;
	hiddenToOutCon = new float[hiddenToOutConCount];
	for (int i = 0; i < hiddenToOutConCount; i++)
	{
		hiddenToOutCon[i] = dna->GetCharacter(i + startpos + 4 + inpToHiddenConCount) * 1.0f / UniqCharsInDNA;
	}
}

NeuralNetwork::~NeuralNetwork()
{
	delete[] inputLayer, hiddenLayer, outputLayer;
	delete[] inpToHiddenCon, hiddenToOutCon;
}
