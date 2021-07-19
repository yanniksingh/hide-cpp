#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#define STBI_ONLY_PNG
#include "stb/stb_image.h"
 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "hide.hpp"
#include <string>
#include <iostream>
#include <stdexcept>

/*
Modifies the image located at file to contain message such that it can be recovered with key.
*/
void embedToFile(const std::string& file, const std::string& key, const std::string& message);

/*
Given the key, recovers and returns the message embedded in the image located at file.
*/
std::string extractFromFile(const std::string& file, const std::string& key);

int main(int argc, char* argv[]) {
	std::string execName = argc > 0 ? argv[0] : "";
	std::string embedUsage = execName + " embed [image-file] [password] [data]"; 
	std::string extractUsage = execName + " extract [image-file] [password]";
	
	if (argc > 1) {
		std::string subcommand = argv[1];
		try {
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
					std::cout << extractFromFile(argv[2], argv[3]) << std::endl;
					return 0;
				}
				else {
					//Show extraction-specific help information
					std::cerr << "usage: " << extractUsage << std::endl;
					return 1;
				}
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}

	//Show general help information
	std::cerr << "description: a tool for hiding data in images" << std::endl << std::endl;
	std::cerr << "usage: " << std::endl << '\t' << embedUsage << std::endl << '\t' << extractUsage << std::endl << std::endl;
	std::cerr << "documentation: https://github.com/yanniksingh/hide-cpp" << std::endl;
	return 1;
}

/*
Provides access to the pixel data of a PNG image on the file system.
*/
class PNG {
private:
        int width, height, channels;
        unsigned char* pixelData;
public:
        std::string filePath;
        std::vector<unsigned char> pixelArray;

        PNG(const std::string& file) {
                pixelData = stbi_load(file.c_str(), &width, &height, &channels, 0); 
                if (pixelData == NULL) throw std::runtime_error(stbi_failure_reason()); 
                pixelArray.assign(pixelData, pixelData + width * height * channels);
                filePath = file;
        }   

        ~PNG() {
                stbi_image_free(pixelData);
        }   

	/*
	Saves in-memory data to the file system.
	*/
        void Save() {
                int success = stbi_write_png(filePath.c_str(), width, height, channels, &pixelArray[0], width * channels);
                if (success == 0) throw std::runtime_error("Could not save image");
        }   
};

void embedToFile(const std::string& file, const std::string& key, const std::string& message) {
        PNG image(file);
        hide::embed(image.pixelArray, key, message);
        image.Save();
}

std::string extractFromFile(const std::string& file, const std::string& key) {
        PNG image(file);
        return hide::extract(image.pixelArray, key);
}
