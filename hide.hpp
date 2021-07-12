#ifndef HIDE_H
#define HIDE_H

#include <string>
#include <vector>

namespace hide {
	void embed(std::vector<unsigned char>& image, const std::string& key, const std::string& message);
	std::string extract(const std::vector<unsigned char>& image, const std::string& key);
}

#endif
