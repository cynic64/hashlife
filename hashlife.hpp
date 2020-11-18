#ifndef HASHLIFE_H
#define HASHLIFE_H

#include <array>
#include <unordered_map>
#include <vector>

namespace hashlife {
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
		std::vector<offset_t> positions_alive();
	};

	class Cache
	{
		// Given a description of a Node, gives a cache key
		std::string get_key(int level, Node* a, Node* b, Node* c, Node* d, bool alive);

		// Calculates result of (a, b, c, d), 2**(l-2) generations into
		// the future. Returned Node will have the same level as input
		// nodes. Input nodes must all have the same level, at least 2.
		Node* result(Node* a, Node* b, Node* c, Node* d);

		// Special case for results of 4x4s. Input levels must all be 1.
		Node* result_4x4(Node* a, Node* b, Node* c, Node* d);

		std::unordered_map<std::string, Node*> cache;

		int hits = 0;
		int misses = 0;
	public:
		Node* create(int level, Node* a, Node* b, Node* c, Node* d, bool alive);

		Node* load_from_file(std::string path);

		void print_stats();
	};
}

#endif // HASHLIFE_H
