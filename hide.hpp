#ifndef HIDE_H
#define HIDE_H

#include <string>
#include <vector>

namespace hide {
	/*
	Modifies image to contain message such that it can be recovered with key.

	Will throw an exception if message contains embedded null characters or is too large to fit in image.
	*/
	void embed(std::vector<unsigned char>& image, const std::string& key, const std::string& message);

	/*
	Recovers an embedded message from image given the key.
	Returns the recovered message.
	*/	
	std::string extract(const std::vector<unsigned char>& image, const std::string& key);
}

#endif
