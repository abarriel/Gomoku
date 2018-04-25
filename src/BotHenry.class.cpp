#include "BotHenry.class.hpp"
# define MAX_DEPTH 2

BotHenry::BotHenry( std::string nm ) : APlayer(nm) {
    // this
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

static std::vector<std::vector<int>> mvs(4);
std::atomic_bool run(true);
std::atomic_bool score_done(false);

unsigned short int BotHenry::play(std::map<unsigned short, char> grid, char value, char mode, bool noDouble = true) const {
	auto realStart = std::chrono::high_resolution_clock::now();
    std::vector<std::future<int>> thread(4);
    char currentPoint, enemyPoint, currentThread, check;
    std::map<unsigned short, char> grid1, grid2, grid3;
    unsigned short ret[] = {0, 0, 0, 0};

    if (grid.empty()) return (0x909);
    for (size_t i = 0; i < mvs.size(); i++) {
        mvs[i].clear();
    }
    check = 0;
    currentThread = 0;
    currentPoint = this->getPoint();
    enemyPoint = GameManager::instance()->otherPoint(this->getPoint());
	BotHenry::generateAttack(grid, mvs[0], value, mode, noDouble, 10);
    std::size_t const generateAttackLength = mvs[0].size();
    std::size_t const size = round((generateAttackLength / 3));
    // for(int mov: mvs[0]) {
    //     std::cout << "\ts:" << (mov >> 16)  << " "<< mov << std::endl;
    // }
    if (generateAttackLength == 2) {
        mvs[1].push_back(mvs[0].at(0));
        mvs[2].push_back(mvs[0].at(1));
    } else if (generateAttackLength == 1) {
        mvs[1] = mvs[0];
    } else if (generateAttackLength) {
        mvs[1].insert(mvs[1].begin(), mvs[0].begin(), mvs[0].begin() + size);
        mvs[0].erase(mvs[0].begin(), mvs[0].begin() + size); // lazyy
        mvs[2].insert(mvs[2].begin(), mvs[0].begin(), mvs[0].begin() + size);
        mvs[0].erase(mvs[0].begin(), mvs[0].begin() + size); // lazyy
        mvs[3] = mvs[0];
    }
    if (generateAttackLength >= 3)
        currentThread = 3;
    else if (generateAttackLength == 1)
        currentThread = 1;
    else if (generateAttackLength == 2)
        currentThread = 2;
    grid1 = grid;
    grid2 = grid;
    grid3 = grid;
    auto getScore = std::async(std::launch::async, &BotHenry::getScore, std::ref(grid), value, mode, noDouble, currentPoint, enemyPoint, MAX_DEPTH, std::ref(ret[0]), -100000000, 100000000);
    auto res1 = std::async(std::launch::async, &BotHenry::getAttack, std::ref(grid1), value, mode, noDouble, currentPoint, enemyPoint, 18, std::ref(ret[1]), -100000000, 100000000, 1);
    auto res2 = std::async(std::launch::async, &BotHenry::getAttack, std::ref(grid2), value, mode, noDouble, currentPoint, enemyPoint, 18, std::ref(ret[2]), -100000000, 100000000, 2);
    auto res3 = std::async(std::launch::async, &BotHenry::getAttack, std::ref(grid3), value, mode, noDouble, currentPoint, enemyPoint, 18, std::ref(ret[3]), -100000000, 100000000, 3);

	auto startAlgo = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms(600);
	auto start = std::chrono::high_resolution_clock::now() + ms;
    while (std::chrono::high_resolution_clock::now() <= start)
        if (score_done)
            check = 1;
    run = false;
    try {
        getScore.get();
    } catch (std::runtime_error &e) {
        std::cout << "getScore is too long" << std::endl;
    }
    try {
        std::cout << res1.get() << std::endl;
        if (res1.get() != 2) ret[1] = 0;
    } catch (std::runtime_error &e) {
        ret[1] = 0;
        // std::cout << "getAttack is too long" << std::endl;
    }
    try {
        if (res2.get() != 2) ret[2] = 0;
    } catch (std::runtime_error &e) {
        ret[2] = 0;
        // std::cout << "getAttack is too long" << std::endl;
    }
    try {
        if (res3.get() != 2) ret[3] = 0;
    } catch (std::runtime_error &e) {
        ret[3] = 0;
        // std::cout << "getAttack is too long" << std::endl;
    }
    std::cout << "\tgetScore (ontime?)" << (int)check << " " << ret[0] << " (" << ((ret[0] & 0xFFFF) >> 8) <<","<< (ret[0] & 0xFF) <<")"<<std::endl;
    if (!check && !ret[1] && !ret[2] && !ret[3]) {
        mvs[0].clear();
        std::cout << "getscore and attacks move too slow. generate mov give us a good shot" << std::endl;
        BotHenry::generateMove(grid, mvs[0], value, mode, noDouble, MAX_DEPTH);
        ret[0] = mvs[0].at(0) & 0xFFFF;
    } else if (ret[1] || ret[2] || ret[3]) {
        std::cout << "attack is ready" << std::endl;
        if (ret[1]) ret[0] = ret[1];
        else if (ret[2]) ret[0] = ret[2];
        else if (ret[3]) ret[0] = ret[3];
    }
    if (ret[1]) std::cout << "\tgetAttack(1): " << ret[1] << " (" << ((ret[1] & 0xFFFF) >> 8) <<","<< (ret[1] & 0xFF) <<")"<<std::endl;
    if (ret[2]) std::cout << "\tgetAttack(2): " << ret[2] <<  " (" << ((ret[2] & 0xFFFF) >> 8) <<","<< (ret[2] & 0xFF) <<")"<<std::endl;
    if (ret[3]) std::cout << "\tgetAttack(3): " << ret[3] << " (" << ((ret[3] & 0xFFFF) >> 8) <<","<< (ret[3] & 0xFF) <<")"<<std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "\tchoose: " << ret[0] << std::endl;
    std::cout << std::chrono::duration<double, std::milli>(end - startAlgo).count() << " ms thread: "<< std::chrono::duration<double, std::milli>(end - realStart).count() << " ms \n";
    run = true;
    score_done = false;
    return ret[0];
    }

unsigned short int BotHenry::debugPlay(std::map<unsigned short, char> grid, char value, char mode, bool noDouble = true) const {
	auto start = std::chrono::high_resolution_clock::now();
	unsigned short res;

    if (grid.empty()) return (0x909);
    auto getScore = std::async(std::launch::async, &BotHenry::getScore, std::ref(grid), value, mode, noDouble, this->getPoint(), GameManager::instance()->otherPoint(this->getPoint()), MAX_DEPTH, std::ref(res), -100000000, 100000000);
    getScore.get();
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
	return res;
    return 0;
}

void BotHenry::popScore(std::map<unsigned short, char> &grid, unsigned short pos, unsigned short dir, char &bestMoveScore) {
    pos = pos - dir;
	if (grid[pos] == 0) return ;
    while (grid[pos] == grid[pos - dir]) {
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

void BotHenry::generateMove(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble, char depth) {
    char bestMoveScore;

    (void)depth;
    for (char x = 0; x < 19; x++)
		for (char y = 0; y < 19; y++) {
			if (BotHenry::getSquare(grid, (y << 8) + x)
                && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
                bestMoveScore = 0;
                 BotHenry::popScore(grid, (y << 8) + x, 256, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 255, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 257, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 65535, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 1, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 0xFF00, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 0xFF01, bestMoveScore);
                 BotHenry::popScore(grid, (y << 8) + x, 0xFEFF, bestMoveScore);
                if (bestMoveScore < 0) {
                    moves.clear();
                    moves.push_back((1000 << 16) + ((y << 8) + x));
					return ;
				}
				if (bestMoveScore != 0) {
                    moves.insert(std::upper_bound(moves.begin(), moves.end(), (bestMoveScore << 16) + ((y << 8) + x), [](const int& lhs, const int& rhs ) {
                         return ((unsigned short)(lhs >> 16) > (unsigned short)(rhs >> 16));
                    }), (bestMoveScore << 16) + ((y << 8) + x));
				}
            }
        }
}

void BotHenry::generateAttack(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble, char depth) {
	char tmp;

    for (char x = 0; x < 19; x++)
		for (char y = 0; y < 19; y++) {
			if (BotHenry::getSquare(grid, (y << 8) + x)
                && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble)) {
				grid[(y << 8) + x] = value;
                tmp = Heuristic(grid, GameManager::instance()->getHistory(), value, 0).run().isAtack((depth + 1) % 2);
				grid[(y << 8) + x] = 0;
                if (tmp ==  2 && depth % 2 == 0) {
                    moves.clear();
                    moves.push_back((2 << 16) + ((y << 8) + x));
					return ;
				}
				if (tmp == 1) {
                    moves.insert(std::upper_bound(moves.begin(), moves.end(), (1 << 16) + ((y << 8) + x), [](const int& lhs, const int& rhs ) {
                         return ((unsigned short)(lhs >> 16) > (unsigned short)(rhs >> 16));
				}), (1 << 16) + ((y << 8) + x));
				}
				if (tmp == 0 && depth % 2) {
					moves.clear();
					return ;
				}
            }
        }
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

    if (!run) throw std::runtime_error("timeout");
	if (oponentPoint >= 10)
        return 10000;
	if (GameManager::checkBoard(&grid, GameManager::instance()->getEnding()))
		return 10000;
	if (depth == 0)
		return Heuristic(grid, GameManager::instance()->getHistory(), value, MAX_DEPTH % 2).run().getScore() + 300 * currentPoint + 300 * oponentPoint;
	BotHenry::generateMove(grid, moves, value, mode, noDouble, depth);
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
    if (depth == MAX_DEPTH)
        score_done = true;
	return tmpScore;
}

// std::vector<int> = std::vector<int>();
int BotHenry::getAttack(std::map<unsigned short, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta, char already) {
	char attackValue;
	bool flag = true;
	unsigned short oponentPlace;
    std::vector<int> moves;
    
    (void)beta;
    if (!run) throw std::runtime_error("timeout");    
	if (currentPoint >= 10)
        return 2;
	if (oponentPoint >= 10)
        return 0;
	if (depth == 0)
		return 0;
    if (!already)
        BotHenry::generateAttack(grid, moves, value, mode, noDouble, depth);
    else {
        if (mvs[already].empty()) return 0;
        moves = mvs[already];
    }
	for(int mov: (moves)) {
		if ((mov >> 16) == 2 && depth % 2 == 0) {		//si l'attaquant a win
			pos = mov & 0xFFFF;
			return 2;
		}
		if ((mov >> 16) == 0 && depth % 2)			//si le defenseur a win
			return 0;
		grid[mov & 0xFFFF] = value;
		attackValue = getAttack(grid, 3 - value, mode, noDouble, 0, 0, depth - 1, oponentPlace, -(alpha + 1), -alpha, 0);
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
