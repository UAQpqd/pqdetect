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
		const double epsilon = pow(10,-20);
		double phi[2], alpha = M_PI/2, omega = 2*M_PI*f, d[2], alphai[2];

		SinusoidalWave tmp;
		Signal a[2];

		phi[0] = M_PI/2;
		phi[1] = -M_PI/2;
		
		const unsigned int N = 100;
		for (unsigned int k = 0; k < N && abs(alpha)>=epsilon; k++)
		{
			for (unsigned int i = 0; i < 2; i++)
			{
				tmp = SinusoidalWave(
					f/*Freq*/,
					signal.getNominalValue()/*Voltage*/,
					signal.getPPS()/*PointsPerSecond*/,
					(double)signal.getLength()/signal.getPPS()/*Seconds*/,
					phi[i]/*Start offset or phase in radians*/,
					false/*use sin = false*/);
				a[i] = tmp.createSignalFrom(signal,Signal::OP_PROD);

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
					phi[i] = alphai[0] + pow(-1, i + 1)*alpha / 2;
			else
				for (unsigned int i = 0; i < 2; i++)
					phi[i] = alphai[1] + pow(-1, i + 1)*(alpha = -abs(alpha)) / 2;
			alpha /= 2;
		}
		return tmp;
	}
}