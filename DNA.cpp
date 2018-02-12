#include "DNA.h"

DNA::DNA()
{

}

DNA::DNA(string InpDNA)
{
	DNAString = InpDNA;
	DNALength = DNAString.size();
}

void DNA::GenerateRandomDNA(int size)
{
	DNAString = "";

	for (int i = 0; i < size; i++)
	{
		DNAString.append({ static_cast<char> ((rand() % UniqCharsInDNA) + 32) });
		//DNAString.append({ static_cast<char> (94 + 32) });
	}

	DNALength = DNAString.size();
}

DNA* DNA::CloneDNA()
{
	string ClonedDNAString = "";

	SetCurrentPosition(DNAString.size() - 12);

	int changingChance = GetGeneInt(1, 100);
	int addingChance = GetGeneInt(1, 100);
	int removingChance = GetGeneInt(1, 100);

	for (char i : DNAString)
	{
		if (rand() % changingChance == 0)
		{
			ClonedDNAString.append({ static_cast<char> min(max(i + (rand() % 20) - 10, 32), UniqCharsInDNA + 32) });
			continue;
		}

		if (rand() % addingChance == 0) // if (!rand() % addingChance) would have also been possible but i think this is more readable
		{
			ClonedDNAString.append({ i });
			ClonedDNAString.append({ static_cast<char> ((rand() % UniqCharsInDNA) + 32) });
			continue;
		}

		//yes removing is more unlikely than adding or changing (because its getting checked last) but i did this on purpose so that DNA generally grows, to help in cell development in early stages
		if (!rand() % removingChance == 0)
		{
			ClonedDNAString.append({ i });
		}

	}

	return new DNA(ClonedDNAString);
}

float DNA::GetGeneFloat(float min, float max)
{
	currentPos += 4;

	return (((GetCharacter((currentPos - 4) % DNALength) + GetCharacter((currentPos - 3) % DNALength) + GetCharacter((currentPos - 2) % DNALength) + GetCharacter((currentPos - 1) % DNALength)) * (max - min)) / (UniqCharsInDNA * 4)) + min;}

float DNA::GetGeneFloatFromSingleChar(float min, float max)
{
	currentPos++;
	
	return (GetCharacter((currentPos - 1) % DNALength) * (max - min) / UniqCharsInDNA) + min;
}

int DNA::GetGeneInt(int min, int max)
{
	currentPos += 4;

	return (((GetCharacter((currentPos - 4) % DNALength) + GetCharacter((currentPos - 3) % DNALength) + GetCharacter((currentPos - 2) % DNALength) + GetCharacter((currentPos - 1) % DNALength)) * (max - min)) / (UniqCharsInDNA * 4)) + min;
}

DNA::~DNA()
{
}
