#pragma once
#include <iostream>
class Signal
{
public:
	typedef enum {
		OP_DIFF,
		OP_PROD
	} opType;

	Signal()
	{
	}
	double *getSignalPtr() { return _data; };
	void setSignalPtr(double *ptr) { _data = ptr; };
	unsigned long int getLength() { return _length; };
	void setLength(unsigned long int in) { _length = in; };
	unsigned long int getPPS() { return _pps; };
	void setPPS(unsigned long int in) { _pps = in; };
	bool Signal::saveData(std::string fileName);
	void addSignal(Signal other);
	void addPQDSagSwell(double percentage, unsigned long int start, unsigned long int length);
	Signal createSignalFrom(Signal second, opType operation);

	virtual ~Signal()
	{
	}

private:
	double *_data = 0;
	unsigned long int _length = 0;
	unsigned long int _pps = 0;
};

