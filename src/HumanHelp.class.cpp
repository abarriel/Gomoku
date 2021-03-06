#include "HumanHelp.class.hpp"
// #include "B.class.hpp"

HumanHelp::HumanHelp( std::string nm ) : APlayer(nm) {
	return;
}

HumanHelp::HumanHelp( void ) : APlayer() {
	return;
}

HumanHelp::~HumanHelp( void ) {
	return;
}

unsigned short int HumanHelp::play(std::map<unsigned short int, char> grid, char value, char mode, bool noDouble = true) const {
	SDL_Event event;
    unsigned short nextMove;
	unsigned short ret;

    if (grid.empty()) return (0x909);
    BotHenry::getScore(grid, value, mode, noDouble, this->getPoint(), GameManager::instance()->otherPoint(this->getPoint()), 2, ret, -100000000, 100000000);
    nextMove = grid[ret];
    grid[ret] = 4;
    GameManager::instance()->printGrid(grid);
    grid[ret] = nextMove;
    nextMove = 0;
	GameManager::instance()->getSDl()->render();
	while (1) {
		SDL_WaitEvent(&event);
		if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) {
	    	throw std::exception();
        }
        nextMove = ((event.button.x - 46) / 60) + ((event.button.y - 46) / 60) * 256;
        if (this->goodInput(&grid, value, &event, mode, noDouble))
                break ;
	}
    std::cout << "AI would have play: (" << (ret & 0xFF) << "," <<  (ret >> 8)  << "). Human play (" << (nextMove & 0xFF) << "," <<  (nextMove >> 8)  << ")" << std::endl;
	return nextMove;
}

unsigned short int HumanHelp::debugPlay(std::map<unsigned short int, char> grid, char value, char mode, bool noDouble = true) const {
	SDL_Event event;
	std::cout << "HumanHelp " << this->getName();
    unsigned short nextMove;
	while (1) {
		SDL_WaitEvent(&event);
		if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) {
	    	throw std::exception();
        }
        nextMove = ((event.button.x - 46) / 60) + ((event.button.y - 46) / 60) * 256;
        if (this->goodInput(&grid, value, &event, mode, noDouble))
                break ;
	}
    std::cout << " play: (" << (nextMove & 0xFF) << "," <<  (nextMove >> 8)  << ")" << std::endl;
	return nextMove;
}

bool HumanHelp::goodAwnser(SDL_Event *event) {
	if (event->key.type == SDL_KEYDOWN &&
		(event->key.keysym.scancode == SDL_SCANCODE_Y || event->key.keysym.scancode == SDL_SCANCODE_N) &&
		event->key.windowID == 1
	)
		return true;
	return false;
}

bool HumanHelp::wantSwap(std::map<unsigned short int, char> grid) const {
	SDL_Event event;

	(void)grid;
	std::cout << "Do you want to swap " << this->getName() << "?" << std::endl;
	while ( !this->goodAwnser(&event) ) {
		SDL_WaitEvent(&event);
		// if (event.key.keysym.sym == SDLK_ESCAPE)
	    // 	throw std::exception();
	}
	if (event.key.keysym.scancode == SDL_SCANCODE_Y)
		return true;
	return false;
}

bool HumanHelp::wantDoublePlay(std::map<unsigned short int, char> grid) const {
	SDL_Event event;

	(void)grid;
	std::cout << "Do you want to double play " << this->getName() << "?" << std::endl;
	while ( !this->goodAwnser(&event) ) {
		SDL_WaitEvent(&event);
		// if (event.key.keysym.sym == SDLK_ESCAPE)
	    // 	throw std::exception();
	}
	if (event.key.keysym.scancode == SDL_SCANCODE_Y)
		return true;
	return false;
}

bool HumanHelp::goodInput(std::map<unsigned short, char> *grid, char value, SDL_Event *event, char mode, bool noDouble = true) {
	unsigned short int pos;

	if (event->button.type == SDL_MOUSEBUTTONDOWN &&
		event->button.button == SDL_BUTTON_LEFT &&
		event->button.windowID == 1 &&
		event->button.x >= 46 && event->button.y >= 46 &&
		event->button.x <= 1158 && event->button.y <= 1158)
	{
		pos = ((event->button.x - 46) / 60) + ((event->button.y - 46) / 60) * 256;
		return GameManager::goodInput(*grid, value, pos, mode, noDouble);
	}
	return false;
}
