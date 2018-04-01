#include "Flagellum.h"
#include "include.h"
#include "Cell.h"
#include "DNA.h"
#include "NeuralNetwork.h"
#include "ChemicalContainer.h"

Flagellum::Flagellum(Cell* parentCell, DNA* dna, int startpos) : pCell(parentCell)
{
	neuralNet = pCell->GetNeuralNetwork();

	volume = pCell->GetChemCon()->GetVolume();

	dna->SetCurrentPosition(startpos);

	float firstValue = dna->GetGeneFloat(0, 1);
	float secondValue = dna->GetGeneFloat(0, 1);

	//accel and max speed are influenced by the first two dna characters, max speed more by the first and accel more by the second
	accelSpeed = firstValue / 50.0f;
	maxSpeed = firstValue / 10.0f;

	maxSpeed += secondValue / 50.0f;
	accelSpeed += secondValue / 15.0f;

	energyRequierement = pow(maxSpeed + 1, 2) + pow(accelSpeed + 1, 2) - 2;
	surface = energyRequierement * 200;

	XNeuralNetNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());
	YNeuralNetNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());
	ZNeuralNetNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());
}

float Flagellum::Tick(int t)
{
	float modifier = 1.0f;
	float ATPCost = (abs(neuralNet->GetOutputNode(XNeuralNetNode)) + abs(neuralNet->GetOutputNode(YNeuralNetNode)) + abs(neuralNet->GetOutputNode(ZNeuralNetNode))) * t * energyRequierement;
	ATPCost = pCell->LimitATPUsage(ATPCost, surface, &modifier);

	XMFLOAT3 currentVel = *pCell->GetVelocity();

	float currentSpeed = sqrt(pow(currentVel.x, 2) + pow(currentVel.y, 2) + pow(currentVel.z, 2));

	currentVel.x += neuralNet->GetOutputNode(XNeuralNetNode) * t * accelSpeed * (maxSpeed / (currentSpeed + 1)) / (volume * 100.0f) * modifier;
	currentVel.y += neuralNet->GetOutputNode(YNeuralNetNode) * t * accelSpeed * (maxSpeed / (currentSpeed + 1)) / (volume * 100.0f) * modifier;
	currentVel.z += neuralNet->GetOutputNode(ZNeuralNetNode) * t * accelSpeed * (maxSpeed / (currentSpeed + 1)) / (volume * 100.0f) * modifier;

	pCell->SetVelocity(&currentVel);

	return -1.0f * ATPCost;
}

string Flagellum::GetOutputString()
{
	string buffer = " Flagellum\n";

	buffer += "  Max Speed: " + to_string(maxSpeed) + " \n"; 
	buffer += "  Accel Speed : " + to_string(accelSpeed) + " \n";
	buffer += "  Energy Requierement : " + to_string(energyRequierement) + "\n";
	buffer += "  Surface : " + to_string(surface) + "\n";
	buffer += "  Output Nodes: " + to_string(XNeuralNetNode) + " / " + to_string(YNeuralNetNode) + " / " + to_string(ZNeuralNetNode) + "\n        "
		+ to_string(neuralNet->GetOutputNode(XNeuralNetNode)) +  " / " + to_string(neuralNet->GetOutputNode(YNeuralNetNode)) + " / " + to_string(neuralNet->GetOutputNode(ZNeuralNetNode)) + "\n";

	return buffer;
}


Flagellum::~Flagellum()
{
}
