#include<iostream>
#include<vector>
#include<cstdlib>

class World {
	std::vector<std::vector<bool>> grid;
	std::vector<std::vector<bool>> next;
public:
	World(int width, int height);
	void print();
	void randomize();
};

int main() {
	World world(8, 8);

	world.print();
	std::cout << "Randomizing..." << std::endl;
	world.randomize();
	world.print();
}

World::World(int width, int height) {
	grid = std::vector<std::vector<bool>>
		(height, std::vector<bool>(width, false));
}

void World::print() {
	for (auto row : grid) {
		for (auto cell : row) {
			std::cout << (cell ? "# " : ". ");
		}
		std::cout << std::endl;
	}
}

void World::randomize() {
	for (auto row = grid.begin(); row != grid.end(); ++row) {
		for (auto cell = row->begin(); cell != row->end(); ++cell) {
			*cell = rand() % 2;
		}
	}

}
