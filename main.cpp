#include <vector>
#include <iostream>

#include "hashlife.hpp"

int main()
{
	hashlife::Cache cache;

	std::vector<hashlife::Node*> level_0;
	std::vector<hashlife::Node*> level_1;
	std::vector<hashlife::Node*> level_2;
	std::vector<hashlife::Node*> level_3;

	for (int i = 0; i < 256; i++)
		level_0.push_back(cache.create(0,
					       NULL, NULL, NULL, NULL,
					       rand() % 2));

	for (int i = 0; i < 64; i++)
		level_1.push_back(cache.create(1,
					       level_0[4*i], level_0[4*i+1], level_0[4*i+2], level_0[4*i+3],
					       false));

	for (int i = 0; i < 16; i++)
		level_2.push_back(cache.create(2,
					       level_1[4*i], level_1[4*i+1], level_1[4*i+2], level_1[4*i+3],
					       false));

	for (int i = 0; i < 4; i++)
		level_3.push_back(cache.create(3,
					       level_2[4*i], level_2[4*i+1], level_2[4*i+2], level_2[4*i+3],
					       false));

	hashlife::Node* level_4 = cache.create(4, level_3[0], level_3[1], level_3[2], level_3[3], false);
	level_4->print();
	std::cout << std::endl;
	level_4->result->print();
}
