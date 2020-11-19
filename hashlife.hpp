#ifndef HASHLIFE_H
#define HASHLIFE_H

#include <array>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <string>

namespace hashlife {
	typedef std::array<int,2> offset_t;

	class Cache;

	class Node
	{
		// Cached result nodes, 2**i generations into the future: [0] is
		// 1 generation, [2] is 4, etc. up to 2**(l-2)
		std::vector<Node*> cached_results;

		// Special case for results of 4x4s. Internally used for level-2
		// nodes.
		Node* result_4x4(Cache* cache);
	public:
		// a, b, c, d are only used if level > 0. alive is only used if
		// level == 0.
		Node(int level, Node* a, Node* b, Node* c, Node* d, bool alive) {
			this->level = level;
			this->alive = alive;

			if (level == 0) return;

			// a, b, c, d can only be null when level == 0
			assert(a != nullptr && b != nullptr && c != nullptr && d != nullptr);

			assert(level == a->level + 1);
			assert(level == b->level + 1);
			assert(level == c->level + 1);
			assert(level == d->level + 1);

			this->a = a;
			this->b = b;
			this->c = c;
			this->d = d;

			// Anything level-2 or bigger will cache results
			if (level >= 2)
				cached_results.resize(level - 1, nullptr);
		}

		Node() {}

		int level;
		// Children in reading order
		Node *a, *b, *c, *d;
		// Only used for level-0 nodes
		bool alive;

		void print();
	
		// Returns a vector of offsets of alive cells within the node. (0, 0)
		// represents the top corner, (2**k-1, 2**k-1) the bottom-right.
		std::vector<offset_t> positions_alive();

		// Returns result, 2**(l-2) generation into the future.
		Node* result(Cache* cache);

		// Returns result, 2**steps generations into the
		// future.
		Node* result(Cache* cache, int steps);
	};

	class Cache
	{
		// Given a description of a Node, gives a cache key
		std::string get_key(int level, Node* a, Node* b, Node* c, Node* d, bool alive);

		std::unordered_map<std::string, Node*> cache;

		int hits = 0;
		int misses = 0;
	public:
		int node_hits = 0;
		int node_misses = 0;
		
		Node* create(int level, Node* a, Node* b, Node* c, Node* d, bool alive);

		Node* load_from_file(std::string path);

		void print_stats();
	};
}

#endif // HASHLIFE_H
