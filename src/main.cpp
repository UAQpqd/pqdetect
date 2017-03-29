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

template<typename Lambda>
std::vector<double> runDE(
	const unsigned long int S,
	const unsigned long int D,
	const std::vector<double> randomVector,
	const unsigned long int maxGenerations,
	const double stopEpsilon,
	const double F,
	const double R,
	Lambda scoreFunction
);

int main(int argc, char const *argv[])
{
    // Create signal
    const size_t rate = 200;
	SignalGenerator::SineSignalGenerator gen1 = SignalGenerator::SineSignalGenerator();
	gen1.setRate(rate).setFrequency(60).setPhase(0.3).setAmplitude(127*sqrt(2));
	SignalGenerator::WhiteNoiseSignalGenerator gen2 = SignalGenerator::WhiteNoiseSignalGenerator();
	gen2.setMean(0).setStandardDeviation(10);
	//Generate the sum of them
	const size_t signalLength = 10000;
	SignalObject signal1 = gen1.generate(signalLength);
	SignalObject signal2 = gen2.generate(signalLength);
	SignalObject sumSignal = signal1 + signal2;
	sumSignal.addSagSwell(100, 199, 0.5);

	//Random initialization
	std::random_device rnd_device;
	std::mt19937 mersenne_engine(rnd_device());
	std::uniform_real_distribution<double> dist(0, 1);
    auto gen = std::bind(dist, mersenne_engine);
    const unsigned long int S = 300;
    const unsigned long int maxGenerations = 100;
    const double stopEpsilon = 10e-6;
    const double F = 2, R = 0.5;
    size_t D = 3; //Number of parameters to estimate
    std::vector<double> randomVector(S*D+maxGenerations*S);
    std::generate(begin(randomVector), end(randomVector), gen);

    std::vector<double> realSignalPtr = sumSignal.m_data[0];

	std::vector<double> estimatedParameters = runDE(S, D, randomVector, maxGenerations, stopEpsilon, F, R,
		[realSignalPtr,rate](const std::vector<double> c) -> double { 
			double accum = 0;
			for(size_t pos = 0; pos < realSignalPtr.size(); pos++)
				accum += 
					abs(c[0]*400*(
						sin(2.0*M_PI*c[1]*(double)pos/(double)rate*100
						+c[2]*2.0*M_PI)
					)-realSignalPtr[pos]);
			return accum/(double)realSignalPtr.size();
		}
	);
	return 0;
}


template<typename Lambda>
std::vector<double> runDE(
	const unsigned long int S,
	const unsigned long int D,
	const std::vector<double> randomVector,
	const unsigned long int maxGenerations,
	const double stopEpsilon,
	const double F,
	const double R,
	Lambda scoreFunction
)
{
	size_t offset = 0;
	const size_t m = S*D+maxGenerations*S;
	//Create population
	std::vector<std::vector<double>> agents;
	agents.resize(S, std::vector<double>(D+1,0));
	std::vector<std::vector<double>> NGagents(agents);
	std::vector<std::vector<double>> *agentsPtr = &agents, *NGagentsPtr = &NGagents;
	std::vector<double> *best = &(*agentsPtr)[0];
	for (int i = 0; i < S; ++i)
	{
		(*agentsPtr)[i][0]=randomVector[offset++];
		(*agentsPtr)[i][1]=randomVector[offset++];
		(*agentsPtr)[i][2]=randomVector[offset++];
		double score = scoreFunction((*agentsPtr)[i]);
		(*agentsPtr)[i][3]=score;
		if(score<(*best)[3]) best = &(*agentsPtr)[i];
	}
	//Score of the random population

	double lastMSE = 0, minMSE = std::numeric_limits<double>::max();
	unsigned long int currentGen = 0;
	//Once population is created then enter the loop
	do
	{
		lastMSE = minMSE;
		minMSE = std::numeric_limits<double>::max();
		for (int i = 0; i<(*agentsPtr).size(); ++i)
		{	
			if(best == &(*agentsPtr)[i]) continue;
			//For each agent select 3 different (*agentsPtr)
			std::vector<std::vector<double> *> diffagents = {&(*agentsPtr)[i],best};
			std::vector<double> *agentPtr;
			for (int k = 2; k < 4; ++k)
			{
				do
				{
					agentPtr = &(*agentsPtr)[randomVector[offset++%m]*S];
				} while(std::find(diffagents.begin(),diffagents.end(),agentPtr)!=diffagents.end());
				diffagents.push_back(agentPtr);
			}
			//Then reproduce
			//std::vector<double> y = diffagents[1]-F*randomVector[offset++%m]*(diffagents[2]-diffagents[3]);
			std::vector<double> y(*diffagents[2]);
			std::transform(y.begin(),y.end(),diffagents[3]->begin(),y.begin(),
				[F,randomVector,&offset,m](double a,double b){
					return (F*randomVector[offset++%m])*(a-b);
				}
			);
			std::transform(y.begin(),y.end(),diffagents[1]->begin(),y.begin(),
				[](double a,double b){
					return b-a;
				}
			);

			//Then mutate
			//Static member:
			size_t delta = round(randomVector[offset++%m]*D)-1;
			for (int j = 0; j < D; ++j)
			{
				if(delta!=j && randomVector[offset++%m]>R) y[j]=(*diffagents[1])[j];
				else y[j]=std::min(1.0,std::max(0.0,y[j]));
			}
			//Selection
			double score = scoreFunction(y);
			minMSE = std::min(minMSE,score);
			if ((*agentsPtr)[i][D]<score)
				(*NGagentsPtr)[i]=(*agentsPtr)[i];
			else
			{
				y[D]=score;
				(*NGagentsPtr)[i]=y;
			}
			if(y[D]<(*best)[D]) best = &(*NGagentsPtr)[i];
		}
		currentGen++;
		for (std::vector<std::vector<double>>::iterator i = (*agentsPtr).begin(); i != (*agentsPtr).end(); ++i)
		{
			//std::cout << "Agent: [" << (*i)[0]*400 << "," << (*i)[1]/(2*M_PI*1/200) << "," << (*i)[2]*2*M_PI << "]=" << (*i)[3] << std::endl;
			std::cout << "Real Agent: [" << (*i)[0] << "," << (*i)[1] << "," << (*i)[2] << "]=" << (*i)[3] << std::endl;
		}
		std::swap(*NGagentsPtr,*agentsPtr);
		std::cout << "Generation " << currentGen << ", Conditions:" << (abs(lastMSE-minMSE)>=stopEpsilon) << ", " << (maxGenerations > currentGen) << std::endl;
		std::cout << "lastMSE: " << lastMSE << ", minMSE: " << minMSE << std::endl;
		std::cout << "Best of this gen: " << (*best)[0] << "," << (*best)[1] << "," << (*best)[2] << "=" << (*best)[3] << std::endl;
		std::cout << "Agent: [" << (*best)[0]*400 << "," << (*best)[1]*2*M_PI/200*100 << "," << (*best)[2]*2*M_PI << "]=" << (*best)[3] << std::endl;
	} while(abs(lastMSE-minMSE)>=stopEpsilon && maxGenerations > currentGen);
}