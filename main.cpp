#include <vector>
#include <iostream>
#include <cassert>

#include "hashlife.hpp"

/*
   +-------+-------+-------+
   |       |       |       |
   |   + - - - + - - - +   |
   |       |       |       |
   +---|---a---|---b---|---+
   |       |       |       |
   |   + - - - n - - - +   |
   |       |       |       |
   +---|---c---|---d---|---+
   |       |       |       |
   |   + - - - + - - - +   |
   |       |       |       |
   +-------+-------+-------+

 * n is duplicated to fill a 3x3
 * (n, n, n, n)->result ==> a, b, c, d
 * (a, b, c, d)->result ==> new n
 * 
 * a, b, c, d are all the same
 */

int main(int argc, char* argv[])
{
	assert(argc == 2);
	std::string path { argv[1] };
	
	hashlife::Cache cache;

	hashlife::Node* n = cache.load_from_file(path);
	n->print();
	std::cout << std::endl << std::endl;

        for (int i = 0; i < 64; i++) {
		hashlife::Node* a =
			cache.create(n->level + 1, n, n, n, n, false)->result;
		n = cache.create(a->level + 1, a, a, a, a, false)->result;
		n->print();
		std::cout << std::endl << std::endl;
	}
}
