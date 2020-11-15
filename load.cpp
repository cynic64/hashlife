#include "hashlife.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>

using namespace hashlife;

// Returns a Node given a 2D vector of bools and a cache
Node* nodify(Cache cache, std::vector<std::vector<bool>> grid)
{
	assert(grid.size() == grid[0].size());

	// Edge case: level 0
	if (grid.size() == 1)
		return cache.create(0, NULL, NULL, NULL, NULL, grid[0][0]);

	// Split into 4 quadrants and recursively nodify
	size_t half = grid.size() / 2;

	std::vector<std::vector<bool>> a;
	std::vector<std::vector<bool>> b;
	std::vector<std::vector<bool>> c;
	std::vector<std::vector<bool>> d;

	for (size_t y = 0; y < half; y++) {
		std::vector<bool> a_row;
		std::vector<bool> b_row;
		std::vector<bool> c_row;
		std::vector<bool> d_row;
		
		for (size_t x = 0; x < half; x++) {
			a_row.push_back(grid[y][x]);
			b_row.push_back(grid[y][x+half]);
			c_row.push_back(grid[y+half][x]);
			d_row.push_back(grid[y+half][x+half]);
		}

		a.push_back(a_row);
		b.push_back(b_row);
		c.push_back(c_row);
		d.push_back(d_row);
	}

	Node* n_a = nodify(cache, a);
	Node* n_b = nodify(cache, b);
	Node* n_c = nodify(cache, c);
	Node* n_d = nodify(cache, d);

	return cache.create(n_a->level+1, n_a, n_b, n_c, n_d, false);
}

Node* Cache::load_from_file(std::string path)
{
	// Load file into 2D bool array
	std::vector<std::vector<bool>> grid;
	
	std::string line;
	std::ifstream file { path };

	assert(file.is_open());

	while (getline(file, line)) {
		std::vector<bool> row;
		
		for (char c : line) {
			assert(c == '.' || c == '#');
			row.push_back(c == '#');
		}
		
		grid.push_back(row);
	}

	// Make sure all row lengths match
	size_t width = grid[0].size();
	for (auto row : grid) assert(row.size() == width);

	// Pad to a square with a side length that is a power of 2
	size_t height = grid.size();
	size_t side_len = 1;
	while (side_len < std::max(width, height)) side_len *= 2;

	for (auto row = grid.begin(); row != grid.end(); ++row) {
		while (row->size() < side_len) row->push_back(false);
	}

	while (grid.size() < side_len)
		grid.push_back(std::vector<bool>(side_len, false));
	
	return nodify(*this, grid);
}
