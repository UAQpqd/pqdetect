#include "SignalGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

SignalGenerator::SineSignalGenerator::SineSignalGenerator()
{
}

SignalGenerator::SineSignalGenerator::~SineSignalGenerator()
{
}

SignalObject SignalGenerator::SineSignalGenerator::generate(size_t N)
{
	std::vector<double> *m_data = new std::vector<double>(N);

	double w = 2*M_PI*m_frequency;
	for (size_t i = 0; i < N; i++)
		(*m_data)[i] = m_amplitude*sin(w*(double)i/(double)m_rate+m_phase);

	return SignalObject(m_data);
}

SignalGenerator::WhiteNoiseSignalGenerator::WhiteNoiseSignalGenerator()
{
}

SignalGenerator::WhiteNoiseSignalGenerator::~WhiteNoiseSignalGenerator()
{
}

SignalObject SignalGenerator::WhiteNoiseSignalGenerator::generate(size_t N)
{
	std::vector<double> *m_data = new std::vector<double>(N);
	SignalObject result(m_data);

	std::default_random_engine normalNumberGenerator;
	std::normal_distribution<double> distribution(m_mean, m_sd);
	for (size_t i = 0; i < N; i++)
		(*m_data)[i] = distribution(normalNumberGenerator);

	return result;
}
