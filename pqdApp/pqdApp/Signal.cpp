#include "stdafx.h"
#include "Signal.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

bool Signal::saveData(std::string fileName)
{
	double *p = getSignalPtr();
	FILE *pFile;
	std::string format = "%f,";
	std::string format2 = "%f\n";
	fopen_s(&pFile,fileName.c_str(), "w");
	for (unsigned long int i = 0; i < getLength()-1; i++)
	{
		fprintf(pFile, format.c_str(), *p);
		p++;
	}
	fprintf(pFile, format2.c_str(), *p);
	fclose(pFile);
	return true;
}

void Signal::addSignal(Signal other)
{
	if (other.getLength() == this->getLength())
	{
		double *dst = this->getSignalPtr();
		double *src = other.getSignalPtr();
		for (unsigned int i = 0; i < this->getLength(); i++)
			dst[i] += src[i];
	}
}

void Signal::prodSignal(Signal other)
{
	if (other.getLength() == this->getLength())
	{
		double *dst = this->getSignalPtr();
		double *src = other.getSignalPtr();
		for (unsigned int i = 0; i < this->getLength(); i++)
			dst[i] *= src[i];
	}
}

void Signal::addPQDSagSwell(double percentage, unsigned long int start, unsigned long int length)
{
	if (start < getLength() &&
		start + length < getLength() &&
		percentage >= 0.0)
		for (unsigned int i = start; i < start + length; i++)
			data[i] *= percentage;
}
