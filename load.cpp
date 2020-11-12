#include "hashlife.hpp"

#include <string>
#include <iostream>
#include <fstream>

using namespace hashlife;

Node* Cache::loadFromFile(std::string path)
{
	std::string line;
	std::ifstream file { path };

	assert(file.is_open());

	while (getline(file, line)) {
		std::cout << line << std::endl;
	}
	
	
	return new Node{};
}
