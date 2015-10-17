//Header Guard~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef NEATZombiesBoilerMake2015_NEAT_H_Included_
#define NEATZombiesBoilerMake2015_NEAT_H_Included_

//Forward Declared Dependencies~~~~~~~~~~~~~~~~~~~~~~~~~
class Gene;
class Genome;
class Pool;
class Network;
class Neuron;
class Species;

//Included Dependencies~~~~~~~~~~~~~~~~~~~~~~~~~
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Network.cpp"
#include "Neuron.cpp"
#include "Gene.cpp"
#include "Genome.cpp"
#include "Pool.cpp"
#include "Species.cpp"


//Global Varriables and Methods~~~~~~~~~~~~~~~~~~~~~~~~~
#define BOXRADIUS
#define BUTTONS
#define INPUTS
using namespace std;
using namespace System;


int inputSize;
int Inputs;
//Seth's scanning area of blocks centered on mario
vector<double> inputs();
// input of only 
//zombie coordinates, need to include ALL inputs

string ControlNames[BUTTONS];

int population;
double DeltaDisjoint;
double DeltaWeights;
double DetalThreshold;

int StaleSpecies;

double MutateConnectionsChance;
double PerturbChance;
double CrossoverChance;
double LinkMutationChance;
double NodeMutationChance;
double BiasMutationChance;
double StepSize;
double DisableMutationChance;
double EnableMutationChance;

int TimeoutConstant;
int MaxNodes;
Pool globalPool;

int* getPosition() {}

int* getZombies() {}

bool** getInputs() {}

double signmoid(double x) {}

std::vector<double> evaluateNetwork(Network currNet, std::vector<double> currInputs) {}

Genome crossover(Genome g1, Genome g2) {}

int randomNeuron(std::vector<Gene> Genes, bool nonInput) {}

bool containsLink(std::vector<Gene> Genes, Gene link) {}

void pointMutate(Genome currGenome) {}

void linkMutate(Genome currGenome, bool forceBias) {}

void nodeMutate(Genome currGenome) {}

void enableDisableMutate(Genome currGenome, bool enable) {}

void mutate(Genome currGenome) {}

double disjoint(std::vector<Gene> Genes1, std::vector<Gene> Genes2) {}

double weights(std::vector<Gene> Genes1, std::vector<Gene> Genes2) {}

double sameSpecies(Genome Genome1, Genome Genome2) {}

void rankGlobally() {}


class Species 
{
public:

	int topFitness;
	int staleness;
	std::vector<Genome> GenomeList;
	int averageFitness;
	Species()
	{}
}; //end of Species class

class Pool
{
public:
	Species spec;
	int generation;
	int innovation; //size of ControlNames array 
	int currentSpecies;
	int currentGenome;
	int currentFrame;
	int maxFitness;

	Pool()
	{}
}; //end of Pool class

class Gene 
{
public:
	int into;
	int out;
	double weight;
	bool enabled;
	int innovation;

	Gene()
	{ }

	Gene(const Gene& GeneToCopy)
	{ }
}; //End of Gene Class

class Network
{
	vector<Neuron> neuronList{ MaxNodes + BUTTONS };

	Network() { };

	Network(Genome genomeForNetwork)
	{ }

}; //end of Network Class
class Genome
{
public:
	std::vector<Gene> GeneList;
	int fitness;
	int adjustedFitness;
	Network GenomeNetwork;
	int maxNeuron;
	double mutationRates[7];
	double MutateConnectionsChance;
	double PerturbChance;
	double CrossoverChance;
	double LinkMutationChance;
	double NodeMutationChance;
	double BiasMutationChance;
	double StepSize;

	Genome()
	{ }

	Genome(const Genome& GenomeToCopy)
	{ }

	Genome(string basic) //equivalent of basicGenome line 263
	{ }

}; //end of Genome Class

class Neuron
{
	public:
		std::vector<Gene> incoming;

		double value;

		Neuron()
		{

		}
}; //end of Neuron Class


#endif // NeatZombies header file inclusion if ~~~~~~~~~~~~~~~~~~~~~~~~~