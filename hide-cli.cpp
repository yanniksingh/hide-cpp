#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#define STBI_ONLY_PNG
#include "stb/stb_image.h"
 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "hide.hpp"
#include <string>
#include <iostream>

void embedToFile(const std::string& file, const std::string& key, const std::string& message);
void extractFromFile(const std::string& file, const std::string& key);

int main(int argc, char* argv[]) {
	std::string execName = argv[0];
	std::string embedUsage = execName + " embed [file] [key] [message]"; 
	std::string extractUsage = execName + " extract [file] [key]";
	
	if (argc > 1) {
		std::string subcommand = argv[1];
		//Wrap this in a try-catch to report errors to the user through stderr
		if (subcommand == "embed") {
			if (argc == 5) {
				embedToFile(argv[2], argv[3], argv[4]);
				return 0;
			}
			else {
				//Show embedding-specific help information
				std::cerr << "usage: " << embedUsage << std::endl;
				return 1;
			}
		}
		else if (subcommand == "extract") {
			if (argc == 4) {
				extractFromFile(argv[2], argv[3]);
				return 0;
			}
			else {
				//Show extraction-specific help information
				std::cerr << "usage: " << extractUsage << std::endl;
				return 1;
			}
		}
	}

	//Show general help information
	std::cerr << "description: hide data in images with steganography" << std::endl << std::endl;
	std::cerr << "usage: " << std::endl << '\t' << embedUsage << std::endl << '\t' << extractUsage << std::endl << std::endl;
	std::cerr << "documentation: https://github.com/yanniksingh/hide-cpp" << std::endl;
	return 1;
}

void embedToFile(const std::string& file, const std::string& key, const std::string& message) {
	//Perform embedding, either printing nothing or a brief success message to stderr (not which is best)
}

void extractFromFile(const std::string& file, const std::string& key) {
	//Perform extraction, printing the message to stdout
}
