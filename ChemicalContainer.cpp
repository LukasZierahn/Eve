#include "ChemicalContainer.h"
#include "World.h"

ChemicalContainer::ChemicalContainer(World* world, int vol) : world(world), volume(vol)
{
	//all of the substances are in mole * 10^-12 (1 mol = 6.022 * 10^23 atoms, 1 * 10^-12 mol = 6.022 * 10^11 atoms)
	//since the biggest part of a given chemical container is water, if not state differently, the volume is a close approximation to the weight
	//1 litre water ~ 1 kilo water, 1 um^3 ~ 10^-12 g
	//total mass / molar mass = mol
	contains["Na"] = (vol * sodium_cations_concentration) / sodium_cations_atomic_weigth; //sodium cations
	contains["Cl"] = (vol * chlorid_anions_concentration) / chlorid_anions_atomic_weigth; //chloride anions
	contains["Sulfate"] = (vol * sulfate_concentration) / sulfate_atomic_weigth; //Sulfate
}

void ChemicalContainer::DispatchPushMessagesToChunks(float t)
{
	float flow = world->GetChemConFlowSpeed();

	for (short i = 0; i < surroundingChunks.size(); i++)
	{
		ChemicalPush* chemPush = new ChemicalPush();
		chemPush->source = this;
		chemPush->target = surroundingChunks.at(i);
		chemPush->contains.clear();

		if (surroundingChunks.at(i) != nullptr)
		{
			for (map<string, float>::iterator it = contains.begin(); it != contains.end(); ++it)
			{
				//to approximate diffusion I will use, t ~ (d^2)/2D, where d is the distance and D is the Diffusion Coefficient
				//the average distance between two points in the two chunks is the size of a chunk
				double tNeeded = pow(world->GetChunkSize(), 2) / 2 * world->GetDiffusionCoefficient(it->first) * 1000;
				chemPush->contains[it->first] = (t / tNeeded) * world->GetChemConFlowSpeed() * it->second;
			}
		}

		world->AddChemPush(chemPush);
	}
}

ChemicalContainer::~ChemicalContainer()
{
}
