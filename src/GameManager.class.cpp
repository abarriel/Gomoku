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
	return;
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

	place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees );
	this->grid[place] = ((this->turn + 1) % 2) + 1;
	this->printGrid(SDLMan);
	SDLMan->render();
	place = player->play( this->grid, 3 - (((this->turn + 1) % 2) + 1), 0, this->noDoubleThrees );
	this->grid[place] = 3 - (((this->turn + 1) % 2) + 1);
	this->printGrid(SDLMan);
	SDLMan->render();
}

unsigned short int GameManager::threeTurn(APlayer *player, SDLManager *SDLMan) {
	unsigned short int place;

	place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees );
	this->grid[place] = ((this->turn + 1) % 2) + 1;
	this->printGrid(SDLMan);
	SDLMan->render();
	place = player->play( this->grid, 3 - (((this->turn + 1) % 2) + 1), 0, this->noDoubleThrees );
	this->grid[place] = 3 - (((this->turn + 1) % 2) + 1);
	this->printGrid(SDLMan);
	SDLMan->render();
	return player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees );
}

bool GameManager::cantContinue() {
	for (unsigned short int x = 0; x < 19; x++) {
		for (unsigned short int y = 0; y < 19; y++) {
			if (goodIput(&this->grid, ((this->turn + 1) % 2) + 1, y * 256 + x, 0, this->noDoubleThrees))
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
			place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees );
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
			place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees );
	}
	else if (this->turn == 1 && this->gameMode & 4) {
		place = this->threeTurn(player, SDLMan);
		this->swapOportunity = true;
	}
	else if (this->turn == 1 && this->gameMode & 2)
		place = 2313;
	else if (this->turn == 3 && this->gameMode & 2)
		place = player->play( this->grid, ((this->turn + 1) % 2) + 1, this->gameMode, this->noDoubleThrees );
	else
		place = player->play( this->grid, ((this->turn + 1) % 2) + 1, 0, this->noDoubleThrees );
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

bool GameManager::goodIput(std::map<unsigned short, char> *grid, char value, unsigned short int pos, char mode, bool noDouble = true) {
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
			if ((res = checkFive(grid, Vec(x, y), Vec(1, 0), ending)))
				return res;
			if ((res = checkFive(grid, Vec(x, y), Vec(0, 1), ending)))
				return res;
			if ((res = checkFive(grid, Vec(x, y), Vec(1, 1), ending)))
				return res;
			if ((res = checkFive(grid, Vec(x, y), Vec(1, -1), ending)))
				return res;
		}
	}
	return res;
}

char GameManager::checkFive(std::map<unsigned short int, char> *grid, Vec place, Vec dir, bool ending = true) {
	Vec tmp = place + dir * 4;

	if ((*grid)[VecToUsi(place)] == 0)
		return 0;
	if (tmp.x < 0 || tmp.x > 18 || tmp.y < 0 || tmp.y > 18 ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(tmp)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir * 2)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir * 3)] || ((
		canBeEat(grid, place) || canBeEat(grid, place + dir) || canBeEat(grid, tmp) ||
		canBeEat(grid, place * 2) || canBeEat(grid, place + dir * 3)) && ending)
	)
		return 0;
	return (*grid)[VecToUsi(place)];
}

bool GameManager::canBeEat(std::map<unsigned short, char> *grid, Vec place) {
	bool res;

	res = false;
	res += checkEat(grid, place, Vec(1, 0));
	res += checkEat(grid, place, Vec(1, -1));
	res += checkEat(grid, place, Vec(1, 1));
	res += checkEat(grid, place, Vec(0, -1));
	res += checkEat(grid, place, Vec(0, 1));
	res += checkEat(grid, place, Vec(-1, 0));
	res += checkEat(grid, place, Vec(-1, 1));
	res += checkEat(grid, place, Vec(-1, -1));
	return res;
}

char GameManager::checkEat(std::map<unsigned short int, char> *grid, Vec place, Vec dir) {
	if ((*grid)[VecToUsi(place + dir)] != (*grid)[VecToUsi(place)])
		return 0;
	if ((*grid)[VecToUsi(place + dir * 2)] == 0 && (*grid)[VecToUsi(place - dir)] == 3 - (*grid)[VecToUsi(place)])
	 	return 1;
		if ((*grid)[VecToUsi(place + dir * 2)] == 3 - (*grid)[VecToUsi(place)] && (*grid)[VecToUsi(place - dir)] == 0)
		 	return 1;
	return 0;
}

Vec GameManager::UsiToVec(unsigned short int usi) {
	return Vec(usi & 15, usi >> 8);
}

unsigned short int GameManager::VecToUsi(Vec vec) {
	return (vec.x + vec.y * 256);
}
