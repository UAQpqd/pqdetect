#pragma once
#include <vector>
#include "SignalObject.h"

namespace SignalGenerator {
	class SineSignalGenerator {
	public:
		SineSignalGenerator();
		~SineSignalGenerator();
		SineSignalGenerator& setFrequency(double frequency) { m_frequency = frequency; return *this; };
		SineSignalGenerator& setPhase(double phase) { m_phase = phase; return *this; };
		SineSignalGenerator& setAmplitude(double amplitude) { m_amplitude = amplitude; return *this; };
		SineSignalGenerator& setRate(unsigned long int rate) { m_rate = rate; return *this; };
		double getFrequency() { return m_frequency; };
		double getPhase() { return m_phase; };
		double getAmplitude() { return m_amplitude; };
		unsigned long int getRate() { return m_rate; };
		SignalObject generate(size_t N);
	private:
		double m_frequency = 50.0;
		double m_phase = 0.0;
		double m_amplitude = 100.0;
		unsigned long int m_rate = 200;
	};
	class WhiteNoiseSignalGenerator {
	public:
		WhiteNoiseSignalGenerator();
		~WhiteNoiseSignalGenerator();
		WhiteNoiseSignalGenerator& setMean(double mean) { m_mean = mean; return *this; };
		WhiteNoiseSignalGenerator& setStandardDeviation(unsigned long int sd) { m_sd = sd; return *this; };
		double getMean() { return m_mean; };
		double getStandardDeviation() { return m_sd; };
		SignalObject generate(size_t N);
	private:
		double m_mean = 0.0;
		double m_sd = 1.0;
	};
}