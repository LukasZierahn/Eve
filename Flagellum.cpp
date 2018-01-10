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

	//accel and max speed are influenced by the first two dna characters, max speed more by the first and accel more by the second
	accelSpeed = dna->GetCharacter(startpos) / 500.0f;
	maxSpeed = dna->GetCharacter(startpos + 1) / 100.0f;

	maxSpeed += dna->GetCharacter(startpos + 1) / 500.0f;
	accelSpeed += dna->GetCharacter(startpos + 1) / 150.0f;

	energyRequierement = pow(maxSpeed, 2) + pow(accelSpeed, 2);

	XNeuralNetNode = dna->GetCharacter(startpos + 2) % neuralNet->GetOutputLayerCount();
	YNeuralNetNode = dna->GetCharacter(startpos + 3) % neuralNet->GetOutputLayerCount();
	ZNeuralNetNode = dna->GetCharacter(startpos + 4) % neuralNet->GetOutputLayerCount();
}

float Flagellum::Tick(int t)
{
	XMFLOAT3 currentVel = *pCell->GetVelocity();

	float currentSpeed = sqrt(pow(currentVel.x, 2) + pow(currentVel.y, 2) + pow(currentVel.z, 2));

	currentVel.x += neuralNet->GetOutputNode(XNeuralNetNode) * t * accelSpeed * (maxSpeed / (currentSpeed + 1)) / (volume * 100.0f);
	currentVel.y += neuralNet->GetOutputNode(YNeuralNetNode) * t * accelSpeed * (maxSpeed / (currentSpeed + 1)) / (volume * 100.0f);
	currentVel.z += neuralNet->GetOutputNode(ZNeuralNetNode) * t * accelSpeed * (maxSpeed / (currentSpeed + 1)) / (volume * 100.0f);

	pCell->SetVelocity(&currentVel);

	return -1.0f * (neuralNet->GetOutputNode(XNeuralNetNode) + neuralNet->GetOutputNode(YNeuralNetNode) + neuralNet->GetOutputNode(ZNeuralNetNode)) * t * energyRequierement;
}

string Flagellum::GetOutputString()
{
	string buffer = " Flagellum\n";

	buffer += "  Max Speed: " + to_string(maxSpeed) + " \n  Accel Speed: " + to_string(accelSpeed) + " \n  Energy Requierement: " + to_string(energyRequierement);
	buffer += "\n  Output Nodes: " + to_string(XNeuralNetNode) + " / " + to_string(YNeuralNetNode) + " / " + to_string(ZNeuralNetNode) + "\n        "
		+ to_string(neuralNet->GetOutputNode(XNeuralNetNode)) +  " / " + to_string(neuralNet->GetOutputNode(YNeuralNetNode)) + " / " + to_string(neuralNet->GetOutputNode(ZNeuralNetNode)) + "\n";

	return buffer;
}


Flagellum::~Flagellum()
{
}
