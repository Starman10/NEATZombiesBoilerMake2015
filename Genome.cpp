#include "stdafx.h"
#include "NEATZombiesBoilerMake2015.h"
#include <vector>
#include <string.h>

#define BOXRADIUS 6
#define BUTTONS 13
#define INPUTS 170

	Genome::Genome()
	{
		vector<Gene> geneList{};
		fitness = 0;
		adjustedFitness = 0;
		globalRank = 0;
		genomeNetwork; //needs fixing
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
	Genome::Genome(const Genome& genomeToCopy)
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

	Genome::Genome(string basic) //equivalent of basicGenome line 263
	{
		Genome jean = Genome();
		int innovation = 1;
		maxNeuron = Inputs;
		mutate(jean);    //need to write method!
	}
