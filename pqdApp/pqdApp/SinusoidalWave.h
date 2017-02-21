#pragma once
#include "Signal.h"
class SinusoidalWave :
	public Signal
{
public:
	SinusoidalWave();
	SinusoidalWave(double freq, double nominalVoltage, unsigned long int pps, double seconds, double phi, bool psd = true);
	~SinusoidalWave();
	double getNominalValue() { return nominalValue; };
	void setNominalValue(double in) { nominalValue = in; };
	double getFrequency() { return frequency; };
	void setFrequency(double in) { frequency = in; };
	double getPhi() { return phi; };
	void setPhi(double in) { phi = in; };
private:
	double nominalValue = 0;
	double frequency = 0;
	double phi = 0;
};

