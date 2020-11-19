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
	std::cout << "Level: " << n->level << std::endl;

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
	int steps = 0;
	while (!(event.type == SDL_QUIT)) {
		SDL_PollEvent(&event);

		int step_flag = 0;
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				steps++;
				break;
			case SDLK_LEFT:
				steps = std::max(0, steps - 1);
				break;
			case SDLK_SPACE:
				step_flag = 1;
				break;
			default:
				break;
			}
		}

		if (step_flag == 1) {
			// Next generation
			auto a = cache.create(n->level + 1, n, n, n, n, false)->result(&cache, steps);
			n = cache.create(n->level, a->d, a->c, a->b, a->a, false);
			counter++;
		}

		// Clear
		SDL_SetRenderDrawColor(renderer, 80, 80, 80, 80);
		SDL_RenderClear(renderer);

		// Draw
		auto positions = n->positions_alive();
		for (auto pos : positions) {
			int x = pos[0];
			int y = pos[1];
			
			SDL_Rect rect {x*4, y*4, 4, 4};
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &rect);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	std::cout << counter << std::endl << std::endl;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	cache.print_stats();
}
