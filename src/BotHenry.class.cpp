#include "BotHenry.class.hpp"

BotHenry::BotHenry( std::string nm ) : APlayer(nm) {
	return;
}

BotHenry::BotHenry( void ) : APlayer(){
	return;
}

BotHenry::~BotHenry( void ) {
	return;
}

unsigned short int BotHenry::play(std::map<unsigned short int, char> grid, char value, char mode, bool noDouble = true) const {
	std::cout << "AI play" << std::endl;
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			if (GameManager::goodInput(&grid, value, y * 256 + x, mode, noDouble))
				return (y * 256 + x);
		}
	}
	return 0;
}

bool BotHenry::wantSwap( std::map<unsigned short int, char> grid ) const {
	(void)grid;
	return true;
}

bool BotHenry::wantDoublePlay( std::map<unsigned short int, char> grid ) const {
	(void)grid;
	return false;
}
