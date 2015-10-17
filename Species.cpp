#include "stdafx.h"
#include "NEATZombiesBoilerMake2015.h"
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