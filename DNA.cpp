#include "DNA.h"
#include "time.h"

DNA::DNA()
{
}

void DNA::GenerateRandomDNA(int size)
{
	DNAString = "";

	srand(time(NULL));

	for (int i = 0; i < size; i++)
	{
		DNAString.append({ static_cast<char> ((rand() % UniqCharsInDNA) + 32) });
		//DNAString.append({ static_cast<char> (94 + 32) });
	}
}


DNA::~DNA()
{
}
