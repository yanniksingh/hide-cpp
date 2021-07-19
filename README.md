# hide-cpp

`hide-cpp` is a tool for hiding data in images.

### Features
- Command-line interface
- C++ API

### Details

`hide-cpp` supports two operations: **embedding** data in an image with a password, and **extracting** hidden data from an image (which requires the password).

Hidden data is stored in the least significant bits of image pixel values, using a method known as least significant bit matching. For RGB color images, up to 3 bits of data can be stored per pixel. The password determines how the data is distributed throughout the image. See [here](https://www.cs.ox.ac.uk/andrew.ker/docs/informationhiding-lecture-notes-ht2016.pdf) for more information about hiding data in digital images.

## Getting Started

### Requirements

`hide-cpp` requires a C++ compiler supporting C++11.

### Installation

The command-line interface uses the single-file [stb-image and stb-image-write](https://github.com/nothings/stb) libraries to read and save images. These are downloaded automatically as a submodule.

```bash
git clone --recurse-submodules https://github.com/yanniksingh/hide-cpp

cd hide-cpp

# This step will look different if using g++, MSVC, etc.
clang++ hide.cpp hide-cli.cpp -o hide -std=c++11
```

### Usage

#### Command-line Interface

Currently, the command-line interface supports only PNG images. Other formats may be supported in the future.

```bash
./hide embed [file] [key] [message]

# Prints message
./hide extract [file] [key]
```

#### C++ API

The C++ API represents images as vectors of `unsigned char`. When compiling your code, link to `hide.cpp`.

```cpp
#include "hide-cpp/hide.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	std::string data = "Secret message";
	std::string password = "Secret password";

	//Create an image with random pixel values
	size_t imageSize = 1000;
	std::vector<unsigned char> image(imageSize);
	for (size_t i = 0; i < imageSize; i++) {
		image[i] = rand() % UCHAR_MAX;
	}

	hide::embed(image, password, data);

	//Prints "Secret message"
	std::cout << hide::extract(image, password) << std::endl;
}
```

## License

MIT
