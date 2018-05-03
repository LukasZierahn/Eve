#ifndef __H__NeuralNetworkInput
#define __H__NeuralNetworkInput



class NeuralNetworkInput
{
public:
	NeuralNetworkInput();

	virtual void InputValuesToNeuralNetwork() = 0;
	virtual int GetType() = 0;

	~NeuralNetworkInput();
};

#endif