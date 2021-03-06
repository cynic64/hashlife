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

	Node* n = new Node {level, a, b, c, d, alive};
	cache[desc] = n;

	return n;
}

Node* Node::result(Cache *cache)
{
	return result(cache, level - 2);
}
	
Node* Node::result(Cache *cache, int steps)
{
	// Can't take the result of anything smaller than a 4x4
	assert(level >= 2);
	
	if (steps > level - 2) steps = level - 2;
	assert(steps >= 0);

	if (cached_results[steps] != nullptr) {
		cache->node_hits++;
		return cached_results[steps];
	}

	cache->node_misses++;

	if (level == 2) {
		cached_results[steps] = result_4x4(cache);
		return cached_results[steps];
	}
	
	// Capital letters represent 6x6 grid, 2**(l-3) generations into
	// the future
	Node* A = a->result(cache, steps);
	Node* B = cache->create(level - 1,
				a->b, b->a, a->d, b->c,
				false)->result(cache, steps);
	Node* C = b->result(cache, steps);
	Node* D = cache->create(level - 1,
				a->c, a->d, c->a, c->b,
				false)->result(cache, steps);
	Node* E = cache->create(level - 1,
				a->d, b->c, c->b, d->a,
				false)->result(cache, steps);
	Node* F = cache->create(level - 1,
				b->c, b->d, d->a, d->b,
				false)->result(cache, steps);
	Node* G = c->result(cache, steps);
	Node* H = cache->create(level - 1,
				c->b, d->a, c->d, d->c,
				false)->result(cache, steps);
	Node* I = d->result(cache, steps);

	Node *x, *y, *z, *w;
	if (steps < level - 2) {
		// Our level is too high for the number of steps we're trying to
		// compute, so don't do any work - just merge
		x = cache->create(level - 2,
				  A->d, B->c, D->b, E->a, false);
		y = cache->create(level - 2,
				  B->d, C->c, E->b, F->a, false);
		z = cache->create(level - 2,
				  D->d, E->c, G->b, H->a, false);
		w = cache->create(level - 2,
				  E->d, F->c, H->b, I->a, false);
	} else {
		x = cache->create(level - 1,
				  A, B, D, E, false)->result(cache, steps);
		y = cache->create(level - 1,
				  B, C, E, F, false)->result(cache, steps);
		z = cache->create(level - 1,
				  D, E, G, H, false)->result(cache, steps);
		w = cache->create(level - 1,
				  E, F, H, I, false)->result(cache, steps);
	}

	// Combined result, 2**(l-2) generations into the future
	cached_results[steps] = cache->create(level - 1, x, y, z, w, false);
	return cached_results[steps];
}

Node* Node::result_4x4(Cache *cache)
{
	assert(a->level == 1);
	assert(b->level == 1);
	assert(c->level == 1);
	assert(d->level == 1);

	Node* alive = cache->create(0, NULL, NULL, NULL, NULL, true);
	Node* dead = cache->create(0, NULL, NULL, NULL, NULL, false);

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

	return cache->create(1,
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
	std::cout << "node_hits: " << node_hits << std::endl;
	std::cout << "node_misses: " << node_misses << std::endl;
}
