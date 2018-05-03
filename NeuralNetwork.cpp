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

float NeuralNetwork::ComputeResult()
{
	for (int i = 0; i < hiddenLayerCount; i++)
	{
		hiddenLayer[i] = 0.0f;

		for (int j = 0; j < inputLayerCount; j++)
		{
			hiddenLayer[i] += inpToHiddenCon[j + i * inputLayerCount] * inputLayer[j];
		}
		hiddenLayer[i] = hiddenLayer[i] / (1.0f * inputLayerCount);
	}

	for (int i = 0; i < outputLayerCount; i++)
	{
		outputLayer[i] = 0.0f;
		for (int j = 0; j < hiddenLayerCount; j++)
		{
			outputLayer[i] += hiddenToOutCon[j + i * hiddenLayerCount] * hiddenLayer[j];
		}
		outputLayer[i] = outputLayer[i] / (1.0f * hiddenLayerCount);
	}

	return ATP_Cost_Factor_NN * (inpToHiddenConCount + hiddenToOutConCount);
}


void NeuralNetwork::BuildFromDNA(DNA* dna, int startpos)
{
	dna->SetCurrentPosition(startpos);

	inputLayerCount = dna->GetGeneInt(1, Max_Input_Nodes);
	inputLayer = new float[inputLayerCount];
	for (int i = 0; i < inputLayerCount; i++)
	{
		inputLayer[i] = 0.0f;
	}

	inputLayerConstantValue = new float[inputLayerCount];
	inputLayerInputSource = new int[inputLayerCount];
	for (int i = 0; i < inputLayerCount; i++)
	{
		inputLayerConstantValue[i] = dna->GetGeneFloatFromSingleChar(-1, 1);
		inputLayer[i] = inputLayerConstantValue[i];
		inputLayerInputSource[i] = -1;
	}


	outputLayerCount = dna->GetGeneInt(1, Max_Ouput_Nodes);
	outputLayer = new float[outputLayerCount];
	for (int i = 0; i < outputLayerCount; i++)
	{
		outputLayer[i] = 0.0f;
	}


	hiddenLayerCount = dna->GetGeneInt(1, Max_Hidden_Nodes);
	hiddenLayer = new float[hiddenLayerCount];
	for (int i = 0; i < hiddenLayerCount; i++)
	{
		hiddenLayer[i] = 0.0f;
	}


	inpToHiddenConCount = inputLayerCount * hiddenLayerCount;
	inpToHiddenCon = new float[inpToHiddenConCount];
	for (int i = 0; i < inpToHiddenConCount; i++)
	{
		inpToHiddenCon[i] = dna->GetGeneFloatFromSingleChar(-1, 1);
	}


	hiddenToOutConCount = hiddenLayerCount * outputLayerCount;
	hiddenToOutCon = new float[hiddenToOutConCount];
	for (int i = 0; i < hiddenToOutConCount; i++)
	{
		hiddenToOutCon[i] = dna->GetGeneFloatFromSingleChar(-1, 1);
	}
}

void NeuralNetwork::AddSourcesToCellInformation(CellInformation* cellInfo)
{
	for (int i = 0; i < inputLayerCount; i++)
	{
		if (inputLayerInputSource[i] != -1)
		{
			cellInfo->neuralNetworkSources[inputLayerInputSource[i]]++;
		}
	}
}

NeuralNetwork::~NeuralNetwork()
{
	delete[] inputLayer, hiddenLayer, outputLayer;
	delete[] inpToHiddenCon, hiddenToOutCon;
	delete[] inputLayerInputSource;
	delete[] inputLayerConstantValue;
}
