#include "stdafx.h"
#include "Noise.h"
#include <random>


Noise::Noise(noiseType nType, Signal parentSignal)
{
	//This function creates a noise with default parameters over all the signal
	switch (nType)
	{
	case WHITE_NOISE:
		 this->setSignalPtr(createWhiteNoise(parentSignal, 0, 5/*default stddev*/));
		 break;
	default:
		break;
	}
	this->setLength(parentSignal.getLength());
}

double* Noise::createWhiteNoise(Signal parentSignal, double mean, double stddev)
{
	//Creates a white noise with mean and stddev params
	double *tmp = (double *)malloc(sizeof(double)*parentSignal.getLength());
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(mean, stddev);
	for (unsigned long int i = 0; i < parentSignal.getLength(); i++)
		tmp[i] = distribution(generator);
	return tmp;
}


Noise::~Noise()
{
}
