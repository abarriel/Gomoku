#include "Human.class.hpp"

Human::Human( std::string nm ) : APlayer(nm) {
	return;
}

Human::Human( void ) : APlayer() {
	return;
}

Human::~Human( void ) {
	return;
}

unsigned short int Human::play(std::map<unsigned short int, char> grid, char value, char mode, bool noDouble = true) const {
	SDL_Event event;
	std::cout << "Human " << this->getName();
    unsigned short nextMove;
	while (1) {
		SDL_WaitEvent(&event);
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT)
	    	throw std::exception();
        nextMove = ((event.button.x - 125) / 50) + ((event.button.y - 125) / 50) * 256;
        if (this->goodInput(&grid, value, &event, mode, noDouble))
                break ;
	}
    std::cout << " play: (" << (nextMove & 0xFF) << "," <<  (nextMove >> 8)  << ")" << std::endl;
	return nextMove;
}

unsigned short int Human::debugPlay(std::map<unsigned short int, char> grid, char value, char mode, bool noDouble = true) const {
	SDL_Event event;
	std::cout << "Human " << this->getName();
    unsigned short nextMove;
	while (1) {
		SDL_WaitEvent(&event);
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT)
	    	throw std::exception();
        nextMove = ((event.button.x - 125) / 50) + ((event.button.y - 125) / 50) * 256;
        if (this->goodInput(&grid, value, &event, mode, noDouble))
                break ;
	}
    std::cout << " play: (" << (nextMove & 0xFF) << "," <<  (nextMove >> 8)  << ")" << std::endl;
	return nextMove;
}

bool Human::goodAwnser(SDL_Event *event) {
	if (event->key.type == SDL_KEYDOWN &&
		(event->key.keysym.scancode == SDL_SCANCODE_Y || event->key.keysym.scancode == SDL_SCANCODE_N) &&
		event->key.windowID == 1
	)
		return true;
	return false;
}

bool Human::wantSwap( std::map<unsigned short int, char> grid ) const {
	SDL_Event event;

	(void)grid;
	std::cout << "Do you want to swap " << this->getName() << "?" << std::endl;
	while ( !this->goodAwnser(&event) ) {
		SDL_WaitEvent(&event);
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || event.key.keysym.sym == SDLK_ESCAPE)
	    	throw std::exception();
	}
	if (event.key.keysym.scancode == SDL_SCANCODE_Y)
		return true;
	return false;
}

bool Human::wantDoublePlay( std::map<unsigned short int, char> grid ) const {
	SDL_Event event;

	(void)grid;
	std::cout << "Do you want to double play " << this->getName() << "?" << std::endl;
	while ( !this->goodAwnser(&event) ) {
		SDL_WaitEvent(&event);
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || event.key.keysym.sym == SDLK_ESCAPE)
	    	throw std::exception();
	}
	if (event.key.keysym.scancode == SDL_SCANCODE_Y)
		return true;
	return false;
}

bool Human::goodInput(std::map<unsigned short, char> *grid, char value, SDL_Event *event, char mode, bool noDouble = true) {
	unsigned short int pos;

	if (event->button.type == SDL_MOUSEBUTTONDOWN &&
		event->button.button == SDL_BUTTON_LEFT &&
		event->button.windowID == 1 &&
		event->button.x >= 125 && event->button.y >= 125 &&
		event->button.x <= 1075 && event->button.y <= 1075)
	{
		pos = ((event->button.x - 125) / 50) + ((event->button.y - 125) / 50) * 256;
		return GameManager::goodInput(*grid, value, pos, mode, noDouble);
	}
	return false;
}
