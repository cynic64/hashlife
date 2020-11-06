#include<iostream>
#include<cassert>
#include<vector>
#include<array>

typedef std::array<int,2> offset_t;

class Node
{
public:
	int level;
	// Children in reading order
	Node *a, *b, *c, *d;
	// Only used for level-0 nodes
	bool alive;

	Node(bool alive) : level(0),
			    a(NULL), b(NULL), c(NULL), d(NULL),
			    alive(alive) {}

	Node(Node& a, Node& b, Node& c, Node& d) {
		assert(a.level == b.level == c.level == d.level);
		level = a.level + 1;

		this->a = &a;
		this->b = &b;
		this->c = &c;
		this->d = &d;
	}
	
	void print();
	/*
	 * Returns a vector of offsets of alive cells within the node. (0, 0)
	 * represents the top corner, (2**k-1, 2**k-1) the bottom-right.
	 */
	std::vector<offset_t> positionsAlive();
};
	
int main()
{
	// Level-0 nodes
	Node a {true};
	Node b {false};

	// Level 1
	Node a1 {a, a, a, a};
	Node b1 {b, b, b, b};
	Node c1 {a, b, a, b};
	Node d1 {a, b, b, a};

	// Level 2
	Node a2 {a1, b1, c1, d1};

	a2.print();
}

void Node::print()
{
	int size = 1 << level;
	std::vector<std::vector<bool>> grid(
		size, std::vector<bool>(size));

	for (auto pos : this->positionsAlive())
		grid[pos[1]][pos[0]] = true;

	for (auto row : grid) {
		for (auto cell : row) std::cout << (cell ? "# " : ". ");
		std::cout << std::endl;
	}
}

std::vector<offset_t> Node::positionsAlive()
{
	if (level == 0) {
		if (alive) {
			return {{0, 0}};
		} else {
			return {};
		}
	} else {
		int child_size = 1 << a->level;
		
		std::vector<offset_t> all_positions {};

		for (auto pos: a->positionsAlive())
			all_positions.push_back({pos[0], pos[1]});	
		for (auto pos: b->positionsAlive())
			all_positions.push_back({pos[0]+child_size, pos[1]});
		for (auto pos: c->positionsAlive())
			all_positions.push_back({pos[0], pos[1]+child_size});
		for (auto pos: d->positionsAlive())
			all_positions.push_back({pos[0]+child_size, pos[1]+child_size});
	
		return all_positions;
	}
}
