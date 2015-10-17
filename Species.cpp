#include "stdafx.h"
#include "Genome.cpp"
#include <vector>

class Species {
public:

	int topFitness;
	int staleness;
	std::vector<Genome> GenomeList;
	int averageFitness;
	Species() {
		topFitness = 0;
		staleness = 0;
		averageFitness = 0;
	}
};