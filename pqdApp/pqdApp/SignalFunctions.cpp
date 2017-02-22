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
		bool debug = false;
		const double epsilon = pow(10,-20);
		double phi[2], alpha = M_PI/2, omega = 2*M_PI*f, d[2], alphai[2];

		SinusoidalWave a[2];
		Signal tmp;

		phi[0] = M_PI/2;
		phi[1] = -M_PI/2;
		
		const unsigned int N = 100;
		for (unsigned int k = 0; k < N && abs(alpha)>=epsilon; k++)
		{
			for (unsigned int i = 0; i < 2; i++)
			{
				a[i] = SinusoidalWave(
					f/*Freq*/,
					/*1/sqrt(2)*/signal.getNominalValue()/*Voltage*/,
					signal.getPPS()/*PointsPerSecond*/,
					(double)signal.getLength()/signal.getPPS()/*Seconds*/,
					phi[i]+M_PI/2/*Start offset or phase in radians, cos()*/);
				tmp = a[i].createSignalFrom(signal,Signal::OP_PROD);

				long double accum = 0;
				double *myData = tmp.getSignalPtr();
				for (unsigned long int i = 0; i < tmp.getLength(); i++)
					accum += myData[i];
				accum /= (long double)tmp.getLength();
				d[i] = (double) accum;

				alphai[i] = phi[i];
			}
			if (debug) printf("[Debug] Iteration %d -- d0: %.20f, d1: %.20f\n", k, d[0], d[1]);
			if (d[0] > d[1])
				for (unsigned int i = 0; i < 2; i++)
					phi[i] = alphai[0] + pow(-1, i + 1)*alpha / 2;
			else
				for (unsigned int i = 0; i < 2; i++)
					phi[i] = alphai[1] + pow(-1, i + 1)*(alpha = -abs(alpha)) / 2;
			alpha /= 2;
		}
		return a[0];
	}
}