#include <vector>
#include <iostream>

#include "hashlife.hpp"

int main()
{
	hashlife::Cache cache;

	hashlife::Node* n = cache.loadFromFile("pattern.cells");
	n->print();
	
	std::cout << std::endl << "Result:" << std::endl;
	n->result->print();
}
