#include <vector>
#include <iostream>

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

int main()
{
	hashlife::Cache cache;

	hashlife::Node* n = cache.loadFromFile("pattern.cells");
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
