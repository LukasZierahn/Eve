#ifndef __H__NeuralNetworkInput
#define __H__NeuralNetworkInput



class NeuralNetworkInput
{
public:
	NeuralNetworkInput();

	virtual void InputValuesToNeuralNetwork() = 0;

	~NeuralNetworkInput();
};

#endif