#ifndef __H__DNA
#define __H__DNA

#include "include.h"

#define UniqCharsInDNA 94

class DNA
{

	string DNAString = ""; //the string can contain any ASCII number or character that isnt a control character, 32-126

public:
	DNA();

	void GenerateRandomDNA(int size);

	string GetString() { return DNAString; }
	string GetString(int start, int end) { return DNAString.substr(start, end); }
	char GetCharacter (int pos) 
	{ 
		if (pos < DNAString.length())
		{
			return DNAString[pos] - 32;
		}
		else
		{
			return 0;
		}
	}

	~DNA();
};

#endif