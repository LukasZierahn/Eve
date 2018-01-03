#include "ChemicalContainer.h"
#include "World.h"

//these are given in um/s^2
//sources: http://www.physiologyweb.com/calculators/diffusion_time_calculator.html, http://www.academia.edu/13131731/The_diffusion_coefficients_of_sulfate_ammonium_and_phosphate_ions_in_anoxic_marine_sediments1
const float ChemicalContainer::diffusionCoefficients[] = { 1.0f, 1.0f };

ChemicalContainer::ChemicalContainer(World* world, int vol) :  world(world), volume(vol)
{

	contains[0] = 10;
	contains[1] = 10;

	temperature = 20.0f;

	for (int i = 0; i < contains_amount; i++)
	{
		containsBuffer[i] = contains[i];
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


ChemicalContainer::~ChemicalContainer()
{
}
