/*
 ============================================================================
 Name        : T-Rex.c
 Author      : Kenshiro
 Version     :
 Copyright   : 
 Description : Basic example to teach T-Rex the XOR truth table
 ============================================================================
 */
#include "logic_tier/NeuralNetwork.h"
#include "presentation_tier/ConsoleManager.h"

#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_INPUTS 2
#define NUMBER_OF_HIDDEN_LAYERS 1
#define NUMBER_OF_OUTPUTS 1

#define NUMBER_OF_TEST_CASES 4
#define TARGET_FITNESS_SCORE 4

const NeuronData xorInput[NUMBER_OF_TEST_CASES][NUMBER_OF_INPUTS] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
const NeuronData xorOutput[NUMBER_OF_TEST_CASES] = {0, 1, 1, 0};


NeuralNetworkErrorCode evaluateFitness(NeuralNetwork *myNeuralNetwork, int *fitnessScore)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	int xorInputIndex = 0;
	int score = 0;

	NeuronData *neuralNetworkOutput;
	int numberOfOutputs;

	if ((myNeuralNetwork==NULL) || (fitnessScore==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	while ((xorInputIndex<NUMBER_OF_TEST_CASES) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		int neuralNetworkInputIndex = 0;

		while ((neuralNetworkInputIndex<NUMBER_OF_INPUTS) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			NeuronData neuralNetworkInput = xorInput[xorInputIndex][neuralNetworkInputIndex];

			returnValue = setNeuralNetworkInput(myNeuralNetwork, neuralNetworkInputIndex, neuralNetworkInput);

			neuralNetworkInputIndex++;
		}

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = computeNeuralNetworkOutput(myNeuralNetwork, &neuralNetworkOutput, &numberOfOutputs);

		//Evaluate output
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			if (neuralNetworkOutput[0]==xorOutput[xorInputIndex])
				score++;
		}

		xorInputIndex++;
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		*fitnessScore = score;

	return returnValue;
}

NeuralNetworkErrorCode trainNeuralNetwork(NeuralNetwork **myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	NeuralNetwork *myNeuralNetworkClone;

	int generationNumber = 1;
	int myNeuralNetworkScore = 0;
	int myNeuralNetworkCloneScore = 0;

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = createNeuralNetwork(&myNeuralNetworkClone, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	while ((myNeuralNetworkScore<TARGET_FITNESS_SCORE) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		returnValue = evaluateFitness(*myNeuralNetwork, &myNeuralNetworkScore);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = evaluateFitness(myNeuralNetworkClone, &myNeuralNetworkCloneScore);

		//Set the best scoring neural network as the reference
		if (myNeuralNetworkCloneScore>myNeuralNetworkScore)
		{
			NeuralNetwork *auxNeuralNetwork = *myNeuralNetwork;
			*myNeuralNetwork = myNeuralNetworkClone;
			myNeuralNetworkClone = auxNeuralNetwork;

			myNeuralNetworkScore = myNeuralNetworkCloneScore;
		}

		printf("\nCurrent generation: %d, score: %d", generationNumber, myNeuralNetworkScore);

		//Clone the best neural network
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = cloneNeuralNetwork(*myNeuralNetwork, myNeuralNetworkClone);

		//Mutate the neural network clone
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = mutateNeuralNetwork(myNeuralNetworkClone);

		generationNumber++;
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myNeuralNetworkClone);

	return returnValue;
}

int main(void)
{
	printf("\n----- XOR EXAMPLE -----\n");

	NeuralNetwork *myNeuralNetwork;
	NeuronData *neuralNetworkOutput;
	int numberOfOutputs;
	int xorInputIndex = 0;

	NeuralNetworkErrorCode returnValue = createNeuralNetwork(&myNeuralNetwork, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = trainNeuralNetwork(&myNeuralNetwork);

	printf("\n\nPrint trained network results ...\n\n");

	//Print trained network results
	while ((xorInputIndex<NUMBER_OF_TEST_CASES) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		int neuralNetworkInputIndex = 0;

		while ((neuralNetworkInputIndex<NUMBER_OF_INPUTS) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			NeuronData neuralNetworkInput = xorInput[xorInputIndex][neuralNetworkInputIndex];

			returnValue = setNeuralNetworkInput(myNeuralNetwork, neuralNetworkInputIndex, neuralNetworkInput);

			neuralNetworkInputIndex++;
		}

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = computeNeuralNetworkOutput(myNeuralNetwork, &neuralNetworkOutput, &numberOfOutputs);

		printNeuralNetwork(myNeuralNetwork);

		xorInputIndex++;
	}

	returnValue = destroyNeuralNetwork(&myNeuralNetwork);

	return returnValue;
}