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

bool BotHenry::getSquare(std::map<unsigned short int, char> &grid, unsigned short value) {
    if (grid[value] != 0)
        return false;
    for(char x = -1; x < 2; x++)
        for(char y = -1; y < 2; y++) {
            if (!(x == 0 && y == 0) && grid[value + x + 256 * y] != 0) {
				return true;
			}
        }
    return false;
}

unsigned short int BotHenry::play(std::map<unsigned short, char> grid, char value, char mode, bool noDouble = true) const {
	auto start = std::chrono::high_resolution_clock::now();
	unsigned short res;

    if (grid.empty())
		return (0x909);
    auto handle = std::async(std::launch::deferred, &BotHenry::getScore, std::ref(grid), value, mode, noDouble, this->getPoint(),  GameManager::instance()->otherPoint(this->getPoint()), MAX_DEPTH, std::ref(res), -100000000, 100000000);
    handle.get();
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
	return res;
}

unsigned short int BotHenry::debugPlay(std::map<unsigned short, char> grid, char value, char mode, bool noDouble = true) const {
	auto start = std::chrono::high_resolution_clock::now();
	unsigned short res;

    if (grid.empty())
		return (0x909);
    auto handle = std::async(std::launch::deferred, &BotHenry::getScore, std::ref(grid), value, mode, noDouble, this->getPoint(),  GameManager::instance()->otherPoint(this->getPoint()), MAX_DEPTH, std::ref(res), -100000000, 100000000);
    handle.get();
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
	return res;
}

int BotHenry::getScore(std::map<unsigned short, char> &grid, char value,
char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta)
{
	int currentRating;
	unsigned short oponentPlace;
	if (currentPoint >= 10)
        return 999999;
	if (oponentPoint >= 10)
        return 0;
	if (GameManager::checkBoard(&grid, 0))
		return -999999;
	if (depth == 0)
		return Heuristic(grid, GameManager::instance()->getHistory(), value, MAX_DEPTH % 2).run().getScore();
	for (char x = 0; x < 19; x++)
		for (char y = 0; y < 19; y++) {
			if (BotHenry::getSquare(grid, (y << 8) + x) && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
				grid[(y << 8) + x] = value;
				currentRating = -getScore(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -beta, -alpha);
				grid[(y << 8) + x] = 0;
				if (currentRating > alpha) {
					alpha = currentRating;
					pos = (y << 8) + x;
				}
                if (alpha > beta) {
                    break;
                }
			}
		}
	return alpha;
}
// int BotHenry::getScore(std::map<unsigned short, char> &grid, char value,
// char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta)
// {
// 	int currentRating, tmpScore = -10000000;
// 	char fx = 0, fy = 0, first = 0;
// 	unsigned short oponentPlace;
// 	if (currentPoint >= 10)
//         return 999999;
// 	if (oponentPoint >= 10)
//         return 0;
// 	if (GameManager::checkBoard(&grid, 0))
// 		return -999999;
// 	if (depth == 0)
// 		return Heuristic(grid, GameManager::instance()->getHistory(), value, MAX_DEPTH % 2).run().getScore();
// 	for (char x = 0; x < 19; x++) {
//  		for (char y = 0; y < 19; y++) {
// 			if (first == 0 && BotHenry::getSquare(grid, (y << 8) + x) && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
// 				fx = x;
// 				fy = y;
// 				first = 1;
// 			}
// 		}
// 	}
// 	grid[(fy << 8) + fx] = value;
// 	tmpScore = -getScore(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -beta, -alpha);
// 	pos = (fy << 8) + fx;
// 	grid[(fy << 8) + fx] = 0;
// 	if (tmpScore >= alpha)
// 		alpha = tmpScore;
// 	fy++;
// 	if (fy == 19) {
// 		fx++;
// 		fy = 0;
// 	}
// 	if (tmpScore < beta)
// 		for (char x = fx; x < 19; x++) {
// 			for (char y = fy; y < 19; y++) {
// 				if (BotHenry::getSquare(grid, (y << 8) + x) && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
// 					grid[(y << 8) + x] = value;
// 					currentRating = -getScore(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -(alpha + 1), -alpha);
// 					// if (currentRating > alpha && currentRating < beta)
// 						// currentRating = -getScore(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -beta, -alpha);
// 					grid[(y << 8) + x] = 0;
// 					if (currentRating > tmpScore)
// 					{
// 						tmpScore = currentRating;
// 						pos = (y << 8) + x;
// 						if (currentRating > alpha) {
// 							alpha = currentRating;
// 							pos = (y << 8) + x;
// 							if (alpha > beta) {
// 			                    break;
// 			                }
// 						}
// 					}
// 				}
// 			}
// 			fy = 0;
// 		}
// 	return tmpScore;
// }

bool BotHenry::wantSwap( std::map<unsigned short int, char> grid ) const {
    (void)grid;
    return true;
}

bool BotHenry::wantDoublePlay( std::map<unsigned short int, char> grid ) const {
    (void)grid;
    return false;
}
