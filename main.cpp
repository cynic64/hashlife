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

	World(int width, int height);
	void print();
	void randomize();
	void gen();
	int countNeighbors(int x, int y);
};

int main()
{
	World world(60, 61);

	world.randomize();
	world.print();
	std::cout << std::endl;

	for (int i = 0; i < 64; i++) {
		world.gen();
		world.print();
		std::cout << std::endl;
	}
}

World::World(int width, int height)
{
	this->width = width;
	this->height = height;
	
	grid = std::vector<std::vector<bool>>
		(height, std::vector<bool>(width, false));
	next = std::vector<std::vector<bool>>
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

int World::countNeighbors(int x, int y)
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

void World::gen()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			bool current = grid[y][x];
			int count = countNeighbors(x, y);
			
			if (current) {
				if (count == 2 || count == 3) next[y][x] = true;
				else next[y][x] = false;
			} else {
				if (count == 3) next[y][x] = true;
				else next[y][x] = false;
			}
		}
	}

	auto temp = next;
	next = grid;
	grid = temp;
}
