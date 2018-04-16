#ifndef GAMEMANAGER_CLASS_H
# define GAMEMANAGER_CLASS_H

# include <iostream>
# include <map>
# include <string>
# include "SDLManager.class.hpp"
# include "APlayer.class.hpp"
# include "Human.class.hpp"
# include "Vec.class.hpp"

class GameManager {

	public:
		GameManager( void );
		virtual ~GameManager( void );
		void printGrid( SDLManager *SDLMan );
		char playTurn( SDLManager *SDLMan );

		static Vec UsiToVec(unsigned short int usi);
		static unsigned short int VecToUsi(Vec vec);
		static bool goodIput(std::map<unsigned short int, char> *grid, char value, SDL_Event *event);
		static char checkBoard(std::map<unsigned short int, char> *grid);

	private:
		std::map<unsigned short int, char> grid;

		char	turn;
		APlayer *PlayerOne;
		APlayer *PlayerTwo;

		static char capture(std::map<unsigned short int, char> *grid, unsigned short int place);
		static char checkCapture(std::map<unsigned short int, char> *grid, Vec place, Vec dir);
		static bool doubleThrees(std::map<unsigned short, char> *grid, unsigned short int place, char value);
		static char checkThrees(std::map<unsigned short int, char> *grid, Vec place, Vec dir, char value);
		static char checkFive(std::map<unsigned short int, char> *grid, Vec place, Vec dir);
		static bool canBeEat(std::map<unsigned short int, char> *grid, Vec place);
		static char checkEat(std::map<unsigned short int, char> *grid, Vec place, Vec dir);


};

#endif
