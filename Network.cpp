#include "stdafx.h"
#include "Neuron.cpp"
#include <vector>
#include <string>
#include <algorithm>
#include "Gene.cpp"

#define BOXRADIUS 6
#define BUTTONS 13
#define MAXNODES 1000000

class Genome
{
public:
	std::vector<Gene> GeneList;
	int fitness;
	int adjustedFitness;
	Network GenomeNetwork;
	int maxNeuron;
	double mutationRates[7];
	double MutateConnectionsChance = 0.25;
	double PerturbChance = 0.90;
	double CrossoverChance = 0.75;
	double LinkMutationChance = 2.0;
	double NodeMutationChance = 0.50;
	double BiasMutationChance = 0.40;
	double StepSize = 0.1;

	Genome()
	{
		std::vector<Gene> GeneList{};
		fitness = 0;
		adjustedFitness = 0;
		GenomeNetwork;
		double MutateConnectionsChance = 0.25;
		double PerturbChance = 0.90;
		double CrossoverChance = 0.75;
		double LinkMutationChance = 2.0;
		double NodeMutationChance = 0.50;
		double BiasMutationChance = 0.40;
		double StepSize = 0.1;
		double DisableMutationChance = 0.4;
		double EnableMutationChance = 0.2;

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
	Genome(const Genome& GenomeToCopy)
	{
		//Below might be useful for making a copy method instead (see the Gene class line 194-200)
		//Genome copyOfGenome = new Genome();
		for (int x = 1; x < GenomeToCopy.GeneList.capacity; x++)
		{
			//need "table" to store Gene values here. See line 245-247 http://pastebin.com/ZZmSNaHX
		}

		//copying attributes
		GeneList = GenomeToCopy.GeneList;
		maxNeuron = GenomeToCopy.maxNeuron;
		//copies the GenomeToCopy mutation rates into the new Genome. See
		//http://stackoverflow.com/questions/16137953/is-there-a-function-to-copy-an-array-in-c-c answer 2
		std::copy(std::begin(GenomeToCopy.mutationRates), std::end(GenomeToCopy.mutationRates), std::begin(mutationRates));
	}

	Genome(string basic) //equivalent of basicGenome line 263
	{
		Genome jeanGnome = Genome();
		int innovation = 1;
		maxNeuron = Inputs;
		mutate(jeanGnome);    //need to write method!
	}
};


class Network
{
public:
	std::vector<Neuron> NeuronList{ MAXNODES + BUTTONS };
	//not sure how he uses the Neurons object in LUA need to investigate.

	Network() {}

	Network(Genome GenomeForNetwork)
	{
		std::vector<Neuron> temp;
		temp.assign((BOXRADIUS * 2 + 1)*(BOXRADIUS * 2 + 1), Neuron());
		NeuronList = temp;

		for (int o = 1; o <= MAXNODES; o++)
		{
			NeuronList.at(MAXNODES + o) = Neuron();
		}

		/*
		TODO:
		(311)
		sort GenomeForNetwork.GeneList in terms of GeneList.out values

		table.sort(Genome.Genes, function (a,b)
		return (a.out < b.out)
		end)
		Figured out below:
		http://stackoverflow.com/questions/1723066/c-stl-custom-sorting-one-vector-based-on-contents-of-another
		*/
		struct by_out {
			bool operator()(Gene a, Gene b) {
				return a.out < b.out;
			}
		};
		std::sort(NeuronList.begin(), NeuronList.end(), by_out());

		for (int i = 0; i < GenomeForNetwork.GeneList.size(); i++)
		{
			Gene currGene = GenomeForNetwork.GeneList[i];
			if (currGene.enabled)
			{
				/*
				this check may be unneccesary
				http://stackoverflow.com/questions/2099882/checking-for-a-null-object-in-c
				if (NeuronList[currGene.out] == NULL) {
				NeuronList[currGene.out] = Neuron();
				}
				*/
				Neuron tempNeuron = NeuronList[currGene.out];
				tempNeuron.incoming.push_back(currGene);
			}
		}
		GenomeForNetwork.GenomeNetwork = *this;
	}
};
