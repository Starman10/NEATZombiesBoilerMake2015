#include "stdafx.h"
#include "Gene.cpp"
#include "Network.cpp"
#include <vector>
#include <string.h>

#define BOXRADIUS 6
#define BUTTONS 13
#define INPUTS 170


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
		Genome jean = Genome();
		int innovation = 1;
		maxNeuron = INPUTS;
		mutate(jean);    //need to write method!
	}
};
