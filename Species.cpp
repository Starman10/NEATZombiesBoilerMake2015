#include "stdafx.h"
#pragma once

#include "NEATZombiesBoilerMake2015.h"


//See: http://stackoverflow.com/questions/13484178/class-type-redefinition-error-between-header-and-source-files
//For why this works & how
	Species::Species() 
	{
		topFitness = 0;
		staleness = 0;
		averageFitness = 0;
	}