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
    for(char x = -2; x < 3; x++)
        for(char y = -2; y < 3; y++) {
            if (!(x == 0 && y == 0) && grid[value + x + 256 * y] != 0) {
				return true;
			}
        }
    return false;
}

unsigned short int BotHenry::play(std::map<unsigned short, char> grid, char value, char mode, bool noDouble = true) const {
	auto start = std::chrono::high_resolution_clock::now();
	unsigned short res;

    if (grid.empty()) {
		    return (0x909);
    }
    GameManager::instance()->debugGrid(grid);
    auto handle = std::async(std::launch::deferred, &BotHenry::getScore, std::ref(grid), value, mode, noDouble, this->getPoint(),  GameManager::instance()->otherPoint(this->getPoint()), MAX_DEPTH, std::ref(res), -100000000, 100000000);
    handle.get();
    GameManager::instance()->debugGrid(grid);
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

void BotHenry::popScore(std::map<unsigned short, char> &grid, unsigned short pos, unsigned short dir, char &bestMoveScore) {
    pos = pos - dir;
	if (grid[pos] == 0) {
		return ;
    }
    while (grid[pos] == grid[pos - dir]) {
        // std::cout << "-" << (int)grid[pos] << "-" << std::endl;
        pos = pos - dir;
    }
    // std::cout << "(" << ((int)pos >> 8)  << "," << ((int)pos & 0xFF) << ") {"<< (int)grid[pos + dir] <<"}" << std::endl;
    if (grid[pos] != grid[pos + dir])
        bestMoveScore += 2;
    else if (grid[pos] != grid[pos + dir * 2])
        bestMoveScore += 3;
    else if (grid[pos] != grid[pos + dir * 3])
        bestMoveScore += 4;
    else if (grid[pos] != grid[pos + dir * 4])
        bestMoveScore += 5;
    return ;
}
// 3 . 3 3
// 3 3 3

void BotHenry::generateMove(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble) {
    char bestMoveScore;

    for (char x = 0; x < 19; x++)
		for (char y = 0; y < 19; y++) {
			if (BotHenry::getSquare(grid, (y << 8) + x)
                && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
                bestMoveScore = 0;
            // std::cout << "\t(" << (int)y  << "," << (int)x << ") score: " << (int)bestMoveScore << std::endl;               
                //  BotHenry::popScore(grid, (y << 8) + x, 256, bestMoveScore);
                //  BotHenry::popScore(grid, (y << 8) + x, 255, bestMoveScore);
                //  BotHenry::popScore(grid, (y << 8) + x, 257, bestMoveScore);
                //  BotHenry::popScore(grid, (y << 8) + x, 65535, bestMoveScore);
                //  BotHenry::popScore(grid, (y << 8) + x, 1, bestMoveScore);
                //  BotHenry::popScore(grid, (y << 8) + x, 0xFF00, bestMoveScore);
                //  BotHenry::popScore(grid, (y << 8) + x, 0xFF01, bestMoveScore);
                //  BotHenry::popScore(grid, (y << 8) + x, 0xFEFF, bestMoveScore);
                grid[(y << 8) + x] = value;                
                bestMoveScore = Heuristic(grid, GameManager::instance()->getHistory(), value, MAX_DEPTH % 2).run().getScore();
                grid[(y << 8) + x] = 0;
            // std::cout << "\t(" << (int)y  << "," << (int)x << ") score: " << (int)bestMoveScore << std::endl << std::endl;
                //  if (bestMoveScore) {
                    // moves.push_back((bestMoveScore << 16) + ((y << 8) + x));
                    moves.insert(std::upper_bound(moves.begin(), moves.end(), (bestMoveScore << 16) + ((y << 8) + x), [](const int& lhs, const int& rhs ) {
                         return ((lhs >> 16) > (rhs >> 16));
                    }), (bestMoveScore << 16) + ((y << 8) + x));
                //  }
            }
        }
    // exit(1);
}

int BotHenry::getScore(std::map<unsigned short, char> &grid, char value,
char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta)
{
	unsigned short oponentPlace;
    std::vector<int> moves;
	int currentRating;

    // exit(0);
	if (currentPoint >= 10) return 999999;
	if (oponentPoint >= 10) return 0;
	if (GameManager::checkBoard(&grid, 0)) return -999999;
	if (depth == 0) return Heuristic(grid, GameManager::instance()->getHistory(), value, MAX_DEPTH % 2).run().getScore();
    BotHenry::generateMove(grid, moves, value, mode, noDouble);
    for(int mov: (moves)) {
        if (depth == MAX_DEPTH)
            std::cout << "\t\t(" << ((int)((mov & 0xFFFF) >> 8)) << "," << ((int)((mov & 0xFFFF) & 0xFF)) << ") s:" << (int)(mov >> 16) << std::endl;
    }
    for(int mov: (moves)) {
        grid[mov & 0xFFFF] = value;
        currentRating = -getScore(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -beta, -alpha);
        grid[mov & 0xFFFF] = 0;
        if (currentRating > alpha) {
            alpha = currentRating;
            pos = mov & 0xFFFF;
        }
        if (alpha > beta) {
            break;
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
