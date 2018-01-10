#include "DNA.h"

DNA::DNA()
{

}

DNA::DNA(string InpDNA)
{
	DNAString = InpDNA;
}

void DNA::GenerateRandomDNA(int size)
{
	DNAString = "Me";

	for (int i = 0; i < size; i++)
	{
		DNAString.append({ static_cast<char> ((rand() % UniqCharsInDNA) + 32) });
		//DNAString.append({ static_cast<char> (94 + 32) });
	}
}

DNA* DNA::CloneDNA()
{
	string ClonedDNAString = "";

	int changingChance = DNAString[DNAString.size() - 1];
	int addingChance = DNAString[DNAString.size() - 2];
	int removingChance = DNAString[DNAString.size() - 3];

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


DNA::~DNA()
{
}
