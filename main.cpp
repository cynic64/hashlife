#include<iostream>
#include<vector>

class World {
	std::vector<std::vector<bool>> grid;
public:
	World(int width, int height);
	void print();
};

int main() {
	World world(8, 8);

	world.print();
}

World::World(int width, int height) {
	grid = std::vector<std::vector<bool>> (height, std::vector<bool>(width));
}

void World::print() {
	for (auto row : grid) {
		for (auto cell : row) {
			std::cout << (cell ? "# " : ". ");
		}
		std::cout << std::endl;
	}
}
