#include "stdafx.h"
#include "SinusoidalWave.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <math.h>

SinusoidalWave::SinusoidalWave()
{

}

SinusoidalWave::SinusoidalWave(double freq, double nominalVoltage, unsigned long int pps, double seconds, double inPhi)
{
	double maxVoltage = sqrt(2)*nominalVoltage;
	unsigned long int numPoints = (unsigned long int)ceil((double)pps*seconds);
	double *dataPtr = (double *)malloc(sizeof(double)*numPoints);
	double w = 2*M_PI*freq;
	double t;
	for (unsigned long int i = 0; i < numPoints; i++)
	{
		t = (double)i / (double)pps;
		dataPtr[i] = maxVoltage*sin(inPhi+w*t);
	}
	setSignalPtr(dataPtr);
	setLength(numPoints);
	setNominalValue(nominalVoltage);
	setPPS(pps);
	setFrequency(freq);
	setPhi(inPhi);
}


SinusoidalWave::~SinusoidalWave()
{
}
