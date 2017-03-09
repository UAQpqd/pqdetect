// pqdApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <arrayfire.h>
#include "pqdApp.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "SignalGenerator.h"

int main(int argc, char **argv)
{
	printf(""\
		"PQD Parallel Signal Analyzer\n"\
		"------------------------------------------------\n"\
		"Authors:\tFrancisco Jose Solis Munoz\n"\
		"\t\tArturo Yosimar Jaen Cuellar\n\n");
	int device = argc > 1 ? atoi(argv[1]) : 0;
	af::setDevice(device);
	af::info();

	//Create synthetic signals generators
	SignalGenerator::SineSignalGenerator gen1 = SignalGenerator::SineSignalGenerator();
	gen1.setRate(200).setFrequency(60).setPhase(0.3).setAmplitude(127*sqrt(2));
	SignalGenerator::WhiteNoiseSignalGenerator gen2 = SignalGenerator::WhiteNoiseSignalGenerator();
	gen2.setMean(0).setStandardDeviation(10);
	//Generate the sum of them
	const size_t signalLength = 10000;
	SignalObject signal1 = gen1.generate(signalLength);
	SignalObject signal2 = gen2.generate(signalLength);
	SignalObject sumSignal = signal1 + signal2;
	sumSignal.addSagSwell(100, 300, 0.5);
	sumSignal.writeCSV("sumSignal.csv");
	system("PAUSE");
    return 0;
}

