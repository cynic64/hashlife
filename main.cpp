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
	int width, height;

	World(int width_, int height_);
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

	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			std::cout << world.count_neighbors(x, y) << " ";
		}
		std::cout << std::endl;
	}
}

World::World(int width_, int height_)
{
	width = width_;
	height = height_;
	
	grid = std::vector<std::vector<bool>>
		(height, std::vector<bool>(width, false));
}

void World::print()
{
	std::cout << "Width: " << width << std::endl;
	
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
	int count = 0;
	
	for (auto offset : NEIGHBOR_OFFSETS) {
		int nx = x + offset[0];
		int ny = y + offset[1];
		if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
		if (grid[ny][nx]) count++;
	}

	return count;
}
