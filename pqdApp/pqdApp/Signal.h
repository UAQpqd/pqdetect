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
	void setLength (unsigned long int in) { length = in; };
	bool Signal::saveData(std::string fileName);
	void addSignal(Signal other);
	virtual ~Signal()
	{
	}

private:
	double *data;
	unsigned long int length;

};

