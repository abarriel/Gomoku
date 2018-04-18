#include "GameManager.class.hpp"

GameManager::GameManager( bool asking ) :
	turn(1),
	canCapture(true),
	endingCapture(true),
	noDoubleThrees(true),
	gameMode(0),
	swapOportunity(false)
{
	std::string res;

	if (asking) {
		std::cout << "Game mode (STANDARD/pro/long-pro/swap/swap2):" << std::endl;
		std::cin >> res;
		if (res == "pro")
			this->gameMode = 2;
		if (res == "long-pro")
			this->gameMode = 3;
		if (res == "swap")
			this->gameMode = 4;
		if (res == "swap2")
			this->gameMode = 5;
		std::cout << "Capture mode (Y/n):" << std::endl;
		std::cin >> res;
		if (res == "n") {
			this->canCapture = false;
			this->endingCapture = false;
		} else {
			std::cout << "Ending capture mode (Y/n):" << std::endl;
			std::cin >> res;
			if (res == "n")
				this->endingCapture = false;
		}
		std::cout << "No double-threes (Y/n):" << std::endl;
		std::cin >> res;
		if (res == "n")
			this->noDoubleThrees = false;
		std::cout << "Human player 1 (Y/n):" << std::endl;
		std::cin >> res;
		if (res == "n")
			this->PlayerOne = new BotHenry();
		else
			this->PlayerOne = new Human();
		std::cout << "Human player 2 (Y/n):" << std::endl;
		std::cin >> res;
		if (res == "n")
			this->PlayerTwo = new BotHenry();
		else
			this->PlayerTwo = new Human();
	} else {
		this->PlayerOne = new Human("Black");
		this->PlayerTwo = new Human("White");
	}
	this->history = new std::stack<unsigned short int>;
	return;
}

std::stack<unsigned short int> const &GameManager::getHistory() const {
        return *this->history;
}

GameManager::~GameManager( void ) {
	delete this->PlayerOne;
	delete this->PlayerTwo;
	return;
}

void GameManager::printGrid(SDLManager *SDLMan) {
	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			SDLMan->placeRock(this->grid[y * 256 + x], Vec(x, y));
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
			if (goodInput(&this->grid, ((this->turn + 1) % 2) + 1, y * 256 + x, 0, this->noDoubleThrees))
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
        player->setStopUntilPlace().clear();
    }
	if (!skipPlace)
		this->grid[place] = ((this->turn + 1) % 2) + 1;
	if (this->canCapture)
		player->increasePoint(capture(&this->grid, place));
	this->printGrid(SDLMan);
	SDLMan->render();
	if (player->haveWin() || checkBoard(&this->grid, this->endingCapture)) {
		player->victory();
		return (1);
	}
	this->turn++;
	if (this->cantContinue())
		return 1;
	return 0;
}

char GameManager::capture(std::map<unsigned short int, char> *grid, unsigned short int place) {
	char res = 0;
	Vec Vplace = UsiToVec(place);

	res += checkCapture(grid, Vplace, Vec(1, 0));
	res += checkCapture(grid, Vplace, Vec(1, 1));
	res += checkCapture(grid, Vplace, Vec(0, 1));
	res += checkCapture(grid, Vplace, Vec(-1, 1));
	res += checkCapture(grid, Vplace, Vec(1, -1));
	res += checkCapture(grid, Vplace, Vec(0, -1));
	res += checkCapture(grid, Vplace, Vec(-1, -1));
	res += checkCapture(grid, Vplace, Vec(-1, 0));
	return res;
}

char GameManager::checkCapture(std::map<unsigned short int, char> *grid, Vec place, Vec dir) {
	Vec tmp = place + dir * 3;

	if (tmp.x < 0 || tmp.x > 18 || tmp.y < 0 || tmp.y > 18 ||
		(*grid)[VecToUsi(tmp)] != (*grid)[VecToUsi(place)] ||
		3 - (*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir)] ||
		3 - (*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir * 2)]
	)
		return 0;
	(*grid)[VecToUsi(place + dir)] = 0;
	(*grid)[VecToUsi(place + dir * 2)] = 0;
	return 2;
}

unsigned short int GameManager::SeqFromTo(std::map<unsigned short, char> grid, Vec from, Vec to, Vec dir, char value, Vec skip) {
	unsigned short int res;

	res = 0;
	while (from != to + dir) {
		res = res << 2;
		if (from.x >= 0 && from.x <= 18 && from.y >= 0 && from.y <= 18) {
			if (grid[VecToUsi(from)]) {
				if (grid[VecToUsi(from)] == value)
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
	if ((seq | 61455) == 61775)
		return 1;
	if ((seq | 49215) == 50495)
		return 1;
	if ((seq | 64515) == 64595)
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

bool GameManager::doubleThrees(std::map<unsigned short, char> *grid, unsigned short int place, char value) {
	unsigned short int seq;
	Vec Vplace = UsiToVec(place);
	Vec dir = Vec(1, 0);
	char res = 0;

	seq = SeqFromTo(*grid, Vplace - dir * 4, Vplace + dir * 4, dir, value, Vplace);
	res += SeqIsThree(seq);
	dir = Vec(0, 1);
	seq = SeqFromTo(*grid, Vplace - dir * 4, Vplace + dir * 4, dir, value, Vplace);
	res += SeqIsThree(seq);
	dir = Vec(1, 1);
	seq = SeqFromTo(*grid, Vplace - dir * 4, Vplace + dir * 4, dir, value, Vplace);
	res += SeqIsThree(seq);
	dir = Vec(-1, 1);
	seq = SeqFromTo(*grid, Vplace - dir * 4, Vplace + dir * 4, dir, value, Vplace);
	res += SeqIsThree(seq);
	if (res > 1)
		return 1;
	return 0;
}

bool GameManager::goodInput(std::map<unsigned short, char> *grid, char value, unsigned short int pos, char mode, bool noDouble = true) {
	if (mode & 2) {
		Vec tmp = UsiToVec(pos);
		if (tmp.x >= (7 - (mode & 1)) && tmp.y >= (7 - (mode & 1)) &&
			tmp.x <= (11 + (mode & 1)) && tmp.y <= (11 + (mode & 1)))
			return false;
	}
	if ((*grid)[pos] != 0 || (noDouble && doubleThrees(grid, pos, value)))
		return false;
	return true;
}

char GameManager::checkBoard(std::map<unsigned short int, char> *grid, bool ending = true) {
	char res = 0;

	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			res = this->checkFive(grid, Vec(x, y), Vec(1, 0), ending);
			res += this->checkFive(grid, Vec(x, y), Vec(0, 1), ending);
			res += this->checkFive(grid, Vec(x, y), Vec(1, 1), ending);
			res += this->checkFive(grid, Vec(x, y), Vec(1, -1), ending);
            if (res) 
                return res;
		}
	}
	return res;
}

char GameManager::checkFive(std::map<unsigned short int, char> *grid, Vec place, Vec dir, bool ending = true) {
	Vec tmp = place + dir * 4;
    std::list<unsigned short> checkBeEat;
    std::list<unsigned short> res;

	if ((*grid)[VecToUsi(place)] == 0)
		return 0;
    checkBeEat = canBeEat(grid, place);
    for (int i = 1; i <= 4; i++) {
        res = canBeEat(grid, place + dir * i);
        checkBeEat.merge(res);
    }
    if(!checkBeEat.empty() && !ending)
        checkBeEat.clear();
	if (tmp.x < 0 || tmp.x > 18 || tmp.y < 0 || tmp.y > 18 ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(tmp)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir * 2)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir * 3)]) {
            return 0;
        }
    if (!checkBeEat.empty())
    {
        if ((*grid)[VecToUsi(place)] == 1) // PLAYER 1
            this->PlayerTwo->setStopUntilPlace() = checkBeEat;
        else if ((*grid)[VecToUsi(place)] == 2) // PLAYER 1
            this->PlayerOne->setStopUntilPlace() = checkBeEat;
        return 0;
    }
	return (*grid)[VecToUsi(place)];
}

std::list<unsigned short> GameManager::canBeEat(std::map<unsigned short, char> *grid, Vec place) {
    std::list<unsigned short> nextEnnemyPos;
    // res = false;
    for (int xe = 1; xe >= -1; xe--)
        for (int ye = 1; ye >= -1; ye--) {
            if ((xe == 0 && ye == 0)) continue;
            unsigned short res = checkEat(grid, place, Vec(xe, ye));
            if (res == 0) continue;
            nextEnnemyPos.push_back(res - 1);
        }
	return nextEnnemyPos;
}

unsigned short int GameManager::checkEat(std::map<unsigned short int, char> *grid, Vec place, Vec dir) {
	if ((*grid)[VecToUsi(place + dir)] != (*grid)[VecToUsi(place)]) // pas de moi dans la direction 
		return 0;
	if ((*grid)[VecToUsi(place + dir * 2)] == 0 && (*grid)[VecToUsi(place - dir)] == 3 - (*grid)[VecToUsi(place)]) // 2 1(*) 1 0
	 	return ((VecToUsi(place + dir * 2)) + 1);
    if ((*grid)[VecToUsi(place + dir * 2)] == 3 - (*grid)[VecToUsi(place)] && (*grid)[VecToUsi(place - dir)] == 0) // 0 1(*) 1 2
        return ((VecToUsi(place - dir)) + 1);
	return 0;
}

Vec GameManager::UsiToVec(unsigned short int usi) {
	return Vec(usi & 15, usi >> 8);
}

unsigned short int GameManager::VecToUsi(Vec vec) {
	return (vec.x + vec.y * 256);
}
