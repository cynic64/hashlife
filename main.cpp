#include<iostream>
#include<cassert>
#include<vector>
#include<array>
#include<string>
#include<unordered_map>

typedef std::array<int,2> offset_t;

class Node
{
public:
	int level;
	// Children in reading order
	Node *a, *b, *c, *d;
	Node *result;
	// Only used for level-0 nodes
	bool alive;

	void print();
	
	// Returns a vector of offsets of alive cells within the node. (0, 0)
	// represents the top corner, (2**k-1, 2**k-1) the bottom-right.
	std::vector<offset_t> positionsAlive();

	// Calculates the successor and returns it
	Node successor();
};

class Cache
{
	// Given a description of a Node, gives a cache key
	std::string getKey(int level, Node* a, Node* b, Node* c, Node* d, bool alive);

	std::unordered_map<std::string, Node*> cache;
public:
	Node* create(int level, Node* a, Node* b, Node* c, Node* d, bool alive);
};

	
int main()
{
	Cache cache;

	std::vector<Node*> level_0;
	std::vector<Node*> level_1;
	std::vector<Node*> level_2;

	for (int i = 0; i < 64; i++)
		level_0.push_back(cache.create(0,
					       NULL, NULL, NULL, NULL,
					       rand() % 2));

	for (int i = 0; i < 16; i++)
		level_1.push_back(cache.create(1,
					       level_0[4*i], level_0[4*i+1], level_0[4*i+2], level_0[4*i+3],
					       false));

	for (int i = 0; i < 4; i++)
		level_2.push_back(cache.create(2,
					       level_1[4*i], level_1[4*i+1], level_1[4*i+2], level_1[4*i+3],
					       false));

	Node* level_3 = cache.create(3, level_2[0], level_2[1], level_2[2], level_2[3], false);
	level_3->print();
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

Node* Cache::create(int level, Node* a, Node* b, Node* c, Node* d, bool alive)
{
	std::string desc = getKey(level, a, b, c, d, alive);

	if (cache.find(desc) != cache.end()) {
		std::cout << "[Create] "
			  << desc
			  << ": Cached"
			  << std::endl;
		return cache[desc];
	}

	std::cout << "[Create] "
		  << desc
		  << ": Not cached"
		  << std::endl;
	
	Node* n = new Node;
	n->level = level;
	n->alive = alive;

	if (level != 0) {
		assert(level == a->level + 1);
		assert(level == b->level + 1);
		assert(level == c->level + 1);
		assert(level == d->level + 1);
		
		n->a = a;
		n->b = b;
		n->c = c;
		n->d = d;
	};

	cache[desc] = n;

	return n;
}

std::string Cache::getKey(int level, Node* a, Node* b, Node* c, Node* d, bool alive)
{
	char buf[128];
	if (level == 0)
		sprintf(buf, "%s", alive ? "alive" : "dead");
	else
		sprintf(buf, "L: %d, %p|%p|%p|%p",
			level, (void*)a, (void*)b, (void*)c, (void*)d);

	return std::string {buf};
}
