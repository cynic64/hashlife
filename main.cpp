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
	// Create a 4x4 of level-0 nodes
	Node a {true};
	Node b {false};
	Node c {true};
	Node d {false};

	Node q {a, b, c, d};
	a.print();

	/*
	n.print();

	for (auto pos : n.positionsAlive()) {
		std::cout << pos[0] << ", " << pos[1] << std::endl;
	}
	*/
}

void Node::print()
{
	assert(level == 0);

	std::cout << (alive ? "# " : ". ") << std::endl;
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
		assert(0);
	}
}
