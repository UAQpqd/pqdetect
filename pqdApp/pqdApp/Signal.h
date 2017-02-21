#pragma once
#include <iostream>
class Signal
{
public:

	Signal()
	{
	}
	double *getSignalPtr() { return data; };
	void setSignalPtr(double *ptr) { data = ptr; };
	unsigned long int getLength() { return length; };
	void setLength(unsigned long int in) { length = in; };
	unsigned long int getPPS() { return pps; };
	void setPPS(unsigned long int in) { pps = in; };
	bool Signal::saveData(std::string fileName);
	void addSignal(Signal other);
	void prodSignal(Signal other);
	void addPQDSagSwell(double percentage, unsigned long int start, unsigned long int length);


	virtual ~Signal()
	{
	}

private:
	double *data = 0;
	unsigned long int length = 0;
	unsigned long int pps = 0;
};

