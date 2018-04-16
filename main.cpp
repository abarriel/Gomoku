#include <SDL2/SDL.h>
#include <iostream>
#include "SDLManager.class.hpp"
#include "GameManager.class.hpp"
#include "Vec.class.hpp"

int main( void )
{
	SDLManager *SDLMan;
	SDL_Event event;
	bool quit(false);
	GameManager *GameMan;

	SDLMan = new SDLManager();
	GameMan = new GameManager();
	SDLMan->render();
	while(!quit) {
		try {
			if (GameMan->playTurn(SDLMan)) {
				while (!quit) {
					SDL_WaitEvent(&event);
					if (event.window.event == SDL_WINDOWEVENT_CLOSE || event.key.keysym.sym == SDLK_ESCAPE)
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
