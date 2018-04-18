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
	unsigned int curScore;
	unsigned short int cur;
	char currentPoint;
	char oponentPoint;
	std::map<unsigned short int, char> tmp;

	std::cout << "AI play" << std::endl;
	curScore = 0;
	cur = 0;
	currentPoint = this->getPoint();
	oponentPoint = 
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			if (GameManager::goodInput(&grid, value, y << 8 + x, mode, noDouble))
			{
				tmp = grid;
				tmp[y << 8 + x] = value;
				GameManager::capture(&grid, y << 8 + x);
				if (getScore(tmp, value, mode, noDouble, MAX_DEPTH) > curScore)
					cur = y << 8 + x;
			}
		}
	}
	return cur;
}

bool BotHenry::wantSwap( std::map<unsigned short int, char> grid ) const {
	(void)grid;
	return true;
}

bool BotHenry::wantDoublePlay( std::map<unsigned short int, char> grid ) const {
	(void)grid;
	return false;
}
