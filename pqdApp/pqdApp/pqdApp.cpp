// pqdApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pqdApp.h"
#include "Signal.h"
#include "SinusoidalWave.h"
#include "Noise.h"
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
	Signal mySignal = SinusoidalWave(60/*Freq*/,127/*Voltage*/,600/*PointsPerSecond*/,1/*Seconds*/,M_PI+0.5/*Start offset in radians*/);
	Noise myWhiteNoise(Noise::WHITE_NOISE, mySignal);
	//Add white noise to signal
	mySignal.addSignal(myWhiteNoise);

	//Save the signal to a CSV format
	printf("Writing signal data to myTestSignal.txt ...");
	mySignal.saveData("myTestSignal.txt");
	printf("OK\n");
	system("PAUSE");
    return 0;
}

