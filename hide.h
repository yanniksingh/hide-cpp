#ifndef HIDE_H
#define HIDE_H

#include <random>
#include <math.h>

namespace hide {
	//Returns a vector containing a pseudo-random permutation of [0,1,...,length-1]
	//Preconditions: length > 0
	std::vector<size_t>  random_permutation(size_t length, const std::string& seed) {
		//Create a vector containing [0,1,...length-1]
		std::vector<size_t> sequence(length);
		for (size_t i = 0; i < length; i++) {
			sequence[i] = i;
		}
		
		//Initialize the pseudo-random number generator
		std::seed_seq seedSequence(seed.begin(), seed.end());	
		std::mt19937 mt (seedSequence);

		//Shuffle the vector (Fisher-Yates shuffle)
		for (size_t i = 0; i < length - 1; i++) {
			//Generate a random number between i and length-1, inclusive
			std::uniform_int_distribution<size_t> uniform(i, length-1);
			size_t j = uniform(mt);

			//Swap sequence[i] and sequence[j]
			size_t temp = sequence[i];
			sequence[i] = sequence[j];
			sequence[j] = temp;
		}
		return sequence;
	}
	
	//Returns the least significant bit of the byte pixel
	bool extractSymbol(uint8_t pixel) {
		return ((pixel & 1) != 0);
	}
	
	//Embeds the bit symbol in the byte pixel such that the least significant bit of pixel is equal to symbol.
	//Implements the least significant bit matching (LSBM) method of steganography.
	void embedSymbol(uint8_t& pixel, bool symbol, bool random) {
		if (symbol == extractSymbol(pixel)) {
			return;
		}
		else if (pixel == 0) {
			pixel++;
		}
		else if (pixel == UINT8_MAX) {
			pixel--;
		}
		else {
			pixel = random ? (pixel + 1) : (pixel - 1);
		}
	}

	//Uses LSB matching steganography to embed the string message within the vector image, with the order of embedding determined by the secret key.
	void embed(std::vector<uint8_t>& image, const std::string& message, const std::string& key) {
		//Append a null terminator to the message
		std::string messagePlusTerminator = message;
		messagePlusTerminator.push_back('\0');

		//Make sure the message can fit in the image; throw an exception if not
		if (messagePlusTerminator.size() * CHAR_BIT > image.size()) return; //TODO: Throw exception

		//Generate permuation
		std::vector<size_t> permutation = random_permutation(image.size(), key);
		size_t positionInPermutation = 0;

		//Create true random number generator
		std::random_device rng;
		std::uniform_int_distribution<int> coinFlip(0,1);

		//Embed message + terminator in image
		for (char currentChar : messagePlusTerminator) {
			std::bitset<CHAR_BIT> bitsForChar (static_cast<unsigned char>(currentChar));
			for (size_t bit = 0; bit < CHAR_BIT; bit++) {
				embedSymbol(image[permutation[positionInPermutation]], bitsForChar.test(bit), coinFlip(rng));
				positionInPermutation++;
			}
		}
	}
	
	//Returns the message embedded in the vector image given by the secret key.
	//Will always return a message for all inputs, even for images which have not been passed to embed() or images passed to embed() with a different secret key.
	std::string extract(const std::vector<uint8_t>& image, const std::string& key) {
		//Initialize string to hold message
		std::string messagePlusTerminator;

		//Generate permutation from key
		std::vector<size_t> permutation = random_permutation(image.size(), key);
		size_t positionInPermutation = 0;

		//Read the message from the image
		while (positionInPermutation < image.size() && (messagePlusTerminator.empty() || messagePlusTerminator.back() != '\0')) {
			std::bitset<CHAR_BIT> bitsForChar;
			for (size_t bit = 0; bit < CHAR_BIT; bit++) {
				bitsForChar.set(bit, extractSymbol(image[permutation[positionInPermutation]]));
				positionInPermutation++;
				if (positionInPermutation >= image.size()) break;
			}
			messagePlusTerminator.push_back(static_cast<char>(bitsForChar.to_ulong()));
		}
		
		//Remove terminator
		messagePlusTerminator.pop_back();
		
		return messagePlusTerminator;
	}
}

#endif
