#include "Flagellum.h"
#include "include.h"
#include "Cell.h"
#include "DNA.h"
#include "NeuralNetwork.h"


Flagellum::Flagellum(Cell* parentCell, DNA* dna, int startpos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();

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

void Flagellum::Tick(int t)
{
	XMFLOAT3 currentVel = *pCell->GetVelocity();

	float currentSpeed = sqrt(pow(currentVel.x, 2) + pow(currentVel.y, 2) + pow(currentVel.z, 2));

	currentVel.x += neuralNet->GetOutputNode(XNeuralNetNode) * t / 500000.0f * accelSpeed * (maxSpeed / (currentSpeed + 1));
	currentVel.y += neuralNet->GetOutputNode(YNeuralNetNode) * t / 500000.0f * accelSpeed * (maxSpeed / (currentSpeed + 1));
	currentVel.z += neuralNet->GetOutputNode(ZNeuralNetNode) * t / 500000.0f * accelSpeed * (maxSpeed / (currentSpeed + 1));

	pCell->SetVelocity(&currentVel);
}


Flagellum::~Flagellum()
{
}
