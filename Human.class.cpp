#include "Human.class.hpp"

Human::Human( std::string nm ) : APlayer(nm) {
	return;
}

Human::~Human( void ) {
	return;
}

unsigned short int Human::play(std::map<unsigned short int, char> grid, char value) const {
	SDL_Event event;
	std::cout << this->getName() << " turn" << std::endl;
	while ( !GameManager::goodIput(&grid, value, &event) ) {
		SDL_WaitEvent(&event);
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || event.key.keysym.sym == SDLK_ESCAPE)
	    	throw std::exception();
	}
	return ((event.button.x - 125) / 50) + ((event.button.y - 125) / 50) * 256;
}
