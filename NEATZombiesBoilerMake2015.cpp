// NEATZombiesBoilerMake2015.cpp : main project file.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define BOXRADIUS 6
#define BUTTONS 13
using namespace std;
using namespace System;

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Hello World");
    return 0;
}


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
pool globalPool;

int main()
{
	std::cout << "Hello World!";

}

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



class pool {
public:
	species spec;
	int generation;
	int innovation; //size of ControlNames array 
	int currentSpecies;
	int currentGenome;
	int currentFrame;
	int maxFitness;
	pool() {
		species spec;
		generation = 0;
		innovation = BUTTONS; //size of ControlNames array 
		currentSpecies = 1;
		currentGenome = 1;
		currentFrame = 0;
		maxFitness = 0;
	}

};

class species {
public:

	int topFitness;
	int staleness;
	std::vector<genome> genomeList;
	int averageFitness;
	species() {
		topFitness = 0;
		staleness = 0;
		averageFitness = 0;
	}
};

class genome
{
public:
	std::vector<gene> geneList;
	int fitness;
	int adjustedFitness;
	network genomeNetwork;
	int maxNeuron;
	double mutationRates[7];

	genome()
	{
		vector<gene> geneList{};
		fitness = 0;
		adjustedFitness = 0;
		genomeNetwork;
		maxNeuron = 0;
		double mutationRates[7] = {
			MutateConnectionsChance,//connections
			LinkMutationChance,     //link
			BiasMutationChance,		//bias
			NodeMutationChance,		//node
			EnableMutationChance,	//enable
			DisableMutationChance,	//disable
			StepSize				//step
		};
	}

	//Useful source: http://en.cppreference.com/w/cpp/language/copy_constructor
	genome(const genome& genomeToCopy)
	{
		//Below might be useful for making a copy method instead (see the gene class line 194-200)
		//genome copyOfGenome = new genome();
		for (int x = 1; x < genomeToCopy.geneList.capacity; x++)
		{
			//need "table" to store gene values here. See line 245-247 http://pastebin.com/ZZmSNaHX
		}

		//copying attributes
		geneList = genomeToCopy.geneList;
		maxNeuron = genomeToCopy.maxNeuron;
		//copies the genomeToCopy mutation rates into the new genome. See
		//http://stackoverflow.com/questions/16137953/is-there-a-function-to-copy-an-array-in-c-c answer 2
		std::copy(std::begin(genomeToCopy.mutationRates), std::end(genomeToCopy.mutationRates), std::begin(mutationRates));
	}

	genome(string basic) //equivalent of basicGenome line 263
	{
		genome jean = genome();
		int innovation = 1;
		maxNeuron = Inputs;
		mutate(jean);    //need to write method!
	}
};



class gene {
public:
	int into;
	int out;
	double weight;
	bool enabled;
	int innovation;

	gene()
	{
		int into = 0;
		int out = 0;
		double weight = 0.0;
		bool enabled = true;
		int innovation = 0;
	}

	//the "Copy" overloaded constructors might be better off as methods,
	//in Sethbling's script the copy is returned.
	gene(const gene& geneToCopy)
	{
		//gene copyOfGene = new gene();  //<< this would be useful if we wanted to use a method
		//we would then return copy of gene after assigning object values as so:
		//copyOfGene.out = geneToCopy.out;

		//copying attributes
		into = geneToCopy.into;
		out = geneToCopy.out;
		weight = geneToCopy.weight;
		enabled = geneToCopy.enabled;
		innovation = geneToCopy.innovation;
	}


};

class neuron
{
public:
	std::vector<gene> incoming;
	//Line 321 of seth's code indicates it's a gene array

	double value;

	neuron()
	{
		value = 0.0;
		//	incoming = 0; //subject to change... should remove
	}
};


class network
{
public:
	vector<neuron> neuronList{ MaxNodes + BUTTONS };
	//not sure how he uses the neurons object in LUA need to investigate.

	network() {}

	network(genome genomeForNetwork)
	{
		vector<neuron> temp;
		temp.assign((BOXRADIUS * 2 + 1)*(BOXRADIUS * 2 + 1), neuron());
		neuronList = temp;

		for (int o = 1; o <= MaxNodes; o++)
		{
			neuronList.at(MaxNodes + o) = neuron();
		}

		/*
		TODO:
		(311)
		sort genomeForNetwork.geneList in terms of geneList.out values

		table.sort(genome.genes, function (a,b)
		return (a.out < b.out)
		end)
		Figured out below:
		http://stackoverflow.com/questions/1723066/c-stl-custom-sorting-one-vector-based-on-contents-of-another
		*/
		struct by_out {
			bool operator()(gene a, gene b) {
				return a.out < b.out;
			}
		};
		std::sort(neuronList.begin(), neuronList.end(), by_out());

		for (int i = 0; i < genomeForNetwork.geneList.size(); i++)
		{
			gene currGene = genomeForNetwork.geneList[i];
			if (currGene.enabled)
			{
				/*
				this check may be unneccesary
				http://stackoverflow.com/questions/2099882/checking-for-a-null-object-in-c
				if (neuronList[currGene.out] == NULL) {
				neuronList[currGene.out] = neuron();
				}
				*/
				neuron tempNeuron = neuronList[currGene.out];
				tempNeuron.incoming.push_back(currGene);
			}
		}
		genomeForNetwork.genomeNetwork = *this;
	}
};

std::vector<double> evaluateNetwork(network currNet, std::vector<double> currInputs) {
	currInputs.push_back(1.0);

	if (currInputs.size() != Inputs) {
		cout << "Incorrect number of neural network inputs.\n";
		return;
	}
	for (int i = 0; i < Inputs; i++)
	{
		currNet.neuronList[i].value = currInputs[i];
	}
	//http://www.lua.org/pil/7.2.html
	//don't know how to replicate this:
	//for _,neuron in pairs(network.neurons) do
	for (int i = 0; i < currNet.neuronList.size(); i++) {
		int tempSum = 0;
		for (int j = 0; j < currNet.neuronList[i].incoming.size(); j++) {
			gene tempIncoming = currNet.neuronList[i].incoming[j];
			neuron other = currNet.neuronList[tempIncoming.into];
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

genome crossover(genome g1, genome g2) {
	if (g2.fitness > g1.fitness) {
		genome tempg = g1;
		g1 = g2;
		g2 = tempg;
	}

	genome child = genome();

	std::vector<gene> innovations2;
	for (int i = 0; i < g2.geneList.size(); i++) {
		gene tempGene = g2.geneList[i];
		innovations2[tempGene.innovation] = tempGene;
	}

	for (int i = 0; i < g1.geneList.size(); i++) {
		gene gene1 = g1.geneList[i];
		gene gene2 = innovations2[gene1.innovation];
		if (rand() % 2 == 1 && gene2.enabled) {
			child.geneList.push_back(gene(gene2));
		}
		else {
			child.geneList.push_back(gene(gene1));
		}
	}
	child.maxNeuron = max(g1.maxNeuron, g2.maxNeuron);

	for (int i = 0; i < 7; i++) { //7 is number of mutation rates (231)
		child.mutationRates[i] = g1.mutationRates[i];
	}

	return child;
}

int randomNeuron(std::vector<gene> genes, bool nonInput) {
	std::vector<bool> neurons;
	if (!nonInput) {
		for (int i = 0; i < Inputs; i++) {
			neurons[i] = true;
		}
	}

	for (int o = 1; o < BUTTONS; o++) {
		neurons[MaxNodes + o] = true;
	}
	for (int i = 0; i < genes.size(); i++) {
		if (!nonInput || genes[i].into > Inputs) {
			neurons[genes[i].into] = true;
		}
		if (!nonInput || genes[i].out > Inputs) {
			neurons[genes[i].out] = true;
		}
	}
	int count = 0;
	for (int i = 0; i < neurons.size(); i++) {
		count = count + 1;
	}
	int n = rand() % count + 1;

	for (int i = 0; i < neurons.size(); i++) {
		n = n - 1;
		if (n == 0) {
			return i;
		}
	}
	return 0;
}

bool containsLink(std::vector<gene> genes, gene link) {
	for (int i = 0; i < genes.size(); i++) {
		gene tempGene = genes[i];
		if (tempGene.into == link.into && tempGene.out == link.out) {
			return true;
		}
	}
}

void pointMutate(genome currGenome) {
	double step = currGenome.mutationRates[6];

	for (int i = 0; i < currGenome.geneList.size(); i++) {
		gene tempGene = currGenome.geneList[i];
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

void linkMutate(genome currGenome, bool forceBias) {
	int neuron1 = randomNeuron(currGenome.geneList, false);
	int neuron2 = randomNeuron(currGenome.geneList, true);

	gene newLink = gene();
	if (neuron1 <= Inputs && neuron2 <= Inputs) {
		return;
	}
	if (neuron2 <= Inputs) {
		int temp = neuron1;
		neuron1 = neuron2;
		neuron2 = temp;
	}

	newLink.into = neuron1;
	newLink.out = neuron2;

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

void nodeMutate(genome currGenome) {
	if (currGenome.geneList.size() == 0) {
		return;
	}
	currGenome.maxNeuron = currGenome.maxNeuron + 1;
	gene currGene = currGenome.geneList[rand() % currGenome.geneList.size() + 1];
	if (currGene.enabled) {
		return;
	}
	currGene.enabled = false;

	gene gene1 = gene(currGene);
	gene1.out = currGenome.maxNeuron;
	gene1.weight = 1.0;
	gene1.innovation++;
	gene1.enabled = true;
	currGenome.geneList.push_back(gene1);

	gene gene2 = gene(currGene);
	gene2.into = currGenome.maxNeuron;
	gene2.innovation++;
	gene2.enabled = true;
	currGenome.geneList.push_back(gene2);

}

void enableDisableMutate(genome currGenome, bool enable) {
	std::vector<gene> candidates;
	gene tempGene;
	for (int i = 0; i < currGenome.geneList.size(); i++) {
		tempGene = currGenome.geneList[i];
		if (tempGene.enabled == !enable) {
			candidates.push_back(tempGene);
		}
	}
	if (candidates.size() == 0) {
		return;
	}
	gene randGene = candidates[rand() % candidates.size()];
	randGene.enabled = !randGene.enabled;
}

void mutate(genome currGenome) {
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
double disjoint(std::vector<gene> genes1, std::vector<gene> genes2) {
	std::vector<bool> i1;
	for (int i = 0; i < genes1.size(); i++) {
		gene currGene = genes1[i];
		i1[currGene.innovation] = true;
	}

	std::vector<bool> i2;
	for (int i = 0; i < genes2.size(); i++) {
		gene currGene = genes2[i];
		i2[currGene.innovation] = true;
	}

	int disjointGenes = 0;
	for (int i = 0; i < genes1.size(); i++) {
		gene currGene = genes1[i];
		if (!i2[currGene.innovation]) {
			disjointGenes = disjointGenes + 1;
		}
	}
	for (int i = 0; i < genes2.size(); i++) {
		gene currGene = genes2[i];
		if (!i1[currGene.innovation]) {
			disjointGenes = disjointGenes + 1;
		}
	}
	return (double)disjointGenes / (double)std::max(genes1.size(), genes2.size());
}

//621
double weights(std::vector<gene> genes1, std::vector<gene> genes2) {
	std::vector<gene> i2;
	for (int i = 0; i < genes2.size(); i++) {
		gene currGene = genes2[i];
		i2[currGene.innovation] = currGene;
	}
	int sum = 0;
	int coincident = 0;
	for (int i = 0; i < genes1.size(); i++) {
		gene currGene = genes1[i];
		//not needed? if (i2[gene.innovation] != NULL) {
		gene currGene2 = i2[currGene.innovation];
		sum = sum + std::abs(currGene.weight - currGene2.weight);
		coincident = coincident + 1;
		//}
	}
	return sum / coincident;
}

double sameSpecies(genome genome1, genome genome2) {
	double dd = DeltaDisjoint*disjoint(genome1.geneList, genome2.geneList);
	double dw = DeltaWeights*weights(genome1.geneList, genome2.geneList);
	return dd + dw < DeltaThreshold;

}

void rankGlobally() {
	std::vector<genome> global;
	for (int s = 0; s < globalPool.genomeList.size(); s++) {
		species currSpecies = globalPool.species[s];
		for (int g = 0; g < currSpecies.genomeList.size(); g++) {
			global.push_back(currSpecies.genomeList[g]);
		}
	}
	struct by_out {
		bool operator()(gene a, gene b) {
			return a.out < b.out;
		}
	};
	std::sort(neuronList.begin(), neuronList.end(), by_out());


}