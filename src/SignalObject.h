#pragma once
#include <vector>
class SignalObject
{
public:
	SignalObject();
	SignalObject(std::vector<double> *data);
	SignalObject(const SignalObject& other);
	~SignalObject();
	SignalObject& operator+=(const SignalObject& rhs);
	SignalObject& operator=(const SignalObject& rhs);
	const SignalObject operator+(const SignalObject& rhs);
	void addSagSwell(size_t start, size_t end, double factor);
	bool writeCSV(std::string filename);

	std::vector<double> *m_data = 0;
private:
};