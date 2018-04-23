#include <SDL2/SDL.h>
#include <iostream>
#include "SDLManager.class.hpp"
#include "GameManager.class.hpp"
#include "Vec.class.hpp"

int main( int argc, char const **argv )
{
	SDLManager *SDLMan;
	SDL_Event event;
	bool quit(false);
	GameManager *GameMan;
    // GameManager::instance();
	if (argc == 2 && strncmp("default", argv[1], 7) == 0)
		GameMan = GameManager::instance(false);
	else
		GameMan = GameManager::instance(true);
	SDLMan = new SDLManager();
    GameMan->printGrid(SDLMan);
	SDLMan->render();
	auto start = std::chrono::high_resolution_clock::now();
	while(!quit) {
		try {
			if (GameMan->playTurn(SDLMan)) {
				auto end = std::chrono::high_resolution_clock::now();
				std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms for the entiere game\n";
				while (!quit) {
					SDL_WaitEvent(&event);
					if (event.key.keysym.sym == SDLK_h)
						GameMan->replay(SDLMan);
					if (event.key.keysym.sym == SDLK_ESCAPE)
		    			quit = true;
				}
			}
		} catch (std::exception &e) {
			quit = true;
		}
    }
	delete SDLMan;
	delete GameMan;
    return 0;
}
