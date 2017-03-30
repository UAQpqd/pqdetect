#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <functional>
#include <iterator>
#include <limits>
#include "SignalObject.cpp"
#include "SignalGenerator.cpp"

/*
	Serial implementation of Differential Evolution algorithm
	in order to fit a sinusoidal wave
	@author: Francisco José Solís Muñoz
	@date: 29th March 2017
*/

const double voltsConstantMin = 100;
const double voltsConstantMax = 300;					//100-300V peak
const double omegaConstantMin = 2.0*M_PI*40;			
const double omegaConstantMax = 2.0*M_PI*70;			//40-70Hz
const double phiConstant = 2.0*M_PI;					//0-2PI radians

template<typename Lambda>
std::vector<double> runDE(
	const unsigned long int S,
	const unsigned long int N,
	const std::vector<double> randomVector,
	const size_t randomVectorSize,
	const unsigned long int maxGenerations,
	const double F,
	const double R,
	Lambda scoreFunction
);

int main(int argc, char const *argv[])
{
    // Create signal
    const size_t rate = 60*10;
	const size_t signalLength = 1*rate; //Generate 10 seconds
	SignalGenerator::SineSignalGenerator gen1 = SignalGenerator::SineSignalGenerator();
	gen1.setRate(rate).setFrequency(60).setPhase(0.3).setAmplitude(127*sqrt(2));
	SignalGenerator::WhiteNoiseSignalGenerator gen2 = SignalGenerator::WhiteNoiseSignalGenerator();
	gen2.setMean(0).setStandardDeviation(10);
	//Generate the sum of them
	SignalObject signal1 = gen1.generate(signalLength);
	SignalObject signal2 = gen2.generate(signalLength);
	SignalObject sumSignal = signal1 + signal2;
	sumSignal.addSagSwell(100, 199, 0.5);
	sumSignal.writeCSV("signal.txt");

	//Random initialization
	std::random_device rnd_device;
	std::mt19937 mersenne_engine(rnd_device());
	std::uniform_real_distribution<double> dist(0, 1);
    auto gen = std::bind(dist, mersenne_engine);
    const unsigned long int S = 600;
    const unsigned long int maxGenerations = 80;
    const double F = 1.4, R = 0.5;
    size_t N = 3; //Number of parameters to estimate
    const size_t randomVectorSize = S*(N+1)+10000;	//Its recommended a big number
    std::vector<double> randomVector(randomVectorSize);
    std::generate(begin(randomVector), end(randomVector), gen);
    std::vector<double> realSignalPtr = sumSignal.m_data[0];
	std::vector<double> estimatedParameters = runDE(
		S, 
		N, 
		randomVector, 
		randomVectorSize, 
		maxGenerations, 
		F, 
		R,
		[realSignalPtr,rate](const std::vector<double> c) -> double {	//SSE Lambda function
			double accum = 0.0;
			for(size_t pos = 0; pos < realSignalPtr.size(); pos++)
			{
				double volts = voltsConstantMin+c[0]*(voltsConstantMax - voltsConstantMin);
				double omega = omegaConstantMin+c[1]*(omegaConstantMax - omegaConstantMin);
				double phi = c[2]*phiConstant;
				double t = (double)pos/(double)rate;
				accum += pow(
					volts*sin(
						omega*t+
						phi
					)-realSignalPtr[pos],2);
			}
			return accum;
		}
	);
	double signalSS = 0;
	for_each( sumSignal.m_data->begin(),sumSignal.m_data->end(), [&signalSS](double a) { signalSS += pow(a,2); });
	printf("Signal SS: %.16lf\n",signalSS);
	printf("Lowest SSE: %.16lf\n",estimatedParameters[N]);
	printf("Ratio (%%): %.16lf%%\n",estimatedParameters[N]/signalSS*100);
	return 0;
}


template<typename Lambda>
std::vector<double> runDE(
	const unsigned long int S,
	const unsigned long int N,
	const std::vector<double> randomVector,
	const size_t randomVectorSize,
	const unsigned long int maxGenerations,
	const double F,
	const double R,
	Lambda scoreFunction
)
{
	size_t offset = 0;
	//1. Initialization
	std::vector<std::vector<double>> x;
	x.resize(S, std::vector<double>(N+1,0.0));
	std::vector<double> *best = &x[0];	//First is the best
	for (std::vector<std::vector<double>>::iterator i = x.begin(); i != x.end(); ++i)
	{
		int j;
		for (j = 0; j < N; ++j)
			(*i)[j] = randomVector[offset++%randomVectorSize];
		double score = scoreFunction(*i);
		(*i)[j] = score;
		if(score<(*best)[j]) best=&(*i);
	}
	std::vector<std::vector<double>> y;
	y.resize(S, std::vector<double>(N+1,0.0));

	unsigned long int currentGeneration = 0;
	//Once population is created then enter the loop
	do
	{
		//Go one by one and modify it conditionally using best agent
		for (int currx = 0; currx < S; ++currx)
		{
			std::vector<double> *currxPtr = &x[currx], *curryPtr = &y[currx];
			//2. Reproduction
			//a. Select two different agents and best
			std::vector<std::vector<double> *> parents = {currxPtr,best};
			//a. (Variant) Select two different agents
			// std::vector<std::vector<double> *> parents = {currxPtr};
			while(parents.size()!=4)
			{
				for(;std::find(parents.begin(),parents.end(),&x[randomVector[offset % randomVectorSize]*(S-1)])!=parents.end();offset++);
				std::vector<double> *toPushBack = &x[randomVector[offset % randomVectorSize]*(S-1)];
				parents.push_back(toPushBack);
			}
			//b. Reproduce using (2.12) using b=best
			std::vector<double> childAgent(*parents[2]);
			std::transform(childAgent.begin(),childAgent.end(),parents[3]->begin(),childAgent.begin(),
				[](double l, double r){ 
					return l-r; 
				});
			std::transform(childAgent.begin(),childAgent.end(),parents[1]->begin(),childAgent.begin(),
				[F,randomVector,randomVectorSize,&offset](double l, double r){ 
					return r-F*randomVector[offset++%randomVectorSize]*l; 
				});
			//c. Crossover
			const size_t delta = round(randomVector[offset++%randomVectorSize]*(N-1));
			std::transform(childAgent.begin(),childAgent.end(),parents[0]->begin(),childAgent.begin(),
				[R,delta,childAgent,randomVector,randomVectorSize,&offset](double l, double r){ 
					return (randomVector[offset++%randomVectorSize]>R && (&l-&childAgent[0]!=delta))?r:std::min(std::max(l,0.0),1.0); 
				});
			y[currx] = childAgent;
		}
		//3. Evaluation and selection
		for (int i = 0; i < S; ++i)
		{
			double score = scoreFunction(y[i]);
			y[i][N] = score;
			if(score<x[i][N])	//If new score is better than before
			{
				x[i]=y[i];		//This agent will enter at next generation
				if(score<(*best)[N]) {
					best = &x[i];
				}
			}
		}
		//DEBUG
		currentGeneration++;
		printf("Best in generation %ld SSE %lf:\n" \
			"\t[%.16lf,%.16lf,%.16lf]\n" \
			"\t[%.16lf V,%.16lf Hz,%.16lf Radians]\n",
			currentGeneration,(*best)[3],
			(*best)[0],(*best)[1],(*best)[2],
			voltsConstantMin+(*best)[0]*(voltsConstantMax - voltsConstantMin),
			(omegaConstantMin+(*best)[1]*(omegaConstantMax - omegaConstantMin))/(2*M_PI),
			(*best)[2]*phiConstant);
	} while(currentGeneration<maxGenerations);
	return *best;
}