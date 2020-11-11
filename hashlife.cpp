#include <iostream>
#include <cassert>
#include <vector>
#include <string>

#include "hashlife.hpp"

using namespace hashlife;

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

	if (cache.find(desc) != cache.end())
		return cache[desc];

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


	// Special case: level 2 (4x4)
	if (level == 2) {
		Node* alive = create(0, NULL, NULL, NULL, NULL, true);
		Node* dead = create(0, NULL, NULL, NULL, NULL, false);

		auto ns = [&](Node* s, Node* a, Node* b, Node* c, Node* d,
			      Node* e, Node* f, Node* g, Node* h)
			{
				int n = a->alive
					+b->alive
					+c->alive
					+d->alive
					+e->alive
					+f->alive
					+g->alive
					+h->alive;
				return (n == 3 || s->alive && n == 2) ?
					alive : dead;
			};
		n->result = create(1,
				   ns(n->a->d, n->a->a, n->a->b, n->b->a, n->b->c,
				      n->d->a, n->c->b, n->c->a, n->a->c),
				   ns(n->b->c, n->a->b, n->b->a, n->b->b, n->b->d,
				      n->d->b, n->d->a, n->c->b, n->a->d),
				   ns(n->c->b, n->a->c, n->a->d, n->b->c, n->d->a,
				      n->d->c, n->c->d, n->c->c, n->c->a),
				   ns(n->d->a, n->a->d, n->b->c, n->b->d, n->d->b,
				      n->d->d, n->d->c, n->c->d, n->c->b),
				   false);
	}

	// All higher level results are calculated the same way
	if (level >= 3) {
		// Capital letters represent 6x6 grid, 2**(l-3) generations into
		// the future
		Node* A = n->a->result;
		Node* B = create(n->level - 1,
				 n->a->b, n->b->a, n->a->d, n->b->c,
				 false)->result;
		Node* C = n->b->result;
		Node* D = create(n->level - 1,
				 n->a->c, n->a->d, n->c->a, n->c->b,
				 false)->result;
		Node* E = create(n->level - 1,
				 n->a->d, n->b->c, n->c->b, n->d->a,
				 false)->result;
		Node* F = create(n->level - 1,
				 n->b->c, n->b->d, n->d->a, n->d->b,
				 false)->result;
		Node* G = n->c->result;
		Node* H = create(n->level - 1,
				 n->c->b, n->d->a, n->c->d, n->d->c,
				 false)->result;
		Node* I = n->d->result;

		// 2**(l-2) generations into the future
		Node* x = create(n->level - 1,
				 A, B, D, E, false)->result;
		Node* y = create(n->level - 1,
				 B, C, E, F, false)->result;
		Node* z = create(n->level - 1,
				 D, E, G, H, false)->result;
		Node* w = create(n->level - 1,
				 E, F, H, I, false)->result;

		// Combined result, 2**(l-2) generations into the future
		n->result = create(n->level - 1, x, y, z, w, false);
	}

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