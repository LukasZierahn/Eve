#include "ChemicalContainer.h"
#include "World.h"

ChemicalContainer::ChemicalContainer(World* world, int vol) : world(world), volume(vol)
{
	//all concentrations are in percent
	contains["Na"] = 1.925; //sodium cations
	contains["Cl"] = 1.071; //chloride anions
	contains["Sulfate"] = 0.2695; //Sulfate
}

void ChemicalContainer::DispatchPushMessagesToChunks(int t)
{
	float flow = world->GetChemConFlowSpeed();
	
	ChemicalPush chemPush;
	chemPush.source = this;
	chemPush.target = surroundingChunks.at(0);

	for (short i = 0; i < surroundingChunks.size(); i++)
	{
		if (surroundingChunks.at(i) != nullptr)
		{
			map<string, float>* targetContains = surroundingChunks.at(i)->GetContains();
			for (map<string, float>::iterator it = targetContains->begin(); it != targetContains->end(); ++it)
			{
				if (it->second != contains.at(it->first))
				{

				}
			}
		}
	}
}

ChemicalContainer::~ChemicalContainer()
{
}
