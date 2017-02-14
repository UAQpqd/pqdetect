#pragma once
#include "Signal.h"
class SinusoidalWave :
	public Signal
{
public:
	SinusoidalWave();
	SinusoidalWave(double freq, double voltage, unsigned long int pps, double seconds, double offset);
	~SinusoidalWave();
};

