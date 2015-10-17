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
/*#include "Network.cpp"
#include "Neuron.cpp"
#include "Gene.cpp"
#include "Genome.cpp"
#include "Pool.cpp"
#include "Species.cpp"*/


//Global Varriables and Methods~~~~~~~~~~~~~~~~~~~~~~~~~
#define BOXRADIUS 6
#define BUTTONS 13
#define INPUTS 170
using namespace std;
using namespace System;


extern int inputSize;
extern int Inputs;
//Seth's scanning area of blocks centered on mario
extern std::vector<double> inputs;
// input of only 
//zombie coordinates, need to include ALL inputs

extern string ControlNames[BUTTONS];

extern int population;
extern double DeltaDisjoint;
extern double DeltaWeights;
extern double DetalThreshold;

extern int StaleSpecies;

extern double MutateConnectionsChance;
extern double PerturbChance;
extern double CrossoverChance;
extern double LinkMutationChance;
extern double NodeMutationChance;
extern double BiasMutationChance;
extern double StepSize;
extern double DisableMutationChance;
extern double EnableMutationChance;

extern int TimeoutConstant;
extern int MaxNodes;

int* getPosition();

int* getZombies();

bool** getInputs();

double signmoid(double x);

std::vector<double> evaluateNetwork(Network currNet, std::vector<double> currInputs);

int randomNeuron(std::vector<Gene> Genes, bool nonInput);

bool containsLink(std::vector<Gene> Genes, Gene link);

void pointMutate(Genome currGenome);

void linkMutate(Genome currGenome, bool forceBias);

void nodeMutate(Genome currGenome);

void enableDisableMutate(Genome currGenome, bool enable);

void mutate(Genome currGenome);

double disjoint(std::vector<Gene> Genes1, std::vector<Gene> Genes2);

double weights(std::vector<Gene> Genes1, std::vector<Gene> Genes2);

double sameSpecies(Genome Genome1, Genome Genome2);

void rankGlobally();


class Species 
{
public:

	double topFitness;
	double staleness;
	std::vector<Genome> genomeList;
	double averageFitness;
	Species();

}; //end of Species class

class Pool
{
public:
	std::vector<Species> speciesList;
	int generation;
	int innovation; //size of ControlNames array 
	int currentSpecies;
	int currentGenome;
	int currentFrame;
	int maxFitness;

	Pool();

}; //end of Pool class

extern Pool globalPool;

class Gene 
{
public:
	int into;
	int out;
	double weight;
	bool enabled;
	int innovation;

	Gene();

	Gene(const Gene& GeneToCopy);
}; //End of Gene Class

class Network
{
public:
	vector<Neuron> neuronList;

	Network();

	Network(Genome genomeForNetwork);

}; //end of Network Class

class Genome
{
public:
	std::vector<Gene> geneList;
	int fitness;
	int adjustedFitness;
	Network genomeNetwork;
	int maxNeuron;
	double mutationRates[7];
	double MutateConnectionsChance;
	double PerturbChance;
	double CrossoverChance;
	double LinkMutationChance;
	double NodeMutationChance;
	double BiasMutationChance;
	double StepSize;
	int globalRank;

	Genome();

	Genome(const Genome& GenomeToCopy);

	Genome(string basic); //equivalent of basicGenome line 263

}; //end of Genome Class

Genome crossover(Genome g1, Genome g2) {}

class Neuron
{
	public:
		std::vector<Gene> incoming;

		double value;

		Neuron();
}; //end of Neuron Class


#endif // NeatZombies header file inclusion if ~~~~~~~~~~~~~~~~~~~~~~~~~