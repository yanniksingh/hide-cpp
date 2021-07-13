#include "hide.hpp"
#include <stdexcept>
#include <random>
#include <string>
#include <vector>
#include <bitset>

namespace hide {
	
	/*
	Returns a pseudo-random permutation of {0, 1, ... , length - 1} generated with the given seed.
	*/
	static std::vector<size_t> random_permutation(size_t length, const std::string& seed);

	/*
	Embeds a single bit of message (given by symbol) in one pixel of an image.
	The value of random should be chosen uniformly at random each time the function is called.
	*/
	static void embedSymbol(unsigned char& pixel, bool symbol, bool random);

	/*
	Extracts a single bit of message from one pixel of an image.
	*/
	static bool extractSymbol(unsigned char pixel);
	
	void embed(std::vector<unsigned char>& image, const std::string& key, const std::string& message) {
		/*
		High-level overview: Use LSB Matching to embed the bits of message in the pixels of image.
		The key determines the order in which the pixels of image are visited.
		*/

		if (message.find('\0') != std::string::npos) throw std::invalid_argument("Message may not contain embedded null characters");

		//Append a null character, which indicates the end of the message during extraction
		std::string messagePlusTerminator = message;
		messagePlusTerminator.push_back('\0');

		if (messagePlusTerminator.size() * CHAR_BIT > image.size()) throw std::invalid_argument("Message is too large to embed in image"); 

		//The key determines the order in which the pixels of image are modified to contain the bits of message
		std::vector<size_t> permutation = random_permutation(image.size(), key);
		size_t positionInPermutation = 0;

		std::random_device rng;
		std::uniform_int_distribution<int> coinFlip(0,1);

		for (char currentChar : messagePlusTerminator) {
			std::bitset<CHAR_BIT> bitsForChar (static_cast<unsigned char>(currentChar));
			for (size_t bit = 0; bit < CHAR_BIT; bit++) {
				embedSymbol(image[permutation[positionInPermutation]], bitsForChar.test(bit), coinFlip(rng));
				positionInPermutation++;
			}
		}
	}
	
	std::string extract(const std::vector<unsigned char>& image, const std::string& key) {
		std::string messagePlusTerminator;

		//Just like in embedding, the key determines the order in which the pixels of image are read
		std::vector<size_t> permutation = random_permutation(image.size(), key);
		size_t positionInPermutation = 0;

		//Read from the image, stopping when the null character is reached or all pixels are read
		while (positionInPermutation < image.size() && (messagePlusTerminator.empty() || messagePlusTerminator.back() != '\0')) {
			std::bitset<CHAR_BIT> bitsForChar;
			for (size_t bit = 0; bit < CHAR_BIT; bit++) {
				bitsForChar.set(bit, extractSymbol(image[permutation[positionInPermutation]]));
				positionInPermutation++;
				if (positionInPermutation >= image.size()) break; //Reached the end of the image
			}
			messagePlusTerminator.push_back(static_cast<char>(bitsForChar.to_ulong()));
		}
		
		//The message still contains its terminating null character, which should not be returned
		messagePlusTerminator.pop_back();
		
		return messagePlusTerminator;
	}

	static std::vector<size_t> random_permutation(size_t length, const std::string& seed) {
		assert(length > 0);

		std::vector<size_t> sequence(length);
		for (size_t i = 0; i < length; i++) {
			sequence[i] = i;
		}
		
		std::seed_seq seedSequence(seed.begin(), seed.end());	
		std::mt19937 mt (seedSequence);

		//Shuffle the vector with the Fisher-Yates shuffle
		for (size_t i = 0; i < length - 1; i++) {
			std::uniform_int_distribution<size_t> uniform(i, length-1);
			size_t j = uniform(mt);

			size_t temp = sequence[i];
			sequence[i] = sequence[j];
			sequence[j] = temp;
		}
		return sequence;
	}
	
	static void embedSymbol(unsigned char& pixel, bool symbol, bool random) {
		//High-level overview: The pixel is modified such that its least significant bit is symbol.
		if (symbol == extractSymbol(pixel)) {
			return;
		}
		else if (pixel == 0) {
			pixel++;
		}
		else if (pixel == UCHAR_MAX) {
			pixel--;
		}
		else {
			pixel = random ? (pixel + 1) : (pixel - 1);
		}
	}

	static bool extractSymbol(unsigned char pixel) {
		return ((pixel & 1) != 0);
	}
}
