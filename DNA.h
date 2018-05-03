#ifndef __H__DNA
#define __H__DNA

#include "include.h"

#define UniqCharsInDNA 94

class DNA
{

	string DNAString = ""; //the string can contain any ASCII character that isnt a control character, 32-126
	int DNALength = 0;
	int currentPos = 0;

	int changingChance;
	int addingChance;
	int removingChance;

public:
	DNA();
	DNA(string InpDNA);

	void GenerateRandomDNA(int size);
	DNA* CloneDNA();

	void SetCurrentPosition(int newPos) { currentPos = newPos; }
	float GetGeneFloat(float min, float max);
	float GetGeneFloatFromSingleChar(float min, float max);
	int GetGeneInt(int min, int max);

	int GetChangingChance() { return changingChance; }
	int GetAddingChance() { return addingChance; }
	int GetRemovingChance() { return removingChance; }

	string GetString() { return DNAString; }
	string GetString(int start, int end) { return DNAString.substr(start, end); }
	char GetCharacter (int pos) 
	{
		return DNAString[pos % DNALength] - 32;
	}

	bool MutateDNAThroughPoison(float poison);

	~DNA();
};

#endif