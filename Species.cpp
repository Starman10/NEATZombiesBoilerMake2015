#include "stdafx.h"
#include "Genome.cpp"
#include <vector>

class Species {
public:

	double topFitness;
	double staleness;
	std::vector<Genome> GenomeList;
	double averageFitness;
	Species() {
		topFitness = 0;
		staleness = 0;
		averageFitness = 0;
	}
};