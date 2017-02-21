//signalFunctions.hpp
#include "stdafx.h"
#include "SignalFunctions.hpp"
#include "SinusoidalWave.h"
#define _USE_MATH_DEFINES 1
#include <math.h>

namespace SignalFunctions
{
	//phaseSignalDetector estimates the start offset / phase of the signal using its frequency
	SinusoidalWave phaseSignalDetector(SinusoidalWave signal, double f)
	{
		bool debug = true;
		const double epsilon = pow(10,-10);
		double phi[2];
		double alpha = M_PI / 2, omega = 2 * M_PI*f;
		double d[2], alphai[2];
		SinusoidalWave a[2];

		phi[0] = M_PI / 2;
		phi[1] = -M_PI / 2;
		
		const unsigned int N = 100;
		for (unsigned int k = 0; k < N && abs(alpha)>=epsilon; k++)
		{
			for (unsigned int i = 0; i < 2; i++)
			{
				a[i] = SinusoidalWave(
					f/*Freq*/,
					signal.getNominalValue()/*Voltage*/,
					signal.getPPS()/*PointsPerSecond*/,
					signal.getLength()/signal.getPPS()/*Seconds*/,
					phi[i]/*Start offset or phase in radians*/,
					true/*use PSD*/);
				a[i].prodSignal(signal);
				long double accum = 0;
				double *myData = a[i].getSignalPtr();
				for (unsigned int i = 0; i < a[i].getLength(); i++)
					accum += myData[i];
				accum /= (long double)a[i].getLength();
				d[i] = (double) accum;
				alphai[i] = phi[i];
			}
			if (debug) printf("[Debug] Iteration %d -- d0: %f, d1: %f\n", k, d[0], d[1]);
			if (d[0] > d[1])
				for (unsigned int i = 0; i < 2; i++)
					phi[i] = alphai[0] + pow(-1,i + 1)*alpha / 2;
			else
				for (unsigned int i = 0; i < 2; i++)
					phi[i] = alphai[1] + pow(-1, i + 1)*(alpha = -abs(alpha)) / 2;
			alpha /= 2;
		}
		return a[0];
	}
}