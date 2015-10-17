#include "stdafx.h"

class Gene {
public:
	int into;
	int out;
	double weight;
	bool enabled;
	int innovation;

	Gene()
	{
		int into = 0;
		int out = 0;
		double weight = 0.0;
		bool enabled = true;
		int innovation = 0;
	}

	//the "Copy" overloaded constructors might be better off as methods,
	//in Sethbling's script the copy is returned.
	Gene(const Gene& GeneToCopy)
	{
		//Gene copyOfGene = new Gene();  //<< this would be useful if we wanted to use a method
		//we would then return copy of Gene after assigning object values as so:
		//copyOfGene.out = GeneToCopy.out;

		//copying attributes
		into = GeneToCopy.into;
		out = GeneToCopy.out;
		weight = GeneToCopy.weight;
		enabled = GeneToCopy.enabled;
		innovation = GeneToCopy.innovation;
	}


};