#include<iostream>
#include<vector>
#include<array>
#include<cstdlib>

const std::array<std::array<int,2>, 8> NEIGHBOR_OFFSETS {{
	{{-1,-1}},
	{{0,-1}},
	{{1,-1}},
	{{-1,0}},
	{{1,0}},
	{{-1,1}},
	{{0,1}},
	{{1,1}}
	}};

class World
{
	std::vector<std::vector<bool>> grid;
	std::vector<std::vector<bool>> next;
public:
	World(int width, int height);
	void print();
	void randomize();
	int count_neighbors(int x, int y);
};

int main()
{
	World world(8, 8);

	world.print();
	std::cout << "Randomizing..." << std::endl;
	world.randomize();
	world.print();
}

World::World(int width, int height)
{
	grid = std::vector<std::vector<bool>>
		(height, std::vector<bool>(width, false));
}

void World::print()
{
	for (auto row : grid) {
		for (auto cell : row) {
			std::cout << (cell ? "# " : ". ");
		}
		std::cout << std::endl;
	}
}

void World::randomize()
{
	for (auto row = grid.begin(); row != grid.end(); ++row) {
		for (auto cell = row->begin(); cell != row->end(); ++cell) {
			*cell = rand() % 2;
		}
	}

}

int World::count_neighbors(int x, int y)
{
	
}
