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

    if (grid.empty()) {
		    return (0x909);
    }
    // GameManager::instance()->debugGrid(grid);

	auto handle = std::async(std::launch::deferred, &BotHenry::getScore, std::ref(grid), value, mode, noDouble, this->getPoint(),  GameManager::instance()->otherPoint(this->getPoint()), MAX_DEPTH, std::ref(res), -100000000, 100000000);
    handle.get();

	// if (this->getAttack(grid, value, mode, noDouble, this->getPoint(),  GameManager::instance()->otherPoint(this->getPoint()), 6, res, -100000000, 100000000) != 2) {
	// 	std::cout << "J'ai pas trouver" << '\n';
	// }

    GameManager::instance()->debugGrid(grid);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
	// exit(0);
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
    // std::cout << "(" << ((int)pos >> 8)  << "," << ((int)pos & 0xFF) << ")" << std::endl;
    if (grid[pos] != grid[pos + dir])
        bestMoveScore += 2;
    else if (grid[pos] != grid[pos + dir * 2])
        bestMoveScore += 3;
    else if (grid[pos] != grid[pos + dir * 3])
        bestMoveScore += 4;
    else if (grid[pos] != grid[pos + dir * 4])
        bestMoveScore -= 10000;
    return ;
}
// 3 . 3 3
// 3 3 3

void BotHenry::generateMove(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble, char depth) {
    char bestMoveScore;
	char limitScore = 0;
	int tmp;

    for (char x = 0; x < 19; x++)
		for (char y = 0; y < 19; y++) {
			if (BotHenry::getSquare(grid, (y << 8) + x)
                && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
                bestMoveScore = 0;
            // std::cout << "\t(" << (int)y  << "," << (int)x << ") score: " << (int)bestMoveScore << std::endl;
                 BotHenry::popScore(grid, (y << 8) + x, 256, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 255, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 257, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 65535, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 1, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 0xFF00, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 0xFF01, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 0xFEFF, bestMoveScore);
				// grid[(y << 8) + x] = value;
				// if (depth % 2)
				// 	value = 3 - value;
                // tmp = Heuristic(grid, GameManager::instance()->getHistory(), value, depth % 2).run().getScore();
				// tmp -= 30000;
				// if (tmp < -9000)
					// tmp = -tmp;
				// tmp += 30000;
				// bestMoveScore = (unsigned short)tmp;
                // grid[(y << 8) + x] = 0;
                if (bestMoveScore < 0) {
                    moves.clear();
                    moves.push_back((1000 << 16) + ((y << 8) + x));
					return ;
				}
				if (bestMoveScore != 0) {
                // if (bestMoveScore >= limitScore) {
                    // std::cout << "\t(" << (int)y  << "," << (int)x << ") score: " << (int)bestMoveScore << std::endl << std::endl;
                    // moves.push_back((bestMoveScore << 16) + ((y << 8) + x));
                    moves.insert(std::upper_bound(moves.begin(), moves.end(), (bestMoveScore << 16) + ((y << 8) + x), [](const int& lhs, const int& rhs ) {
                         return ((unsigned short)(lhs >> 16) > (unsigned short)(rhs >> 16));
                    //    return 0;
                    }), (bestMoveScore << 16) + ((y << 8) + x));
					// if (moves.size() == 4)
					// 	limitScore = bestMoveScore;
					// else if (moves.size() > 4) {
					// 	moves.pop_back();
					// 	limitScore = moves.back();
					// }
				}
            }
        }
    // exit(1);
}

void BotHenry::generateAttack(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble, char depth) {
    char bestMoveScore;
	char limitScore = 0;
	char tmp;

    for (char x = 0; x < 19; x++)
		for (char y = 0; y < 19; y++) {
			if (BotHenry::getSquare(grid, (y << 8) + x)
                && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
                bestMoveScore = 0;
            // std::cout << "\t(" << (int)y  << "," << (int)x << ") score: " << (int)bestMoveScore << std::endl;
                 // BotHenry::popScore(grid, (y << 8) + x, 256, bestMoveScore);
                 // BotHenry::popScore(grid, (y << 8) + x, 255, bestMoveScore);
                 // BotHenry::popScore(grid, (y << 8) + x, 257, bestMoveScore);
                 // BotHenry::popScore(grid, (y << 8) + x, 65535, bestMoveScore);
                 // BotHenry::popScore(grid, (y << 8) + x, 1, bestMoveScore);
                 // BotHenry::popScore(grid, (y << 8) + x, 0xFF00, bestMoveScore);
                 // BotHenry::popScore(grid, (y << 8) + x, 0xFF01, bestMoveScore);
                 // BotHenry::popScore(grid, (y << 8) + x, 0xFEFF, bestMoveScore);
				grid[(y << 8) + x] = value;
				// if (depth % 2)
				// 	value = 3 - value;
                tmp = Heuristic(grid, GameManager::instance()->getHistory(), value, 0).run().isAtack((depth + 1) % 2);
				// tmp -= 30000;
				// if (tmp < -9000)
					// tmp = -tmp;
				// tmp += 30000;
				// bestMoveScore = (unsigned short)tmp;
				grid[(y << 8) + x] = 0;

                if (tmp ==  2 && depth % 2 == 0) {
                    moves.clear();
                    moves.push_back((2 << 16) + ((y << 8) + x));
					return ;
				}
				if (tmp == 1) {
                // if (bestMoveScore >= limitScore) {
                    // std::cout << "\t(" << (int)y  << "," << (int)x << ") score: " << (int)bestMoveScore << std::endl << std::endl;
                    // moves.push_back((bestMoveScore << 16) + ((y << 8) + x));
                    moves.insert(std::upper_bound(moves.begin(), moves.end(), (1 << 16) + ((y << 8) + x), [](const int& lhs, const int& rhs ) {
                         return ((unsigned short)(lhs >> 16) > (unsigned short)(rhs >> 16));
                    //    return 0;
				}), (1 << 16) + ((y << 8) + x));
					// if (moves.size() == 4)
					// 	limitScore = bestMoveScore;
					// else if (moves.size() > 4) {
					// 	moves.pop_back();
					// 	limitScore = moves.back();
					// }
				}
				if (tmp == 0 && depth % 2) {
					moves.clear();
					return ;
				}
            }
        }
    // exit(1);
}

// int BotHenry::getScore(std::map<unsigned short, char> &grid, char value,
// char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta)
// {
// 	unsigned short oponentPlace;
//     std::vector<int> moves;
// 	int currentRating;
//
//     // exit(0);
// 	if (currentPoint >= 10) return 999999;
// 	if (oponentPoint >= 10) return 0;
// 	if (GameManager::checkBoard(&grid, 0)) return -999999;
// 	if (depth == 0) return Heuristic(grid, GameManager::instance()->getHistory(), value, MAX_DEPTH % 2).run().getScore();
//     BotHenry::generateMove(grid, moves, value, mode, noDouble);
//     for(int mov: (moves)) {
//         if (depth == MAX_DEPTH)
//             std::cout << "\t\t(" << ((int)((mov & 0xFFFF) >> 8)) << "," << ((int)((mov & 0xFFFF) & 0xFF)) << ") s:" << (int)(mov >> 16) << std::endl;
//     }
//     for(int mov: (moves)) {
//         grid[mov & 0xFFFF] = value;
//         currentRating = -getScore(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -beta, -alpha);
//         grid[mov & 0xFFFF] = 0;
//         if (currentRating > alpha) {
//             alpha = currentRating;
//             pos = mov & 0xFFFF;
//         }
//         if (alpha > beta) {
//             break;
//         }
//     }
// 	return alpha;
// }

char simulatePlay(std::map<unsigned short, char> &grid, unsigned short &pos, char value) {
	grid[pos] = value;
	return (GameManager::capture(&grid, pos, 3));
}

char undoPlay(std::map<unsigned short, char> &grid, unsigned short &pos, char value) {
	GameManager::capture(&grid, pos, 3);
	grid[pos] = 0;
}

int BotHenry::getScore(std::map<unsigned short, char> &grid, char value,
char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta)
{
	int currentRating, tmpScore = -10000000;
	unsigned short oponentPlace;
	std::vector<int> moves;

	if (oponentPoint >= 10)
        return 10000;
	if (GameManager::checkBoard(&grid, GameManager::instance()->getEnding()))
		return 10000;
	if (depth == 0)
		return Heuristic(grid, GameManager::instance()->getHistory(), value, MAX_DEPTH % 2).run().getScore() + 300 * currentPoint + 300 * oponentPoint;
	BotHenry::generateMove(grid, moves, value, mode, noDouble, depth);

	if (depth == MAX_DEPTH) {
		for(int mov: (moves)) {
			std::cout << "\t\t(" << ((int)((mov & 0xFFFF) >> 8)) << "," << ((int)((mov & 0xFFFF) & 0xFF)) << ") s:" << (unsigned short)(mov >> 16) << std::endl;
		}
		std::cout << '\n';
	}
	// std::cout << moves.size() << '\n';
	grid[moves.front() & 0xFFFF] = value;
	tmpScore = -getScore(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -beta, -alpha);
	pos = moves.front()  & 0xFFFF;
	grid[moves.front() & 0xFFFF] = 0;
	moves.erase(moves.begin());
	if (tmpScore >= alpha)
		alpha = tmpScore;
	if (tmpScore < beta)
		for(int mov: (moves)) {
			grid[mov & 0xFFFF] = value;
			currentRating = -getScore(grid, 3 - value, mode, noDouble, oponentPoint, currentPoint, depth - 1, oponentPlace, -(alpha + 1), -alpha);
			// if (currentRating > 40000)
			// 	return 40001;
			// currentRating = -currentRating;
			if (currentRating > alpha && currentRating < beta)
			{
				currentRating = -getScore(grid, 3 - value, mode, noDouble, oponentPoint, oponentPoint, depth - 1, oponentPlace, -beta, -alpha);
				// if (currentRating == -999999)
				// {
				// 	pos = mov & 0xFFFF;
				// 	grid[mov & 0xFFFF] = 0;
				// 	return 999999;
				// }
				// if (currentRating > 40000 && depth == 1)
				// 	return 40001;
				// if (currentRating < 10000)
				// {
				// 	grid[mov & 0xFFFF] = 0;
				// 	return 5000;
				// }
				// currentRating = -currentRating;
			}
			grid[mov & 0xFFFF] = 0;
			if (currentRating > tmpScore)
			{
				tmpScore = currentRating;
				pos = mov & 0xFFFF;
				if (currentRating > alpha) {
					alpha = currentRating;
					pos = mov & 0xFFFF;
					if (alpha > beta) {
	                    break;
	                }
				}
			}
		}
	// if (tmpScore > 40000 || tmpScore < -40000){
	// 	return 999999;
	// }
	return tmpScore;
}

int BotHenry::getAttack(std::map<unsigned short, char> &grid, char value,
char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta)
{
	char attackValue;
	bool flag = true;
	int currentRating, tmpScore = -10000000;
	unsigned short oponentPlace;
	std::vector<int> moves;

	if (currentPoint >= 10)
        return 2;
	if (oponentPoint >= 10)
        return 0;
	if (depth == 0)
		return 0;
	BotHenry::generateAttack(grid, moves, value, mode, noDouble, depth);
	for(int mov: (moves)) {
		if ((mov >> 16) == 2 && depth % 2 == 0) {		//si l'attaquant a win
			pos = mov & 0xFFFF;
			return 2;
		}
		if ((mov >> 16) == 0 && depth % 2)			//si le defenseur a win
			return 0;
		grid[mov & 0xFFFF] = value;
		if (depth == 8)
			GameManager::debugGrid(grid);
		if (depth == 7)
		{
			GameManager::debugGrid(grid);
		}
		attackValue = getAttack(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -(alpha + 1), -alpha);
		if (attackValue == 2 && depth % 2 == 0) {
			pos = mov & 0xFFFF;            //si l'attaque marche
			grid[mov & 0xFFFF] = 0;
			return 2;
		} else if (attackValue != 2) {
			if (depth == 7)
				std::cout << (mov & 0xFFFF) << '\n';
			flag = false;
		} else if (attackValue == 0 && depth % 2)
			return 0;
		grid[mov & 0xFFFF] = 0;
	}
	if (flag == true && depth % 2)
		return 2;
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
