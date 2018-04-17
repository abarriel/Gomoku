#ifndef SDLMANAGER_CLASS_H
# define SDLMANAGER_CLASS_H

# include <iostream>
# include <SDL2/SDL.h>
# include "Vec.class.hpp"


class SDLManager {

	public:
		SDLManager( void );
		virtual ~SDLManager( void );

		void placeRock(int color, Vec vec);
		void placeRock(int color, unsigned short int point);
		void render();

	private:

		SDL_Window* MainWindow;
		SDL_Renderer* MainRenderer;
		SDL_Texture* WhiteRock;
		SDL_Texture* BlackRock;
  		SDL_Texture* Background;

};


#endif
