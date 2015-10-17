#include "stdafx.h"
#include "Species.cpp"
#define BUTTONS 13

class Pool {
public:
	std::vector<Species> speciesList;
	int generation;
	int innovation; //size of ControlNames array 
	int currentSpecies;
	int currentGenome;
	int currentFrame;
	int maxFitness;
	Pool() {
		std::vector<Species> speciesList;
		generation = 0;
		innovation = BUTTONS; //size of ControlNames array 
		currentSpecies = 1;
		currentGenome = 1;
		currentFrame = 0;
		maxFitness = 0;
	}

};
