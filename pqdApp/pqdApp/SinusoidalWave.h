#pragma once
#include "Signal.h"
class SinusoidalWave :
	public Signal
{
public:
	SinusoidalWave();
	SinusoidalWave(double freq, double nominalVoltage, unsigned long int pps, double seconds, double phi, bool useSin = true);
	~SinusoidalWave();
	double getNominalValue() { return _nominalValue; };
	void setNominalValue(double in) { _nominalValue = in; };
	double getFrequency() { return _frequency; };
	void setFrequency(double in) { _frequency = in; };
	double getPhi() { return _phi; };
	void setPhi(double in) { _phi = in; };
private:
	double _nominalValue = 0;
	double _frequency = 0;
	double _phi = 0;
};

