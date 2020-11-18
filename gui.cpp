#include <cassert>
#include <iostream>

#include <SDL.h>

#include "hashlife.hpp"

const int WIDTH = 1024;
const int HEIGHT = 1024;

int main(int argc, char* argv[])
{
	// Load pattern from file
	assert(argc == 2);
	std::string path { argv[1] };
	
	hashlife::Cache cache;

	hashlife::Node* n = cache.load_from_file(path);

	// Set up SDL
	SDL_Renderer* renderer;
	SDL_Window* window;
	
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

	SDL_Surface* surface = SDL_GetWindowSurface(window);

	// Loop
	SDL_Event event;
	int counter = 0;
	while (!(event.type == SDL_QUIT)) {
		// Next generation
		auto a = cache.create(n->level + 1, n, n, n, n, false)->result(&cache);
		n = cache.create(a->level + 1, a, a, a, a, false)->result(&cache);

		// Clear
		SDL_SetRenderDrawColor(renderer, 80, 80, 80, 80);
		SDL_RenderClear(renderer);

		// Draw
		auto positions = n->positions_alive();
		for (auto pos : positions) {
			int x = pos[0];
			int y = pos[1];
			
			SDL_Rect rect {x*8, y*8, 8, 8};
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &rect);
		}

		SDL_RenderPresent(renderer);

		SDL_PollEvent(&event);
		counter++;
	}

	std::cout << counter << std::endl << std::endl;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	cache.print_stats();
}
