#include "GameManager.class.hpp"

GameManager::GameManager( void ) : turn(1), PlayerOne(new Human("Human 1")), PlayerTwo(new Human("Human 2")) {

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

char GameManager::playTurn(SDLManager *SDLMan) {
	unsigned short int place;
	APlayer *player;

	if (this->turn == 1)
		player = PlayerOne;
	else
		player = PlayerTwo;
	place = player->play( this->grid, this->turn );
	this->grid[place] = this->turn;
	player->increasePoint(capture(&this->grid, place));
	this->printGrid(SDLMan);
	SDLMan->render();
	if (player->haveWin() || checkBoard(&this->grid)) {
		player->victory();
		return (1);
	}
	turn = 3 - turn;
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

bool GameManager::doubleThrees(std::map<unsigned short, char> *grid, unsigned short int place, char value) {
	char check;
	int res = 0;
	Vec Vplace = UsiToVec(place);

	check = (*grid)[place];
	(*grid)[place] = value;
	res += checkThrees(grid, Vplace, Vec(1, 0), value) || checkThrees(grid, Vplace - Vec(1, 0), Vec(1, 0), value);
	res += checkThrees(grid, Vplace, Vec(1, 1), value) || checkThrees(grid, Vplace - Vec(1, 1), Vec(1, 1), value);
	res += checkThrees(grid, Vplace, Vec(0, 1), value) || checkThrees(grid, Vplace - Vec(0, 1), Vec(0, 1), value);
	res += checkThrees(grid, Vplace, Vec(-1, 1), value) || checkThrees(grid, Vplace - Vec(-1, 1), Vec(-1, 1), value);
	res += checkThrees(grid, Vplace, Vec(1, -1), value) || checkThrees(grid, Vplace - Vec(1, -1), Vec(1, -1), value);
	res += checkThrees(grid, Vplace, Vec(0, -1), value) || checkThrees(grid, Vplace - Vec(0, -1), Vec(0, -1), value);
	res += checkThrees(grid, Vplace, Vec(-1, -1), value) || checkThrees(grid, Vplace - Vec(-1, -1), Vec(-1, -1), value);
	res += checkThrees(grid, Vplace, Vec(-1, 0), value) || checkThrees(grid, Vplace - Vec(-1, 0), Vec(-1, 0), value);
	(*grid)[place] = check;

	if (res > 1)
		return true;
	return false;
}

char GameManager::checkThrees(std::map<unsigned short int, char> *grid, Vec place, Vec dir, char value) {
	Vec tmp = place + dir * 4;
	Vec tmp2 = place - dir;
	char check = 0;

	if (tmp.x < 0 || tmp.x > 18 || tmp.y < 0 || tmp.y > 18 ||
		tmp2.x < 0 || tmp2.x > 18 || tmp2.y < 0 || tmp2.y > 18 ||
		3 - value == (*grid)[VecToUsi(place + dir)] ||
		3 - value == (*grid)[VecToUsi(place + dir * 2)] ||
		3 - value == (*grid)[VecToUsi(place + dir * 3)] ||
		3 - value == (*grid)[VecToUsi(place + dir * 4)] ||
		3 - value == (*grid)[VecToUsi(place - dir)] ||
		3 - value == (*grid)[VecToUsi(place)]
	)
		return 0;
	check += (value == (*grid)[VecToUsi(place + dir)]) ? 1 : 0;
	check += (value == (*grid)[VecToUsi(place + dir * 2)]) ? 1 : 0;
	check += (value == (*grid)[VecToUsi(place + dir * 3)]) ? 1 : 0;
	check += (value == (*grid)[VecToUsi(place)]) ? 1 : 0;
	if (check == 3) {
		return 1;
	}
	return 0;
}

bool GameManager::goodIput(std::map<unsigned short, char> *grid, char value, SDL_Event *event) {
	unsigned short int pos;

	if (event->button.type == SDL_MOUSEBUTTONDOWN &&
		event->button.button == SDL_BUTTON_LEFT &&
		event->button.windowID == 1 &&
		event->button.x >= 125 && event->button.y >= 125 &&
		event->button.x <= 1075 && event->button.y <= 1075)
	{
		pos = ((event->button.x - 125) / 50) + ((event->button.y - 125) / 50) * 256;
		if ((*grid)[pos] != 0 || doubleThrees(grid, pos, value))
			return false;
		return true;
	}
	return false;
}

char GameManager::checkBoard(std::map<unsigned short int, char> *grid) {
	char res = 0;

	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			if ((res = checkFive(grid, Vec(x, y), Vec(1, 0))))
				return res;
			if ((res = checkFive(grid, Vec(x, y), Vec(0, 1))))
				return res;
			if ((res = checkFive(grid, Vec(x, y), Vec(1, 1))))
				return res;
			if ((res = checkFive(grid, Vec(x, y), Vec(1, -1))))
				return res;
		}
	}
	return res;
}

char GameManager::checkFive(std::map<unsigned short int, char> *grid, Vec place, Vec dir) {
	Vec tmp = place + dir * 4;

	if ((*grid)[VecToUsi(place)] == 0)
		return 0;
	if (tmp.x < 0 || tmp.x > 18 || tmp.y < 0 || tmp.y > 18 ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(tmp)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir * 2)] ||
		(*grid)[VecToUsi(place)] != (*grid)[VecToUsi(place + dir * 3)] ||
		canBeEat(grid, place) || canBeEat(grid, place + dir) || canBeEat(grid, tmp) ||
		canBeEat(grid, place * 2) || canBeEat(grid, place + dir * 3)
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
