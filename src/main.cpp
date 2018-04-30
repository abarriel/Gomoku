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
    try {
        if (argc == 2 && strncmp("battle", argv[1], 6) == 0)
            GameMan = GameManager::instance(0,0);
        else if (argc == 2 && strncmp("default", argv[1], 7) == 0)
            GameMan = GameManager::instance(2,2);
        else if (argc == 2 && strncmp("helpme", argv[1], 7) == 0)
            GameMan = GameManager::instance(3,3);
        else
            GameMan = GameManager::instance(1,1);
   	} catch (std::exception &e) {
           delete GameMan;
           return 0;
	}
	SDLMan = new SDLManager();
    GameMan->setSDL(SDLMan);
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
