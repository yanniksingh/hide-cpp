#ifndef HIDE_H
#define HIDE_H

#include <string>
#include <vector>

namespace hide {
	void embed(std::vector<uint8_t>& image, const std::string& message, const std::string& key);
	std::string extract(const std::vector<uint8_t>& image, const std::string& key);
}

#endif
