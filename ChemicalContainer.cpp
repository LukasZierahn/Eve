#include "ChemicalContainer.h"
#include "World.h"

//these are given in um/s^2
//sources: http://www.physiologyweb.com/calculators/diffusion_time_calculator.html, http://www.academia.edu/13131731/The_diffusion_coefficients_of_sulfate_ammonium_and_phosphate_ions_in_anoxic_marine_sediments1
const float ChemicalContainer::diffusionCoefficients[] = { 0.133, 0.203, 0.05 };

ChemicalContainer::ChemicalContainer(World* world, int vol) :  world(world), volume(vol)
{
	//all of the substances are in mole * 10^-12 (1 mol = 6.022 * 10^23 atoms, 1 * 10^-12 mol = 6.022 * 10^11 atoms)
	//since the biggest part of a given chemical container is water, if not state differently, the volume is a close approximation to the weight
	//1 litre water ~ 1 kilo water, 1 um^3 ~ 10^-12 g
	//total mass / molar mass = mol
	contains[sodium_cations] = (vol * sodium_cations_concentration) / sodium_cations_atomic_weigth; //sodium cations
	contains[chlorid_anions] = (vol * chlorid_anions_concentration) / chlorid_anions_atomic_weigth; //chloride anions
	contains[sulfate] = (vol * sulfate_concentration) / sulfate_atomic_weigth; //Sulfate

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
	float flow = world->GetChemConFlowSpeed();

	for (short i = 0; i < surroundingChunks.size(); i++)
	{

		if (surroundingChunks.at(i) != nullptr)
		{
			for (int i = 0; i < number_of_substances; i++)
			{
				//to approximate diffusion I will use, t ~ (d^2)/2D, where d is the distance and D is the Diffusion Coefficient
				//the average distance between two points in the two chunks is the size of a chunk
				surroundingChunks.at(i)->containsBuffer[i] += (t / diffusionTimes[i]) * world->GetChemConFlowSpeed() * contains[i];
				containsBuffer[i] -= (t / diffusionTimes[i]) * world->GetChemConFlowSpeed() * contains[i];
			}
		}
	}
}

ChemicalContainer::~ChemicalContainer()
{
}
