#include "stdafx.h"
#include "NEATZombiesBoilerMake2015.h"
//#define BUTTONS 13

	Pool::Pool() 
	{
		generation = 0;
		std::vector<Species> speciesList;
		innovation = BUTTONS; //size of ControlNames array 
		currentSpecies = 1;
		currentGenome = 1;
		currentFrame = 0;
		maxFitness = 0;
	}
