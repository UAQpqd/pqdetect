// pqdApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pqdApp.h"
#include "Signal.h"
#include "SinusoidalWave.h"
#include "Noise.h"
#include "SignalFunctions.hpp"
#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <math.h>

int main()
{
	printf(""\
		"PQD Parallel Signal Analyzer\n"\
		"------------------------------------------------\n"\
		"Authors:\tFrancisco Jose Solis Munoz\n"\
		"\t\tArturo Yosimar Jaen Cuellar\n\n");

	//Create a new signal to analyze
	double frequency = 60;
	double phi = M_PI/2+0.4;
	SinusoidalWave mySignal = SinusoidalWave(frequency/*Freq*/,127/*Voltage*/,600/*PointsPerSecond*/,1/*Seconds*/, phi/*Start offset or phase in radians*/);
	//Noise myWhiteNoise(Noise::WHITE_NOISE, mySignal);
	//Add 0.5 sag between points 300 and 400
	//mySignal.addPQDSagSwell(0.5, 300, 100);
	//Add white noise to signal (this must be last step)
	//mySignal.addSignal(myWhiteNoise);

	//Save the signal to a CSV format
	printf("Writing signal data to myTestSignal.txt ...");
	mySignal.saveData("myTestSignal.txt");
	printf("Saved\n");
	
	//Once the synthetic signal has been created then estimate it's start offset / phase
	printf("Running Phase Signal Detector...\n");
	SinusoidalWave mySignalEstimated = SignalFunctions::phaseSignalDetector(mySignal, frequency);
	printf("Real phi: %.4f, Estimated phi: %.4f\n", phi, mySignalEstimated.getPhi());
	//Save the signal to a CSV format
	printf("Writing estimated signal data to myEstSignal.txt ...");
	mySignalEstimated.saveData("myEstSignal.txt");
	printf("Saved\n");

	//Once the signal is estimated the diff is calculated
	Signal diffSignal = mySignal.createSignalFrom(mySignalEstimated,Signal::OP_DIFF);
	//Save the signal to a CSV format
	printf("Writing diff data to myDiffSignal.txt ...");
	diffSignal.saveData("myDiffSignal.txt");
	printf("Saved\n");

	system("PAUSE");
    return 0;
}

