#include "NEATZombiesBoilerMake2015.h"
#include "stdafx.h"
#include <vector>
#include <string>
#include <algorithm>



class Network
{
public:
	vector<Neuron> neuronList{ MaxNodes + BUTTONS };
	//not sure how he uses the neurons object in LUA need to investigate.

	Network() {}

	Network(Genome genomeForNetwork)
	{
		vector<Neuron> temp;
		temp.assign((BOXRADIUS * 2 + 1)*(BOXRADIUS * 2 + 1), Neuron());
		neuronList = temp;

		for (int o = 1; o <= MaxNodes; o++)
		{
			neuronList.at(MaxNodes + o) = Neuron();
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
			bool operator()(Gene a, Gene b) {
				return a.out < b.out;
			}
		};
		std::sort(neuronList.begin(), neuronList.end(), by_out());

		for (int i = 0; i < genomeForNetwork.geneList.size(); i++)
		{
			Gene currGene = genomeForNetwork.geneList[i];
			if (currGene.enabled)
			{
				/*
				this check may be unneccesary
				http://stackoverflow.com/questions/2099882/checking-for-a-null-object-in-c
				if (neuronList[currGene.out] == NULL) {
				neuronList[currGene.out] = neuron();
				}
				*/
				Neuron tempNeuron = neuronList[currGene.out];
				tempNeuron.incoming.push_back(currGene);
			}
		}
		genomeForNetwork.genomeNetwork = *this;
	}
};