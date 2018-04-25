#include "GameManager.class.hpp"

GameManager *GameManager::p_instance = 0;
GameManager::GameManager( bool asking ) :
	turn(1),
	canCapture(true),
	endingCapture(true),
	noDoubleThrees(true),
	gameMode(0),
	swapOportunity(false)
{
	std::string res;
	this->history = new std::stack<unsigned short int>;
    if (!asking) {
		this->PlayerOne = new Human("Black");
        this->PlayerTwo = new Human("White");
        // this->PlayerTwo = new BotHenry("White");
        return;
    }
    else {
        std::cout << "mode (STANDARD/pro/long-pro/swap/swap2): ";
        std::getline(std::cin, res);
        if (res == "pro")
            this->gameMode = 2;
        if (res == "long-pro")
            this->gameMode = 3;
        if (res == "swap")
            this->gameMode = 4;
        if (res == "swap2")
            this->gameMode = 5;
        std::cout << "Capture mode (Y/n): ";
        std::getline(std::cin, res);
        if (res.at(0) == 'n') {
            this->canCapture = false;
            this->endingCapture = false;
        } else {
            std::cout << "Ending capture mode (Y/n): ";
            std::getline(std::cin, res);
            if (res.at(0) == 'n')
                this->endingCapture = false;
        }
        std::cout << "No double-threes (Y/n): ";
        std::getline(std::cin, res);
        if (res.at(0) == 'n')
            this->noDoubleThrees = false;
        std::cout << "Human player 1 (Y/n): ";
        std::getline(std::cin, res);
        if (res.at(0) == 'n' && !this->gameMode)
            this->PlayerOne = new BotHenry();
        else if(res.at(0) == 'n')
            {
                std::cout << "ai can only be play in standard mode" << std::endl;
                throw std::exception();
            }
        else
            this->PlayerOne = new Human();
        std::cout << "Human player 2 (Y/n): ";
        std::getline(std::cin, res);
        if (res.at(0) == 'n'  && !this->gameMode)
            this->PlayerTwo = new BotHenry();
        else if(res.at(0) == 'n')
            {
                std::cout << "ai can only be play in standard mode" << std::endl;
                throw std::exception();
            }
        else
            this->PlayerTwo = new Human();
    }
    std::cout << "debug:";
    std::getline(std::cin, res);
    if (res.at(0) == 'y')
        this->fillMap();
    std::cout << std::endl;
	return;
}

void GameManager::fillMap(void) {
    std::string res;
    char p;

    std::getline(std::cin, res);
    p = res.at(0);
    this->turn = (((p - 48) - 1) + 3);
    for (char x = 0; std::getline(std::cin, res); x++) {
        if(x > 19) return ;
        std::stringstream ss(res);
        for (char y = 0; ss >> p; y++) {
            if (p == '1' || p == '2') {
                this->grid[(x << 8) + y] = p - 48;
            }
            // std::cout << "y" << (int)y << "d" << std::endl;
        }
    }
}

std::stack<unsigned short int> &GameManager::getHistory() const {
        return *this->history;
}

GameManager::~GameManager( void ) {
	delete this->PlayerOne;
	delete this->PlayerTwo;
	return;
}

void GameManager::printGrid(SDLManager *SDLMan) {
    if (this->grid.empty()) return ;
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			SDLMan->placeRock(this->grid[y * 256 + x], Vec(x, y));
		}
	}
}

void GameManager::printGrid(SDLManager *SDLMan, std::map<unsigned short int, char> gridE) {
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			SDLMan->placeRock(gridE[y * 256 + x], Vec(x, y));
		}
	}
}

void GameManager::twoTurn(APlayer *player, SDLManager *SDLMan) {
	unsigned short int place;

	this->history->push(place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees ));
	this->grid[place] = ((this->turn + 1) % 2) + 1;
	this->printGrid(SDLMan);
	SDLMan->render();
	this->history->push(place = player->play( this->grid, 3 - (((this->turn + 1) % 2) + 1), 0, this->noDoubleThrees ));
	this->grid[place] = 3 - (((this->turn + 1) % 2) + 1);
	this->printGrid(SDLMan);
	SDLMan->render();
}

unsigned short int GameManager::threeTurn(APlayer *player, SDLManager *SDLMan) {
	unsigned short int place;

	this->history->push(place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees ));
	this->grid[place] = ((this->turn + 1) % 2) + 1;
	this->printGrid(SDLMan);
	SDLMan->render();
	this->history->push(place = player->play( this->grid, 3 - (((this->turn + 1) % 2) + 1), 0, this->noDoubleThrees ));
	this->grid[place] = 3 - (((this->turn + 1) % 2) + 1);
	this->printGrid(SDLMan);
	SDLMan->render();
	return player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees );
}

bool GameManager::cantContinue() {
	for (unsigned short int x = 0; x < 19; x++) {
		for (unsigned short int y = 0; y < 19; y++) {
			if (goodInput(this->grid, ((this->turn + 1) % 2) + 1, y * 256 + x, 0, this->noDoubleThrees))
				return (false);
		}
	}
	std::cout << "It's draw !" << std::endl;
	return (true);
}

char GameManager::playTurn(SDLManager *SDLMan) {
	unsigned short int place;
	APlayer *player;
	bool skipPlace = false;

    if (this->turn == 1)
        if(this->grid.empty())
            std::cout << "EMPTY" << std::endl;
	if (this->turn % 2 == 1)
		player = PlayerOne;
	else
		player = PlayerTwo;
	if (this->swapOportunity) {
		this->swapOportunity = false;
		if (player->wantSwap(this->grid)) {
			std::cout << player->getName() << " swap!" << std::endl;
			player = this->PlayerOne;
			this->PlayerOne = this->PlayerTwo;
			this->PlayerTwo = player;
			if (this->turn % 2 == 1)
				player = PlayerOne;
			else
				player = PlayerTwo;
			this->history->push(place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees ));
		} else if (this->gameMode & 1 && player->wantDoublePlay(this->grid)) {
			std::cout << player->getName() << " double play!" << std::endl;
			this->swapOportunity = true;
			this->gameMode --;
			skipPlace = true;
			this->twoTurn(player, SDLMan);
			player = this->PlayerOne;
			this->PlayerOne = this->PlayerTwo;
			this->PlayerTwo = player;
			if (this->turn % 2 == 1)
				player = PlayerOne;
			else
				player = PlayerTwo;
			this->turn -= 1;
		} else
			this->history->push(place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees ));
	}
	else if (this->turn == 1 && this->gameMode & 4) {
		this->history->push(place = this->threeTurn(player, SDLMan));
		this->swapOportunity = true;
	}
	else if (this->turn == 1 && this->gameMode & 2)
		this->history->push(place = 2313);
	else if (this->turn == 3 && this->gameMode & 2)
		this->history->push(place = player->play( this->grid, ((this->turn + 1) % 2) + 1, this->gameMode, this->noDoubleThrees ));
	else {
		this->history->push(place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees ));
    }
	if (!skipPlace)
		this->grid[place] = ((this->turn + 1) % 2) + 1;
	printf("%d\n", this->canCapture);
	if (this->canCapture)
		player->increasePoint(capture(&this->grid, place));
	this->printGrid(SDLMan);
	SDLMan->render();
    std::cout << Heuristic(grid, GameManager::instance()->getHistory(), 2, 0).run() << std::endl ;

	if (player->haveWin() || checkBoard(&this->grid, this->endingCapture)) {
		player->victory();
		return (1);
	}
	this->turn++;
	if (this->cantContinue())
		return 1;
	return 0;
}

void GameManager::debugGrid(std::map<unsigned short int, char> &grid) {
    if (grid.empty()) return ;
    std::cout << "=================== DEBUG GRID ================" << std::endl;
    std::cout << "   : 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
    for (char x = 0; x < 19; x++) {
        std::cout << std::setw(2) << (int)x << " : ";
		for (char y = 0; y < 19; y++) {
            if ((int)grid[x * 256 + y] == 1)
                std::cout << "\033[1;31m" << (int)grid[x * 256 + y] << "\033[0m" << " ";
            else if ((int)grid[x * 256 + y] == 2)
                std::cout << "\033[1;32m" << (int)grid[x * 256 + y] << "\033[0m" << " ";
            else if ((int)grid[x * 256 + y] == 0)
                std::cout << ". ";
             else
                std::cout << "! ";
		}
        std::cout << std::endl;
    }
    std::cout << "=================== END GRID ================" << std::endl;
}

void GameManager::debugGrid(void) {
    if (grid.empty()) return ;
    std::cout << "=================== DEBUG GRID ================" << std::endl;
    std::cout << "   : 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
    for (char x = 0; x < 19; x++) {
        std::cout << std::setw(2) << (int)x << " : ";
		for (char y = 0; y < 19; y++) {
            if ((int)this->grid[x * 256 + y] == 1)
                std::cout << "\033[1;31m" << (int)this->grid[x * 256 + y] << "\033[0m" << " ";
            else if ((int)this->grid[x * 256 + y] == 2)
                std::cout << "\033[1;32m" << (int)this->grid[x * 256 + y] << "\033[0m" << " ";
            else
                std::cout << ". ";
		}
        std::cout << std::endl;
    }
    std::cout << "=================== END GRID ================" << std::endl;
}

char GameManager::otherPoint(char o) {
	if (this->PlayerOne->getPoint() == o)
		return this->PlayerTwo->getPoint();
	return this->PlayerOne->getPoint();
}

bool GameManager::getEnding( void ) {
	return this->endingCapture;
}

bool GameManager::getCapture( void ) {
	return this->canCapture;
}

APlayer& GameManager::getPlayer(char which) {
	if (which == 1)
		return *this->PlayerOne;
	return *this->PlayerTwo;
}

char GameManager::capture(std::map<unsigned short int, char> *grid, unsigned short int place, char replace) {
	char res = 0;

    res += checkCapture(grid, place, 256, replace);
	res += checkCapture(grid, place, 255, replace);
	res += checkCapture(grid, place, 257, replace);
	res += checkCapture(grid, place, 65535, replace);
	res += checkCapture(grid, place, 1, replace);
	res += checkCapture(grid, place, 0xFF00, replace);
	res += checkCapture(grid, place, 0xFF01, replace);
	res += checkCapture(grid, place, 0xFEFF, replace);
	return res;
}

char GameManager::checkCapture(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, char replace) {
	unsigned short tmp = place + dir * 3;

	if ((tmp & 0xFF) < 0 || (tmp & 0xFF) > 18 || (tmp >> 8) < 0 || (tmp >> 8) > 18 ||
		(*grid)[tmp] != (*grid)[place] ||
		3 - (*grid)[place] != (*grid)[place + dir] ||
		3 - (*grid)[place] != (*grid)[place + dir * 2]
	)
		return 0;
	(*grid)[place + dir] = replace;
	(*grid)[place + dir * 2] = replace;
	return 2;
}

void GameManager::undoCapture(std::map<unsigned short int, char> *grid, unsigned short int place, char replace) {
    removeCapture(grid, place, 256, replace);
	removeCapture(grid, place, 255, replace);
	removeCapture(grid, place, 257, replace);
	removeCapture(grid, place, 65535, replace);
	removeCapture(grid, place, 1, replace);
	removeCapture(grid, place, 0xFF00, replace);
	removeCapture(grid, place, 0xFF01, replace);
	removeCapture(grid, place, 0xFEFF, replace);
}

void GameManager::removeCapture(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, char replace) {
	unsigned short tmp = place + dir * 3;

	if (!((tmp & 0xFF) < 0 || (tmp & 0xFF) > 18 || (tmp >> 8) < 0 || (tmp >> 8) > 18 ||
		(*grid)[tmp] != (*grid)[place] ||
		3 != (*grid)[place + dir] || 3 != (*grid)[place + dir * 2])
	) {
		(*grid)[place + dir] = 3 - (*grid)[place];
		(*grid)[place + dir * 2] = 3 - (*grid)[place];
	}
}

unsigned short int GameManager::SeqFromTo(std::map<unsigned short, char>&grid, Vec from, Vec to, Vec dir, char value, Vec skip) {
    return (GameManager::SeqFromTo(grid, VecToUsi(from), VecToUsi(to), VecToUsi(dir), value, VecToUsi(skip)));
}

unsigned short int GameManager::SeqFromTo(std::map<unsigned short, char>&grid, unsigned short from, unsigned short to, unsigned short dir, char value, unsigned short skip) {
	unsigned short int res;

	res = 0;
	while (from != to + dir) {
		res = res << 2;
		if ((from & 0xFF) >= 0 && (from & 0xFF) <= 18 && (from >> 8)>= 0 && (from >> 8)<= 18) {
			if (grid[from] && grid[from] != 3) {
				if (grid[from] == value)
					res += 1;
				else
					res += 2;
			}
		}
		else
			res += 3;
		from = from + dir;
		if (from == skip)
			from = from + dir;
	}
	return res;
}

char GameManager::SeqIsThree(unsigned short int seq) {
	if ((seq | 61455) == 61775 && ((seq | 53247) == 53247 || (seq | 65523) == 65523))
		return 1;
	if ((seq | 49215) == 50495 && ((seq | 16383) == 16383 || (seq | 65487) == 65487))
		return 1;
	if ((seq | 64515) == 64595 && ((seq | 62463) == 62463 || (seq | 65532) == 65532))
		return 1;
	if ((seq | 64512) == 64532)
		return 1;
	if ((seq | 64512) == 64580)
		return 1;
	if ((seq | 49167) == 50255)
		return 1;
	if ((seq | 61443) == 61715)
		return 1;
	if ((seq | 63) == 4415)
		return 1;
	if ((seq | 63) == 5183)
		return 1;
	return 0;
}

bool GameManager::doubleThrees(std::map<unsigned short, char>&grid, unsigned short int place, char value) {
	unsigned short int seq;
    unsigned short dir = 256;
	char res = 0;

	seq = SeqFromTo(grid, place - dir * 4, place + dir * 4, dir, value, place);
	res += SeqIsThree(seq);
	dir = 1;
	seq = SeqFromTo(grid, place - dir * 4, place + dir * 4, dir, value, place);
	res += SeqIsThree(seq);
	dir = 257;
	seq = SeqFromTo(grid, place - dir * 4, place + dir * 4, dir, value, place);
	res += SeqIsThree(seq);
	dir = 255;
	seq = SeqFromTo(grid, place - dir * 4, place + dir * 4, dir, value, place);
	res += SeqIsThree(seq);
	if (res > 1)
		return 1;
	return 0;
}

bool GameManager::goodInput(std::map<unsigned short, char> &grid, char value, unsigned short int pos, char mode, bool noDouble = true) {
	if (mode & 2) {
		Vec tmp = UsiToVec(pos);
		if (tmp.x >= (7 - (mode & 1)) && tmp.y >= (7 - (mode & 1)) &&
			tmp.x <= (11 + (mode & 1)) && tmp.y <= (11 + (mode & 1)))
			return false;
	}
	if ((grid)[pos] != 0 || (noDouble && doubleThrees(grid, pos, value)))
		return false;
	return true;
}

char GameManager::checkBoard(std::map<unsigned short int, char> *grid, bool ending = true) {
	char res = 0;

	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
            if ((res = checkFive(grid, (y << 8) + x, 256, ending)))
                return res;
            if ((res = checkFive(grid, (y << 8) + x, 1, ending)))
                return res;
            if ((res = checkFive(grid, (y << 8) + x, 257, ending)))
                return res;
            if ((res = checkFive(grid, (y << 8) + x, 255, ending)))
                return res;
		}
	}
	return res;
}

char GameManager::checkPosVictory(std::map<unsigned short int, char> *grid, unsigned short pos, bool ending = true) {
	char res = 0;

    if ((res = checkFiveRe(grid, pos, 256, ending)))
        return res;
    if ((res = checkFiveRe(grid, pos, 1, ending)))
        return res;
    if ((res = checkFiveRe(grid, pos, 257, ending)))
        return res;
    if ((res = checkFiveRe(grid, pos, 255, ending)))
        return res;
    if ((res = checkFiveRe(grid, pos, 0xFF00, ending)))
        return res;
    if ((res = checkFiveRe(grid, pos, 0xFF01, ending)))
        return res;
    if ((res = checkFiveRe(grid, pos, 0xFEFF, ending)))
        return res;
    if ((res = checkFiveRe(grid, pos, 0xFFFF, ending)))
        return res;
    return 0;
}

char GameManager::checkFiveRe(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, bool ending = true) {
	unsigned short tmp;

	if ((*grid)[place] == 0 || (*grid)[place] == 3)
		return 0;
    while ((*grid)[place] == (*grid)[place - dir])
        place = place - dir;
    tmp = place + dir * 4;
	if ((tmp & 0xFF) < 0 || (tmp & 0xFF) > 18 ||(tmp >> 8) < 0 || (tmp >> 8) > 18 ||
		(*grid)[place] != (*grid)[tmp] ||
		(*grid)[place] != (*grid)[place + dir] ||
		(*grid)[place] != (*grid)[place + dir * 2] ||
		(*grid)[place] != (*grid)[place + dir * 3] || ((
		canBeEat(grid, place) || canBeEat(grid, place + dir) || canBeEat(grid, tmp) ||
		canBeEat(grid, place + dir * 2) || canBeEat(grid, place + dir * 3)) && ending)
	)
		return 0;
	return (*grid)[place];
}

char GameManager::checkFive(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, bool ending = true) {
	unsigned short tmp = place + dir * 4;

	if ((*grid)[place] == 0 || (*grid)[place] == 3)
		return 0;
	if ((tmp & 0xFF) < 0 || (tmp & 0xFF) > 18 ||(tmp >> 8) < 0 || (tmp >> 8) > 18 ||
		(*grid)[place] != (*grid)[tmp] ||
		(*grid)[place] != (*grid)[place + dir] ||
		(*grid)[place] != (*grid)[place + dir * 2] ||
		(*grid)[place] != (*grid)[place + dir * 3] || ((
		canBeEat(grid, place) || canBeEat(grid, place + dir) || canBeEat(grid, tmp) ||
		canBeEat(grid, place + dir * 2) || canBeEat(grid, place + dir * 3)) && ending)
	)
		return 0;
	return (*grid)[place];
}

bool GameManager::canBeEat(std::map<unsigned short, char> *grid, unsigned short place) {
	bool res;

	res = false;
	res += checkEat(grid, place, 256);
	res += checkEat(grid, place, 255);
	res += checkEat(grid, place, 257);
	res += checkEat(grid, place, 65535);
	res += checkEat(grid, place, 1);
	res += checkEat(grid, place, 0xFF00);
	res += checkEat(grid, place, 0xFF01);
	res += checkEat(grid, place, 0xFEFF);
	return res;
}

void GameManager::replay( SDLManager *SDLMan ) {
	std::stack<unsigned short int> tmp;
	std::map<unsigned short int, char> griH;
	SDL_Event event;
	bool oneTwo = true;
	int tmpColor = 1;

	while (!this->history->empty()) {
		tmp.push(this->history->top());
		this->history->pop();
	}
	SDLMan->clear();
	SDLMan->render();
	while (!tmp.empty()) {
		SDL_WaitEvent(&event);
		if (event.key.keysym.sym == SDLK_h && event.key.type == SDL_KEYDOWN) {
			griH[tmp.top()] = tmpColor;
			if (this->canCapture)
				capture(&griH, tmp.top());
			this->printGrid(SDLMan, griH);
			SDLMan->render();
			tmp.pop();
			tmpColor = 3 - tmpColor;
			oneTwo = !oneTwo;
		}
		if (event.key.keysym.sym == SDLK_j && event.key.type == SDL_KEYDOWN) {
			std::cout << std::endl << "=========================================" << std::endl << std::endl;
			std::cout << "Black player :" << std::endl;
			std::cout << Heuristic(griH, GameManager::instance()->getHistory(), 1, oneTwo).run() << std::endl ;
			std::cout << "White player :" << std::endl;
			std::cout << Heuristic(griH, GameManager::instance()->getHistory(), 2, oneTwo).run() << std::endl ;
		}
		if (event.button.type == SDL_MOUSEBUTTONDOWN &&
			event.button.button == SDL_BUTTON_LEFT &&
			event.button.windowID == 1 &&
			event.button.x >= 125 && event.button.y >= 125 &&
			event.button.x <= 1075 && event.button.y <= 1075)
		{
			unsigned short pos = ((event.button.x - 125) / 50) + ((event.button.y - 125) / 50) * 256;
			griH[pos] = tmpColor;
			if (this->canCapture)
				capture(&griH, pos);
			this->printGrid(SDLMan, griH);
			SDLMan->render();
			tmp.pop();
			tmpColor = 3 - tmpColor;
			oneTwo = !oneTwo;
		}
		if (event.key.keysym.sym == SDLK_g && event.key.type == SDL_KEYDOWN) {
			APlayer *player;
			if (oneTwo)
				player = this->PlayerOne;
			else
				player = this->PlayerTwo;
			unsigned short pos = player->debugPlay( griH, 2 - oneTwo, 0, this->noDoubleThrees );
			std::cout << "(" << (pos >> 8) << ", " << (pos & 15) << ")" << std::endl;
		}
	}
}

unsigned short int GameManager::checkEat(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir) {
	if ((*grid)[place + dir] != (*grid)[place]) // pas de moi dans la direction
		return 0;
	if ((*grid)[place + dir * 2] == 0 && (*grid)[place - dir] == 3 - (*grid)[place]) // 2 1(*) 1 0
        return 1;
    if ((*grid)[place + dir * 2] == 3 - (*grid)[place] && (*grid)[place - dir] == 0) // 0 1(*) 1 2
        return 1;
	return 0;
}
GameManager *GameManager::instance(bool d) { /* singleton */
	if (!GameManager::p_instance) GameManager::p_instance = new GameManager(d);
	return GameManager::p_instance;
}
Vec GameManager::UsiToVec(unsigned short int usi) {
	return Vec(usi & 0xFF , usi >> 8);
}

unsigned short int GameManager::VecToUsi(Vec vec) {
	return (vec.x + vec.y * 256);
}
