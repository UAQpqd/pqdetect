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
		fprintf(pFile, format.c_str(), *p++);
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
		for (unsigned long int i = 0; i < this->getLength(); i++)
			dst[i] += src[i];
	}
}

void Signal::addPQDSagSwell(double percentage, unsigned long int start, unsigned long int length)
{
	if (start < getLength() &&
		start + length < getLength() &&
		percentage >= 0.0)
		for (unsigned long int i = start; i < start + length; i++)
			data[i] *= percentage;
}

Signal Signal::createSignalFrom(Signal second, opType operation)
{
	Signal result; //this-second
	if (getLength() == second.getLength())
	{
		result.setLength(getLength());
		result.setPPS(getPPS());
		double *myPtr = (double *)malloc(sizeof(double)*getLength());
		double *firstPtr = getSignalPtr();
		double *secondPtr = second.getSignalPtr();
		result.setSignalPtr(myPtr);
		switch (operation)
		{
		case OP_DIFF:
			for (unsigned long int i = 0; i < getLength(); i++)
				myPtr[i] = firstPtr[i] - secondPtr[i];
			break;
		case OP_PROD:
			for (unsigned long int i = 0; i < getLength(); i++)
				myPtr[i] = firstPtr[i] * secondPtr[i];
			break;
		default:
			break;
		}
		
	}
	return result;
}
