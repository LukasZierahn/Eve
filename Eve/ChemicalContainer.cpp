#include "ChemicalContainer.h"



ChemicalContainer::ChemicalContainer(int vol) : volume(vol)
{
	//all concentrations are in percent
	contains["Na"] = 1.925; //sodium cations
	contains["Cl"] = 1.071; //chloride anions
	contains["Sulfate"] = 0.2695; //Sulfate
}


ChemicalContainer::~ChemicalContainer()
{
}
