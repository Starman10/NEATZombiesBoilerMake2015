// NEATZombiesBoilerMake2015.cpp : main project file.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>
#include "NEATZombiesBoilerMake2015.h"


#define BOXRADIUS 6
#define BUTTONS 13
#define INPUTS 170
using namespace std;
using namespace System;

int main()
{
    Console::WriteLine(L"Hello World");
    return 0;
}

class Pool;

int inputSize = (BOXRADIUS * 2 + 1) * (BOXRADIUS * 2 + 1);
int Inputs = inputSize + 1;
//Seth's scanning area of blocks centered on mario
vector<double> inputs((BOXRADIUS * 2 + 1)*(BOXRADIUS * 2 + 1));
// input of only 
//zombie coordinates, need to include ALL inputs


string ControlNames[BUTTONS] = {
	"W", //forward
	"A", //left
	"S", //backpedal
	"D", //right
	"RIGHTCLICK", //ADS
	"LEFTCLICK", //attack
	"R", //reload
	"V", //melee 
	"F", //interact
	"I", //look up
	"J", //look left
	"K", //look down
	"L"  //look right
};
int population = 300;
double DeltaDisjoint = 2.0;
double DeltaWeights = 0.4;
double DeltaThreshold = 1.0;

int StaleSpecies = 15;

double MutateConnectionsChance = 0.25;
double PerturbChance = 0.90;
double CrossoverChance = 0.75;
double LinkMutationChance = 2.0;
double NodeMutationChance = 0.50;
double BiasMutationChance = 0.40;
double StepSize = 0.1;
double DisableMutationChance = 0.4;
double EnableMutationChance = 0.2;

int TimeoutConstant = 20;

int MaxNodes = 1000000;
Pool globalPool = Pool();

int* getPositions() {
	int playerPosition[2];
	playerPosition[1] = getPlayerX();//pseudo
	playerPosition[2] = getPlayerY();//pseudo
}

/*
int getTile(dx, dy) {
used to get tiles with reference to mario's view centered on player
}
*/
int* getZombies() {
	int zombieLocations[2][24];
	Entity[] zombies = getEntArray("ZOMBIE ENTITIY NAME FIND OUT", "targetname");//find out how to get zombie objects

	for (int zombieNum = 0; zombieNum < 24; zombieNum++) {
		if (zombies.length < zombieNum) {
			zombieLocations[zombieNum][0] = (int)zombies[zombieNum].getX;//pseudo
			zombieLocations[zombieNum][1] = (int)zombies[zombieNum].getZ;//pseudo
		}
		zombieLocations[zombieNum][0] = INT_MAX;
		zombieLocations[zombieNum][1] = INT_MAX;
	}

}

bool** getInputs() {
	bool** inputs = new bool*[BOXRADIUS * 2 + 1];
	for (int i = 0; i < 10; ++i)
	{
		inputs[i] = new bool[BOXRADIUS * 2 + 1];
	}
	int* playerPos = getPositions();//pseudo
	int** zombiePos = getZombies();//pseudo
	for (int zNum = 0; zNum < 24; zNum++) {
		for (int dx = -BOXRADIUS; dx <= BOXRADIUS; dx++) {
			for (int dz = -BOXRADIUS; dz <= BOXRADIUS; dz++) {
				if (playerPos[0] + dx == zombiePos[zNum][0] &&
					playerPos[1] + dz == zombiePos[zNum][1])
					inputs[dx + BOXRADIUS][dz + BOXRADIUS] = true;
			}
		}
	}
	return inputs;
}

double sigmoid(double x) {
	return 2 / (1 + exp(-4.9*x)) - 1;
}

std::vector<double> evaluateNetwork(Network currNet, std::vector<double> currInputs) {
	currInputs.push_back(1.0);

	if (currInputs.size() != Inputs) {
		cout << "Incorrect number of neural Network inputs.\n";
		return;
	}
	for (int i = 0; i < Inputs; i++)
	{
		currNet.neuronList[i].value = currInputs[i];
	}
	//http://www.lua.org/pil/7.2.html
	//don't know how to replicate this:
	//for _,Neuron in pairs(Network.Neurons) dob1
	for (int i = 0; i < currNet.neuronList.size(); i++) {
		int tempSum = 0;
		for (int j = 0; j < currNet.neuronList[i].incoming.size(); j++) {
			Gene tempIncoming = currNet.neuronList[i].incoming[j];
			Neuron other = currNet.neuronList[tempIncoming.into];
			tempSum = tempSum + tempIncoming.weight * other.value;
		}
		if (currNet.neuronList[i].incoming.size() > 0) {
			currNet.neuronList[i].value = sigmoid(tempSum);
		}
	}
	std::vector<double> outputs;
	for (int o = 1; o < BUTTONS + 1; o++) {
		string button = "P1 " + ControlNames[o];
		if (currNet.neuronList[MaxNodes + o].value > 0)
		{
			outputs[o] = true;
		} // lines 359-361 imply outputs[string] = true which makes no sense...?
		else {
			outputs[o] = false;
		}
	}
	return outputs;
}

Genome crossover(Genome g1, Genome g2) {
	if (g2.fitness > g1.fitness) {
		Genome tempg = g1;
		g1 = g2;
		g2 = tempg;
	}

	Genome child = Genome();

	std::vector<Gene> innovations2;
	for (int i = 0; i < g2.geneList.size(); i++) {
		Gene tempGene = g2.geneList[i];
		innovations2[tempGene.innovation] = tempGene;
	}

	for (int i = 0; i < g1.geneList.size(); i++) {
		Gene Gene1 = g1.geneList[i];
		Gene Gene2 = innovations2[Gene1.innovation];
		if (rand() % 2 == 1 && Gene2.enabled) {
			child.geneList.push_back(Gene(Gene2));
		}
		else {
			child.geneList.push_back(Gene(Gene1));
		}
	}
	child.maxNeuron = max(g1.maxNeuron, g2.maxNeuron);

	for (int i = 0; i < 7; i++) { //7 is number of mutation rates (231)
		child.mutationRates[i] = g1.mutationRates[i];
	}

	return child;
}

int randomNeuron(std::vector<Gene> Genes, bool nonInput) {
	std::vector<bool> Neurons;
	if (!nonInput) {
		for (int i = 0; i < Inputs; i++) {
			Neurons[i] = true;
		}
	}

	for (int o = 1; o < BUTTONS; o++) {
		Neurons[MaxNodes + o] = true;
	}
	for (int i = 0; i < Genes.size(); i++) {
		if (!nonInput || Genes[i].into > Inputs) {
			Neurons[Genes[i].into] = true;
		}
		if (!nonInput || Genes[i].out > Inputs) {
			Neurons[Genes[i].out] = true;
		}
	}
	int count = 0;
	for (int i = 0; i < Neurons.size(); i++) {
		count = count + 1;
	}
	int n = rand() % count + 1;

	for (int i = 0; i < Neurons.size(); i++) {
		n = n - 1;
		if (n == 0) {
			return i;
		}
	}
	return 0;
}

bool containsLink(std::vector<Gene> Genes, Gene link) {
	for (int i = 0; i < Genes.size(); i++) {
		Gene tempGene = Genes[i];
		if (tempGene.into == link.into && tempGene.out == link.out) {
			return true;
		}
	}
}

void pointMutate(Genome currGenome) {
	double step = currGenome.mutationRates[6];

	for (int i = 0; i < currGenome.geneList.size(); i++) {
		Gene tempGene = currGenome.geneList[i];
		if (((double)rand() / (double)(RAND_MAX)) < PerturbChance) {
			tempGene.weight = tempGene.weight +
				((double)rand() / (double)(RAND_MAX))*step * 2
				- step;
		}
		else {
			tempGene.weight = ((double)rand() / (double)(RAND_MAX)) * 4 - 2;
		}
	}
}

void linkMutate(Genome currGenome, bool forceBias) {
	int Neuron1 = randomNeuron(currGenome.geneList, false);
	int Neuron2 = randomNeuron(currGenome.geneList, true);

	Gene newLink = Gene();
	if (Neuron1 <= Inputs && Neuron2 <= Inputs) {
		return;
	}
	if (Neuron2 <= Inputs) {
		int temp = Neuron1;
		Neuron1 = Neuron2;
		Neuron2 = temp;
	}

	newLink.into = Neuron1;
	newLink.out = Neuron2;

	if (forceBias) {
		newLink.into = Inputs;
	}
	if (containsLink(currGenome.geneList, newLink)) {
		return;
	}
	newLink.innovation = newLink.innovation + 1;
	newLink.weight = ((double)rand() / (double)(RAND_MAX)) * 4 - 2;
	currGenome.geneList.push_back(newLink);
	//STOPPED AT 485 of Seth
}

void nodeMutate(Genome currGenome) {
	if (currGenome.geneList.size() == 0) {
		return;
	}
	currGenome.maxNeuron = currGenome.maxNeuron + 1;
	Gene currGene = currGenome.geneList[rand() % currGenome.geneList.size() + 1];
	if (currGene.enabled) {
		return;
	}
	currGene.enabled = false;

	Gene Gene1 = Gene(currGene);
	Gene1.out = currGenome.maxNeuron;
	Gene1.weight = 1.0;
	Gene1.innovation++;
	Gene1.enabled = true;
	currGenome.geneList.push_back(Gene1);

	Gene Gene2 = Gene(currGene);
	Gene2.into = currGenome.maxNeuron;
	Gene2.innovation++;
	Gene2.enabled = true;
	currGenome.geneList.push_back(Gene2);

}

void enableDisableMutate(Genome currGenome, bool enable) {
	std::vector<Gene> candidates;
	Gene tempGene;
	for (int i = 0; i < currGenome.geneList.size(); i++) {
		tempGene = currGenome.geneList[i];
		if (tempGene.enabled == !enable) {
			candidates.push_back(tempGene);
		}
	}
	if (candidates.size() == 0) {
		return;
	}
	Gene randGene = candidates[rand() % candidates.size()];
	randGene.enabled = !randGene.enabled;
}

void mutate(Genome currGenome) {
	for (int i = 0; i < 7; i++) {
		double rate = currGenome.mutationRates[i];
		if ((rand() % 2) < 1) {
			currGenome.mutationRates[i] = 0.95*rate;
		}
		else {
			currGenome.mutationRates[i] = 1.05263*rate;
		}
		if (((double)rand() / (double)(RAND_MAX)) < currGenome.mutationRates[0])
		{
			pointMutate(currGenome);
		}
		double p = currGenome.mutationRates[1];
		while (p > 0)
		{
			if (((double)rand() / (double)(RAND_MAX)) < p)
			{
				linkMutate(currGenome, true);
			}
			p = p - 1;
		}
		p = currGenome.mutationRates[2];
		while (p > 0) {
			if (((double)rand() / (double)(RAND_MAX)) < p)
			{
				linkMutate(currGenome, true);
			}
			p = p - 1;
		}
		p = currGenome.mutationRates[3];
		while (p > 0)
		{
			if (((double)rand() / (double)(RAND_MAX)) < p) {
				nodeMutate(currGenome);
			}
			p = p - 1;
		}
		p = currGenome.mutationRates[4];
		while (p > 0)
		{
			if (((double)rand() / (double)(RAND_MAX)) < p) {
				enableDisableMutate(currGenome, true);
			}
			p = p - 1;
		}
		p = currGenome.mutationRates[5];
		while (p > 0)
		{
			if (((double)rand() / (double)(RAND_MAX)) < p)
			{
				enableDisableMutate(currGenome, true);
			}
			p = p - 1;
		}
	}
}

//588
double disjoint(std::vector<Gene> Genes1, std::vector<Gene> Genes2) {
	std::vector<bool> i1;
	for (int i = 0; i < Genes1.size(); i++) {
		Gene currGene = Genes1[i];
		i1[currGene.innovation] = true;
	}

	std::vector<bool> i2;
	for (int i = 0; i < Genes2.size(); i++) {
		Gene currGene = Genes2[i];
		i2[currGene.innovation] = true;
	}

	int disjointGenes = 0;
	for (int i = 0; i < Genes1.size(); i++) {
		Gene currGene = Genes1[i];
		if (!i2[currGene.innovation]) {
			disjointGenes = disjointGenes + 1;
		}
	}
	for (int i = 0; i < Genes2.size(); i++) {
		Gene currGene = Genes2[i];
		if (!i1[currGene.innovation]) {
			disjointGenes = disjointGenes + 1;
		}
	}
	return (double)disjointGenes / (double)std::max(Genes1.size(), Genes2.size());
}

//621
double weights(std::vector<Gene> Genes1, std::vector<Gene> Genes2) {
	std::vector<Gene> i2;
	for (int i = 0; i < Genes2.size(); i++) {
		Gene currGene = Genes2[i];
		i2[currGene.innovation] = currGene;
	}
	int sum = 0;
	int coincident = 0;
	for (int i = 0; i < Genes1.size(); i++) {
		Gene currGene = Genes1[i];
		//not needed? if (i2[Gene.innovation] != NULL) {
		Gene currGene2 = i2[currGene.innovation];
		sum = sum + std::abs(currGene.weight - currGene2.weight);
		coincident = coincident + 1;
		//}
	}
	return sum / coincident;
}

double sameSpecies(Genome Genome1, Genome Genome2) {
	double dd = DeltaDisjoint*disjoint(Genome1.geneList, Genome2.geneList);
	double dw = DeltaWeights*weights(Genome1.geneList, Genome2.geneList);
	return dd + dw < DeltaThreshold;

}

void rankGlobally() {
	std::vector<Genome> global;
	for (int s = 0; s < globalPool.speciesList.size(); s++) {
		Species currSpecies = globalPool.speciesList[s];
		for (int g = 0; g < currSpecies.genomeList.size(); g++) {
			global.push_back(currSpecies.genomeList[g]);
		}
	}
	struct by_out {
		bool operator()(Genome a, Genome b) {
			return a.fitness < b.fitness;
		}
	};
	std::sort(global.begin(), global.end(), by_out());

	for (int g = 0; g < global.size(); g++) {
		global[g].globalRank = g;
	}

}

//665
void calculateAverageFitness(Species currSpecies) {
	int total = 0;
	int g;
	for (g = 0; g < currSpecies.genomeList.size(); g++) {
		Genome currGenome = currSpecies.genomeList[g];
		total = total + currGenome.globalRank;
//NEED TO ADD GLOBAL RANK TO GENOME CONSTRUCTOR
	}
	currSpecies.averageFitness = (double)total / --g;
}

double totalAverageFitness() {
	double total;
	for (int s = 0; s < globalPool.speciesList.size(); s++) {
		Species currSpecies = globalPool.speciesList[s];
		total = total + currSpecies.averageFitness;
	}
	return total;
}

void cullSpecies(bool cutToOne) {
	for (int s = 0; s < globalPool.speciesList.size(); s++) {
		Species currSpecies = globalPool.speciesList[s];

		struct by_out {
			bool operator()(Genome a, Genome b) {
				return a.fitness > b.fitness;
			}
		};
		std::sort(currSpecies.genomeList.begin(), currSpecies.genomeList.end(), by_out());

		int remaining = ceil((double)currSpecies.genomeList.size() / 2.0);
		if (cutToOne) {
			remaining = 1;
		}
		while (currSpecies.genomeList.size() > remaining) {
			currSpecies.genomeList.pop_back();
		}
	}
}

Genome breedChild(Species currSpecies) {
	Genome child;
	if (((double)rand() / (double)RAND_MAX) < CrossoverChance) {
		Genome g1 = currSpecies.genomeList[((double)rand() / (double)RAND_MAX) * currSpecies.genomeList.size()];
		Genome g2 = currSpecies.genomeList[((double)rand() / (double)RAND_MAX) * currSpecies.genomeList.size()];
		child = crossover(g1, g2);
	}
	else {
		Genome g = currSpecies.genomeList[((double)rand() / (double)RAND_MAX) * currSpecies.genomeList.size()];
		child = copyGenome(g);
	}
	mutate(child);
	return child;
}