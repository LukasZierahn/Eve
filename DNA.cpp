#include "DNA.h"

DNA::DNA()
{

}

DNA::DNA(string InpDNA)
{
	DNAString = InpDNA;
	DNALength = DNAString.size();

	SetCurrentPosition(DNAString.size() - 12);

	changingChance = GetGeneInt(1, 80);
	addingChance = GetGeneInt(1, 80);
	removingChance = GetGeneInt(1, 80);
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

	SetCurrentPosition(DNAString.size() - 12);

	changingChance = GetGeneInt(1, 80);
	addingChance = GetGeneInt(1, 80);
	removingChance = GetGeneInt(1, 80);
}

DNA* DNA::CloneDNA()
{
	string ClonedDNAString = "";

	SetCurrentPosition(DNAString.size() - 12);

	changingChance = GetGeneInt(1, 80);
	addingChance = GetGeneInt(1, 80);
	removingChance = GetGeneInt(1, 80);

	for (char i : DNAString)
	{
		if (rand() % changingChance == 0)
		{
			ClonedDNAString.append({ static_cast<char> min(max(i + (rand() % 20) - 10, 32), UniqCharsInDNA + 32) });
		}

		if (rand() % addingChance == 0) // if (!rand() % addingChance) would have also been possible but i think this is more readable
		{
			ClonedDNAString.append({ i });
			ClonedDNAString.append({ static_cast<char> ((rand() % UniqCharsInDNA) + 32) });
		}

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

	return (((GetCharacter((currentPos - 4) % DNALength) + GetCharacter((currentPos - 3) % DNALength) + GetCharacter((currentPos - 2) % DNALength) + GetCharacter((currentPos - 1) % DNALength)) * (max - min)) / (UniqCharsInDNA * 4)) + min;
}

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

bool DNA::MutateDNAThroughPoison(float poison)
{
	if (poison <= 0.0001)
		return false;

	if (poison >= 1000.0f)
		return false;

	SetCurrentPosition(DNAString.size() - 12);

	string ClonedDNAString = "";

	changingChance = GetGeneInt(1, 100);
	addingChance = GetGeneInt(1, 100);
	removingChance = GetGeneInt(1, 100);

	bool change = false;

	for (char i : DNAString)
	{
		if (rand() % max(1, static_cast<int>((changingChance * 10000.0f) / poison)) == 0)
		{
			ClonedDNAString.append({ static_cast<char> min(max(i + (rand() % 20) - 10, 32), UniqCharsInDNA + 32) });
			change = true;
			continue;
		}

		if (rand() % max(1, static_cast<int>((addingChance * 10000.0f) / poison)) == 0)
		{
			ClonedDNAString.append({ i });
			ClonedDNAString.append({ static_cast<char> ((rand() % UniqCharsInDNA) + 32) });
			change = true;
			continue;
		}

		if (rand() % max(1, static_cast<int>((removingChance * 10000.0f) / poison)) == 0)
		{
			change = true;
		}
		else
		{
			ClonedDNAString.append({ i });
		}

	}

	DNAString = ClonedDNAString;
	DNALength = ClonedDNAString.size();

	return change;
}

DNA::~DNA()
{
}
