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

void do_join(std::thread& t)
{
    t.join();
}

// unsigned short int BotHenry::play(std::map<unsigned short int, char> grid, char value, char mode, bool noDouble = true) const {
// 	unsigned short int cur;
// 	char currentPoint, tmpPoint, oponentPoint;
// 	std::map<unsigned short int, char> tmp;
//     std::vector<std::thread> t;
//     auto start = std::chrono::high_resolution_clock::now();

//     resThread resT;
//     std::cout << "AI ";
// 	resT.curScore = 0;
// 	cur = 0;
//     resT.res = 0;
// 	currentPoint = this->getPoint();
// 	oponentPoint = GameManager::instance()->otherPoint(currentPoint);
// 	for (char x = 0; x < 19; x++) {
// 		for (char y = 0; y < 19; y++) {
// 			if (BotHenry::getSquare(grid, (y << 8) + x) && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble))
// 			{
// 				tmpPoint = currentPoint;
// 				grid[(y << 8) + x] = value;
// 				currentPoint += GameManager::capture(&grid, (y << 8) + x);
//                 cur = (y << 8) + x;
//                 t.push_back(std::thread(&BotHenry::getFirstScore, std::ref(resT), grid, value, mode, noDouble, currentPoint, oponentPoint, MAX_DEPTH, cur));
// 				currentPoint = tmpPoint;
// 				grid[(y << 8) + x] = 0;
// 			}
// 		}
// 	}
//     std::for_each(t.begin(), t.end(), do_join);
//     auto end = std::chrono::high_resolution_clock::now();
//     std::cout << resT.curScore << std::endl;
//     std::cout << "play: (" << (cur & 0xFF) << "," <<  (cur >> 8)  << ") - in ";
//     std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
    
// 	return resT.res;
// }

unsigned short int BotHenry::play(std::map<unsigned short int, char> grid, char value, char mode, bool noDouble = true) const {
	unsigned int curScore, tmpScore;
	unsigned short int cur;
	char currentPoint, tmpPoint, oponentPoint;
	std::map<unsigned short int, char> tmp;
    
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "AI ";
	curScore = 0;
	cur = 0;
	currentPoint = this->getPoint();
	oponentPoint = GameManager::instance()->otherPoint(currentPoint);
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			if (BotHenry::getSquare(grid, (y << 8) + x) && GameManager::goodInput(grid, value, (y << 8) + x, mode, noDouble))
			{
				tmpPoint = currentPoint;
				tmp = grid;
				grid[(y << 8) + x] = value;
				currentPoint += GameManager::capture(&grid, (y << 8) + x);
				if ((tmpScore = getScore(grid, value, mode, noDouble, currentPoint, oponentPoint, MAX_DEPTH)) > curScore) {
                    cur = (y << 8) + x;
                    curScore = tmpScore;                    
                }
                std::cout << "tmpscore : " << tmpScore << std::endl;
				currentPoint = tmpPoint;
				grid[(y << 8) + x] = 0;
			}
		}
	}
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "play: (" << (cur & 0xFF) << "," <<  (cur >> 8)  << ") - in ";
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
	return cur;
}

bool BotHenry::getSquare(std::map<unsigned short int, char> &grid, unsigned short value) {
    if (grid[value] != 0)
        return false;
    for(char x = -2; x < 3; x++)
        for(char y = -2; y < 3; y++) {
            if (!(x == 0 && y == 0) && grid[value + x + 256 * y] != 0) 
                return true;
        }
    return false;
}

unsigned int BotHenry::getFirstScore(resThread &resT, std::map<unsigned short int, char> grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth,  unsigned short cur)
{
    unsigned int tmpScore;
    if ((tmpScore = getScore(grid, value, mode, noDouble, currentPoint, oponentPoint, depth)) > resT.curScore) {
            resT.res = cur;
            resT.curScore = tmpScore;                    
    }
    return cur;
}

unsigned int BotHenry::getScore(std::map<unsigned short int, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth) {
	char val;
	std::map<unsigned short int, char> tmp;
	unsigned int res, tmpRes;
    bool firstInst = true;
    
	depth--;
    res = 0;
    tmpRes = 0;
    if (depth == 0) {
        res = Heuristic(tmp, GameManager::instance()->getHistory(), value).run().getScore();
        std::cout << "Hres: " << res << " will return 0" << std::endl;
        // return 1;
        return res;
    } 
	if (currentPoint >= 10)
        return 999999;
	if (oponentPoint >= 10)
        return 0;
	val = (depth % 2) ? 3 - value : value;
    // std::cout << "depth: " << (int)depth << " i:" << i++ << std::endl;    
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
            std::cout << "res: " << res << " will return 0" << std::endl;
			if (BotHenry::getSquare(grid, (y << 8) + x) && GameManager::goodInput(grid, val, (y << 8) + x, mode, noDouble))
			{
                tmp = grid;
				tmp[(y << 8) + x] = val;
            std::cout << "YYYY: " << tmpRes << " will return 0" << std::endl;
				
                    if (GameManager::instance()->checkPosVictory(&grid, (y << 8) + x, GameManager::instance()->getEnding())) {
                        std::cout << "ppp: " << tmpRes << " will return 0" << std::endl;                        
                        if (depth % 2)
                            return 0;
                        else
                            return 999999;
                    }
                    if (depth % 2) {
                        if ((tmpRes = getScore(tmp, value, mode, noDouble, currentPoint, oponentPoint + GameManager::capture(&tmp, (y << 8) + x), depth)) < res || firstInst) {
                            if (tmpRes == 0) {
                               return 0;
                            }
                            std::cout << "tmpddd: " << tmpRes << " will return 0" << std::endl;
                            res = tmpRes;
                        }
                    } else if ((tmpRes = getScore(tmp, value, mode, noDouble, currentPoint + GameManager::capture(&tmp, (y << 8) + x), oponentPoint, depth)) > res || firstInst) {
                        std::cout << "tmQQQQ: " << tmpRes << " will return 0" << std::endl;
                        if (tmpRes == 999999)
                            return 999999;
                        res = tmpRes;
                    }
                    firstInst = false;
            }
        }
    }
    std::cout << "res: " << res << " will return 0" << std::endl;
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
