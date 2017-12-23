#include "ChemicalContainer.h"
#include "World.h"

//these are given in um/s^2
//sources: http://www.physiologyweb.com/calculators/diffusion_time_calculator.html, http://www.academia.edu/13131731/The_diffusion_coefficients_of_sulfate_ammonium_and_phosphate_ions_in_anoxic_marine_sediments1
const float ChemicalContainer::diffusionCoefficients[] = { 0.197, 0.05, 0.133, 0.203, 0.05 };

ChemicalContainer::ChemicalContainer(World* world, int vol) :  world(world), volume(vol)
{
	temperature = 20.0f;

	//all of the substances are in mole * 10^-12 (1 mol = 6.022 * 10^23 atoms, 1 * 10^-12 mol = 6.022 * 10^11 atoms)
	//since the biggest part of a given chemical container is water, if not state differently, the volume is a close approximation to the weight
	//1 litre water ~ 1 kilo water, 1 um^3 ~ 10^-12 g
	//total mass / molar mass = mol
	contains[CO2] = (vol * CO2_concentration) / CO2_atomic_weigth;
	contains[glucose] = (vol * glucose_concentration) / glucose_atomic_weigth;
	contains[sodium_cations] = (vol * sodium_cations_concentration) / sodium_cations_atomic_weigth; //sodium cations
	contains[chlorid_anions] = (vol * chlorid_anions_concentration) / chlorid_anions_atomic_weigth; //chloride anions
	contains[sulfate] = (vol * sulfate_concentration) / sulfate_atomic_weigth; //Sulfate
	contains[water] = (vol * 0.9f) / water_atomic_weigth;

	for (int i = 0; i < number_of_substances; i++)
	{
		containsBuffer[i] = contains[i];
	}

	for (int i = 0; i < number_of_substances; i++)
	{
		diffusionTimes[i] = pow(world->GetChunkSize(), 2) / 2 * diffusionCoefficients[i] * 1000;
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

	//target->temperature += (t / diffusionTimes[j]) * flow * (temperature - target->temperature);
	//temperature += (t / diffusionTimes[j]) * flow * (target->temperature - temperature);

	for (int j = 0; j < number_of_substances; j++)
	{
		//to approximate diffusion I will use, t ~ (d^2)/2D, where d is the distance and D is the Diffusion Coefficient
		//the average distance between two points in the two chunks is the size of a chunk
		target->containsBuffer[j] += (t / diffusionTimes[j]) * flow * ((contains[j] / volume) - (target->contains[j] / target->volume)) * chunkSize;
		containsBuffer[j] += (t / diffusionTimes[j]) * flow * ((target->contains[j] / target->volume) - (contains[j] / volume)) * chunkSize;
	}
}

void ChemicalContainer::DoReactions(float t, bool apply)
{
	//we will use the contains before anything has been flowing over or away

	double newGlucose = contains[water] * contains[CO2] * temperature * water_CO2_to_glucose;

	if (apply)
	{
		ApplyContains();
	}
}

ChemicalContainer::~ChemicalContainer()
{
}
