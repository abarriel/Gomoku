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
	char currentPoint, tmpPoint, oponentPoint;
	std::map<unsigned short int, char> tmp;

	std::cout << "AI play" << std::endl;
	curScore = 0;
	cur = 0;
	currentPoint = this->getPoint();
	oponentPoint = GameManager::instance()->otherPoint(currentPoint);
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			if (GameManager::goodInput(&grid, value, (y << 8) + x, mode, noDouble))
			{
				tmpPoint = currentPoint;
				tmp = grid;
				tmp[(y << 8) + x] = value;
				currentPoint += GameManager::capture(&grid, (y << 8) + x);
				if (getScore(tmp, value, mode, noDouble, currentPoint, oponentPoint, MAX_DEPTH) > curScore)
					cur = (y << 8) + x;
				currentPoint = tmpPoint;
			}
		}
	}
	return cur;
}

unsigned int BotHenry::getScore(std::map<unsigned short int, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth) const {
	char val;
	std::map<unsigned short int, char> tmp;
	unsigned int res, tmpRes = 0;

	depth--;
	if (currentPoint >= 10)
		return 999999;
	if (oponentPoint >= 10)
		return 0;
	if (GameManager::instance()->checkBoard(&grid, GameManager::instance()->getEnding())) {
		if (depth % 2 )
			return 999999;
		else
			return 0;
	}
	GameManager::instance()->getPlayer(value).setStopUntilPlace().clear();
	val = (depth % 2) ? 3 - value : value;
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			if (GameManager::goodInput(&grid, val, (y << 8) + x, mode, noDouble))
			{
				tmp = grid;
				tmp[(y << 8) + x] = val;
				if (depth == 0) {
					return Heuristic(grid, GameManager::instance()->getHistory(), value).run().getScore();
				} else if (depth % 2) {
				 	if ((tmpRes = getScore(tmp, val, mode, noDouble, currentPoint, oponentPoint + GameManager::capture(&grid, (y << 8) + x), MAX_DEPTH)) < res)
						res = tmpRes;
			 	} else if ((tmpRes = getScore(tmp, val, mode, noDouble, currentPoint + GameManager::capture(&grid, (y << 8) + x), oponentPoint, MAX_DEPTH)) > res) {
					res = tmpRes;
				}
			}
		}
	}
	return res;
}

bool BotHenry::wantSwap( std::map<unsigned short int, char> grid ) const {
	(void)grid;
	return true;
}

bool BotHenry::wantDoublePlay( std::map<unsigned short int, char> grid ) const {
	(void)grid;
	return false;
}
