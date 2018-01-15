#include "ChemicalContainer.h"
#include "Membrane.h"
#include "World.h"

//these are given in um/s^2
const float ChemicalContainer::diffusionCoefficients[] = { 1.0f, 1.0f };

ChemicalContainer::ChemicalContainer(World* world, float vol, float sA, ChemicalContainer* splittingFrom) :  world(world), volume(vol), surfaceArea(sA)
{

	if (splittingFrom == nullptr)
	{
		contains[0] = 10;
		contains[1] = 10;

		temperature = 20.0f;

		for (int i = 0; i < contains_amount; i++)
		{
			containsBuffer[i] = contains[i];
		}
	}
	else
	{
		temperature = splittingFrom->temperature;

		float volumeRelation = 1 / volume * splittingFrom->volume;
		for (int i = 0; i < contains_amount; i++)
		{
			contains[i] = splittingFrom->contains[i] * volume * volumeRelation;
			splittingFrom->contains[i] = splittingFrom->contains[i] * volumeRelation * splittingFrom->volume;
		}
	}

}

void ChemicalContainer::DiffuseToNeighbouringChunks(float t)
{
	for (short i = 0; i < amountOfNeightbouringChunks; i++)
	{
		DiffuseFromAndTo(surroundingChunks[i], t);
	}
}

void ChemicalContainer::DiffuseFromAndTo(ChemicalContainer* target, float t)
{
	float flow = world->GetChemConFlowSpeed();
	int chunkSize = world->GetChunkSize();

	target->temperature += (t / TemperatureDiffusionCoefficient) * flow * (temperature - target->temperature);
	temperature += (t / TemperatureDiffusionCoefficient) * flow * (target->temperature - temperature);

	for (int j = 0; j < contains_amount; j++)
	{
		//to approximate diffusion I will use, t ~ (d^2)/2D, where d is the distance and D is the Diffusion Coefficient
		//the average distance between two points in the two chunks is the size of a chunk
		target->containsBuffer[j] += t * flow * ((contains[j] / volume) - (target->contains[j] / target->volume)) * chunkSize * diffusionCoefficients[j];
		containsBuffer[j] += t * flow * ((target->contains[j] / target->volume) - (contains[j] / volume)) * chunkSize * diffusionCoefficients[j];
	}
}

void ChemicalContainer::DiffuseFromAndTo(ChemicalContainer* target, float t, float surfaceArea)
{
	float flow = world->GetChemConFlowSpeed();
	int chunkSize = world->GetChunkSize();

	target->temperature += (t / TemperatureDiffusionCoefficient) * flow * (temperature - target->temperature);
	temperature += (t / TemperatureDiffusionCoefficient) * flow * (target->temperature - temperature);

	for (int j = 0; j < contains_amount; j++)
	{
		//to approximate diffusion I will use, t ~ (d^2)/2D, where d is the distance and D is the Diffusion Coefficient
		//the average distance between two points in the two chunks is the size of a chunk
		target->containsBuffer[j] += t * flow * ((contains[j] / volume) - (target->contains[j] / target->volume)) * chunkSize * diffusionCoefficients[j] * (surfaceArea / chunkSize);
		containsBuffer[j] += t * flow * ((target->contains[j] / target->volume) - (contains[j] / volume)) * chunkSize * diffusionCoefficients[j] * (surfaceArea / chunkSize);
	}
}

float ChemicalContainer::DiffuseFromAndTo(ChemicalContainer* target, float t, Membrane* membrane)
{
	float totalATPCost = 0.0f;

	float flow = world->GetChemConFlowSpeed();
	int chunkSize = world->GetChunkSize();

	target->temperature += (t / TemperatureDiffusionCoefficient) * flow * (temperature - target->temperature);
	temperature += (t / TemperatureDiffusionCoefficient) * flow * (target->temperature - temperature);

	for (int j = 0; j < contains_amount; j++)
	{
		if (membrane->GetOutputNode(j) > 0)
		{
			float movement = min(1, containsBuffer[j] / 3.0f) * t * membrane->GetModifierCellToChunk(j) * abs(membrane->GetOutputNode(j)) * membrane->GetSurfaceArea() / 50;

			target->containsBuffer[j] += movement;
			containsBuffer[j] -= movement;

			totalATPCost += ATPCostToMoveSubstances[j] * abs(movement);
		}
		else
		{
			float movement = min(1, target->containsBuffer[j] / 3.0f) * t * membrane->GetModifierChunkToCell(j) * abs(membrane->GetOutputNode(j)) * membrane->GetSurfaceArea() / 50;

			target->containsBuffer[j] -= movement;
			containsBuffer[j] += movement;

			totalATPCost += ATPCostToMoveSubstances[j] * abs(movement);
		}
	}

	return -1.0f * totalATPCost;
}

float ChemicalContainer::GetSwellAmount(ChemicalContainer* target)
{
	float returnValue = 0.0f;

	for (int i = 0; i < contains_amount; i++)
	{
		returnValue = ((contains[i] / volume) - (target->contains[i] / target->volume));
	}

	return returnValue;
}


ChemicalContainer::~ChemicalContainer()
{
}
