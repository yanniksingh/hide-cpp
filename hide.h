#ifndef HIDE_H
#define HIDE_H

#include <random>

namespace hide {
	//Returns a vector containing a pseudo-random permutation of [0,1,...,length-1]
	std::vector<int>  random_permutation(int length, std::string seed) {
		//Create a vector containing [0,1,...length-1]
		std::vector<int> sequence(length);
		for (int i = 0; i < length; i++) {
			sequence[i] = i;
		}
		
		//Initialize the pseudo-random number generator
		std::seed_seq seedSequence(seed.begin(), seed.end());	
		std::mt19937 mt (seedSequence);

		//Shuffle the vector (Fisher-Yates shuffle)
		for (int i = 0; i < length - 1; i++) {
			//Generate a random number between i and length-1, inclusive
			std::uniform_int_distribution<int> uniform(i, length-1);
			int j = uniform(mt);

			//Swap sequence[i] and sequence[j]
			int temp = sequence[i];
			sequence[i] = sequence[j];
			sequence[j] = temp;
		}
		return sequence;
	}
}

#endif
