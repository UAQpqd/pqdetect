#include "stdafx.h"
#include "SinusoidalWave.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <math.h>

SinusoidalWave::SinusoidalWave()
{

}
SinusoidalWave::SinusoidalWave(double freq, double voltage, unsigned long int pps, double seconds, double offset)
{
	double maxVoltage = sqrt(2)*voltage;
	unsigned long int numPoints = (unsigned long int)ceil((double)pps*seconds);
	double *dataPtr = (double *)malloc(sizeof(double)*numPoints);
	double w = 2 * M_PI*freq;
	double t;
	for (unsigned long int i = 0; i < numPoints; i++)
	{
		t = (double)i/pps;
		dataPtr[i] = maxVoltage*sin(offset + w*t);
	}
	setSignalPtr(dataPtr);
	setLength(numPoints);
}


SinusoidalWave::~SinusoidalWave()
{
}
