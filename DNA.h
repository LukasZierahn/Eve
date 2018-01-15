#ifndef __H__DNA
#define __H__DNA

#include "include.h"

#define UniqCharsInDNA 94

class DNA
{

	string DNAString = ""; //the string can contain any ASCII character that isnt a control character, 32-126
	int DNALength = 0;
	int currentPos = 0;

public:
	DNA();
	DNA(string InpDNA);

	void GenerateRandomDNA(int size);
	DNA* CloneDNA();

	void SetCurrentPosition(int newPos) { currentPos = newPos; }
	float GetGeneFloat(float min, float max);
	float GetGeneFloatFromSingleChar(float min, float max);
	int GetGeneInt(int min, int max);

	string GetString() { return DNAString; }
	string GetString(int start, int end) { return DNAString.substr(start, end); }
	char GetCharacter (int pos) 
	{
		return DNAString[pos % DNALength] - 32;
	}

	~DNA();
};

#endif