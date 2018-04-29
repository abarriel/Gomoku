#include "SDLManager.class.hpp"
SDLManager::SDLManager( void ) {
	// SDL_Surface* img;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return;
    this->MainWindow = SDL_CreateWindow("Gomoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1200, SDL_WINDOW_SHOWN);
    if(this->MainWindow == 0)
        return;
	this->MainRenderer = SDL_CreateRenderer(this->MainWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *board = IMG_Load("img/board.png");
    SDL_Surface *black = IMG_Load("img/black.png");
    SDL_Surface *white = IMG_Load("img/white.png");
    SDL_Surface *wouldPlay = IMG_Load("img/wouldPlay.png");
	if (board == 0)
		return;
    SDL_SetColorKey(board, 0, SDL_MapRGB(board->format, 0, 0, 0));
	this->Background = SDL_CreateTextureFromSurface(this->MainRenderer, board);
	// SDL_FreeSurface(img);
	if (black == 0)
		return;
    SDL_SetColorKey(black, 0, SDL_MapRGB(black->format, 0, 0, 0));
	this->BlackRock = SDL_CreateTextureFromSurface(this->MainRenderer, black);
	// SDL_FreeSurface(img);
    if (white == 0)
		return;
    SDL_SetColorKey(white, 0, SDL_MapRGB(white->format, 0, 0, 0));
	this->WhiteRock = SDL_CreateTextureFromSurface(this->MainRenderer, white);

    if (wouldPlay == 0)
		return;
    SDL_SetColorKey(wouldPlay, 0, SDL_MapRGB(wouldPlay->format, 0, 0, 0));
	this->wouldPlay = SDL_CreateTextureFromSurface(this->MainRenderer, wouldPlay);
	// SDL_FreeSurface(img);
	SDL_RenderCopy(this->MainRenderer, this->Background, NULL, NULL);
}

void SDLManager::placeRock(int color, Vec vec) {
	SDL_Rect pos;

	pos.x = 46 + 59.6 * vec.x;
	pos.y = 46 + 59.6 * vec.y;
	pos.w = 41;
  	pos.h = 41;
	switch(color) {
    	case 1 : SDL_RenderCopy(this->MainRenderer, this->BlackRock, NULL, &pos);
             break;
    	case 2 : SDL_RenderCopy(this->MainRenderer, this->WhiteRock, NULL, &pos);
             break;
        case 4 : SDL_RenderCopy(this->MainRenderer, this->wouldPlay, NULL, &pos);
             break;
		default : break;
	}
    // SDL_SetColorKey(this->Background, 1, SDL_MapRGB(this->Background->format, 0, 0, 0));
}

void SDLManager::placeRock(int color, unsigned short int point) {
	SDL_Rect pos;

	pos.x = 46 + 59.6 * (point & 15);
	pos.y = 46 + 59.6 * (point >> 8);
	pos.w = 41;
  	pos.h = 41;
	switch(color) {
    	case 1 : SDL_RenderCopy(this->MainRenderer, this->BlackRock, NULL, &pos);
             break;
    	case 2 : SDL_RenderCopy(this->MainRenderer, this->WhiteRock, NULL, &pos);
             break;
        case 4 : SDL_RenderCopy(this->MainRenderer, this->wouldPlay, NULL, &pos);
             break;
		default : break;
	}
}

void SDLManager::clear(){
	SDL_RenderClear(this->MainRenderer);
	SDL_RenderCopy(this->MainRenderer, this->Background, NULL, NULL);
}

void SDLManager::render() {
	SDL_RenderPresent(this->MainRenderer);
	SDL_RenderCopy(this->MainRenderer, this->Background, NULL, NULL);
}


SDLManager::~SDLManager( void ) {
	SDL_DestroyTexture(this->BlackRock);
	SDL_DestroyTexture(this->WhiteRock);
	SDL_DestroyTexture(this->Background);
	SDL_DestroyRenderer(this->MainRenderer);
    SDL_DestroyWindow(this->MainWindow);
    SDL_Quit();
	return;
}
