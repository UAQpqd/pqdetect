#include "SignalObject.h"
#include <algorithm>
#include <functional>


SignalObject::SignalObject()
{
}
SignalObject::SignalObject(std::vector<double>* data)
{
	m_data = data;
}

SignalObject::SignalObject(const SignalObject & other)
{
	m_data = new std::vector<double>(other.m_data->begin(),other.m_data->end());
}

SignalObject::~SignalObject()
{
}

SignalObject & SignalObject::operator+=(const SignalObject & rhs)
{
	std::transform(
		m_data->begin(),
		m_data->end(),
		rhs.m_data->begin(),
		m_data->begin(),
		std::plus<double>()
	);
	return *this;
}

SignalObject & SignalObject::operator=(const SignalObject & rhs)
{
	SignalObject *result = new SignalObject(rhs);
	return *result;
}

const SignalObject SignalObject::operator+(const SignalObject & rhs)
{
	SignalObject result(*this);
	result += rhs;
	return result;
}

void SignalObject::addSagSwell(size_t start, size_t end, double factor)
{
	std::transform(
		m_data->begin() + start, 
		m_data->begin() + end, 
		m_data->begin()+start, 
		std::bind2nd(std::multiplies<double>(), factor)
	);
}

bool SignalObject::writeCSV(std::string filename)
{
	FILE * pFile;
	pFile = fopen(filename.c_str(), "w");
	if (pFile == NULL)
	{
		return false;
		fclose(pFile);
	}
	size_t i = 0;
	for (;i < m_data->size()-1; i++)
		fprintf(pFile, "%f,", (*m_data)[i]);
	fprintf(pFile, "%f\n", (*m_data)[i]);
	return false;
}
