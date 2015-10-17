#include "stdafx.h"
#include "Gene.cpp"
#include <vector>
class Neuron
{
public:
	std::vector<Gene> incoming;
	//Line 321 of seth's code indicates it's a Gene array

	double value;

	Neuron()
	{
		value = 0.0;
		//	incoming = 0; //subject to change... should remove
	}
};
