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

	for (auto pos : this->positions_alive())
		grid[pos[1]][pos[0]] = true;

	for (auto row : grid) {
		for (auto cell : row) std::cout << (cell ? "# " : ". ");
		std::cout << std::endl;
	}
}

std::vector<offset_t> Node::positions_alive()
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

		for (auto pos: a->positions_alive())
			all_positions.push_back({pos[0], pos[1]});	
		for (auto pos: b->positions_alive())
			all_positions.push_back({pos[0]+child_size, pos[1]});
		for (auto pos: c->positions_alive())
			all_positions.push_back({pos[0], pos[1]+child_size});
		for (auto pos: d->positions_alive())
			all_positions.push_back({pos[0]+child_size, pos[1]+child_size});
	
		return all_positions;
	}
}

Node* Cache::create(int level, Node* a, Node* b, Node* c, Node* d, bool alive)
{
	std::string desc = get_key(level, a, b, c, d, alive);

	if (cache.find(desc) != cache.end()) {
		hits++;
		return cache[desc];
	}

	misses++;

	Node* n = new Node;
	n->level = level;
	n->alive = alive;
	cache[desc] = n;

	if (level == 0) return n;

	assert(level == a->level + 1);
	assert(level == b->level + 1);
	assert(level == c->level + 1);
	assert(level == d->level + 1);
		
	n->a = a;
	n->b = b;
	n->c = c;
	n->d = d;

	// No need to calculate result when l = 1
	if (level == 1) return n;

	n->result = result(a, b, c, d);

	return n;
}

Node* Cache::result(Node* a, Node* b, Node* c, Node* d)
{
	assert(a->level == b->level
	       && b->level == c->level
	       && c->level == d->level);
	assert(a->level >= 1);

	if (a->level == 1) {
		return result_4x4(a, b, c, d);
	}
	
	// Capital letters represent 6x6 grid, 2**(l-3) generations into
	// the future
	Node* A = a->result;
	Node* B = create(a->level,
			 a->b, b->a, a->d, b->c,
			 false)->result;
	Node* C = b->result;
	Node* D = create(a->level,
			 a->c, a->d, c->a, c->b,
			 false)->result;
	Node* E = create(a->level,
			 a->d, b->c, c->b, d->a,
			 false)->result;
	Node* F = create(a->level,
			 b->c, b->d, d->a, d->b,
			 false)->result;
	Node* G = c->result;
	Node* H = create(a->level,
			 c->b, d->a, c->d, d->c,
			 false)->result;
	Node* I = d->result;

	// 2**(l-2) generations into the future
	Node* x = create(a->level,
			 A, B, D, E, false)->result;
	Node* y = create(a->level,
			 B, C, E, F, false)->result;
	Node* z = create(a->level,
			 D, E, G, H, false)->result;
	Node* w = create(a->level,
			 E, F, H, I, false)->result;

	// Combined result, 2**(l-2) generations into the future
	return create(a->level, x, y, z, w, false);
}

Node* Cache::result_4x4(Node* a, Node* b, Node* c, Node* d)
{
	assert(a->level == 1);
	assert(b->level == 1);
	assert(c->level == 1);
	assert(d->level == 1);

	Node* alive = create(0, NULL, NULL, NULL, NULL, true);
	Node* dead = create(0, NULL, NULL, NULL, NULL, false);

	auto ns = [&](Node* s, Node* a, Node* b, Node* c, Node* d,
		      Node* e, Node* f, Node* g, Node* h)
		{
			int count = a->alive
				+b->alive
				+c->alive
				+d->alive
				+e->alive
				+f->alive
				+g->alive
				+h->alive;
			return (count == 3 || s->alive && count == 2) ?
				alive : dead;
		};

	return create(1,
		      ns(a->d, a->a, a->b, b->a, b->c,
			 d->a, c->b, c->a, a->c),
		      ns(b->c, a->b, b->a, b->b, b->d,
			 d->b, d->a, c->b, a->d),
		      ns(c->b, a->c, a->d, b->c, d->a,
			 d->c, c->d, c->c, c->a),
		      ns(d->a, a->d, b->c, b->d, d->b,
			 d->d, d->c, c->d, c->b),
		      false);
}


std::string Cache::get_key(int level, Node* a, Node* b, Node* c, Node* d, bool alive)
{
	char buf[128];
	if (level == 0)
		sprintf(buf, "%s", alive ? "alive" : "dead");
	else
		sprintf(buf, "L: %d, %p|%p|%p|%p",
			level, (void*)a, (void*)b, (void*)c, (void*)d);

	return std::string {buf};
}

void Cache::print_stats()
{
	std::cout << "size: " << cache.size() << std::endl;
	std::cout << "bucket_count: " << cache.bucket_count() << std::endl;
	std::cout << "load_factor: " << cache.load_factor() << std::endl;
	std::cout << "max_load_factor: " << cache.max_load_factor() << std::endl;
	std::cout << "hits: " << hits << std::endl;
	std::cout << "misses: " << misses << std::endl;
}
