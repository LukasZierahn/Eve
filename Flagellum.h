#ifndef __H__Flagellum
#define __H__Flagellum

#define FlagellumCriteria "Fl" //Fl has to appear in the DNA for a Flagellum to be created

class DNA;
class NeuralNetwork;
class Cell;

class Flagellum
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;

	//these are given in um per second and uq per second^2
	float accelSpeed = 0.0f;
	float maxSpeed = 0.0f;

	float energyRequierement = 0.0f;

	int XNeuralNetNode = 0;
	int YNeuralNetNode = 0;
	int ZNeuralNetNode = 0;

public:
	Flagellum(Cell* parentCell, DNA* dna, int startpos);
	void Tick(int t);
	~Flagellum();
};

#endif