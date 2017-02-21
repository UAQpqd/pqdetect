#pragma once
#include "Signal.h"
class Noise :
	public Signal
{
public:
	typedef enum {
		WHITE_NOISE = 0
	} noiseType;

	Noise(noiseType nType, Signal parentSignal);
	double *Noise::createWhiteNoise(Signal parentSignal, double mean, double stddev);
	~Noise();

};

