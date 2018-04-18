#ifndef GAMEMANAGER_CLASS_H
# define GAMEMANAGER_CLASS_H

# include <iostream>
# include <map>
# include <list>
# include <algorithm>
# include <string>
# include "SDLManager.class.hpp"
# include "APlayer.class.hpp"
# include "Human.class.hpp"
# include "BotHenry.class.hpp"
# include "Vec.class.hpp"

class GameManager {

	public:
		GameManager(bool asking = true);
		virtual ~GameManager( void );
		void printGrid( SDLManager *SDLMan );
		char playTurn( SDLManager *SDLMan );
		unsigned short int threeTurn(APlayer *player, SDLManager *SDLMan);
		void twoTurn(APlayer *player, SDLManager *SDLMan);
		bool cantContinue( void );

		static Vec UsiToVec(unsigned short int usi);
		static unsigned short int VecToUsi(Vec vec);
		static bool goodInput(std::map<unsigned short int, char> *grid, char value, unsigned short int pos, char mode, bool noDouble);
		char checkBoard(std::map<unsigned short int, char> *grid, bool ending);

	private:

		std::map<unsigned short int, char> grid;

		unsigned short int	turn;
		APlayer *PlayerOne;
		APlayer *PlayerTwo;
		bool canCapture;
		bool endingCapture;
		bool noDoubleThrees;
		char	gameMode;
		bool	swapOportunity;

		char checkFive(std::map<unsigned short int, char> *grid, Vec place, Vec dir, bool ending);
		static char capture(std::map<unsigned short int, char> *grid, unsigned short int place);
		static char checkCapture(std::map<unsigned short int, char> *grid, Vec place, Vec dir);
		static bool doubleThrees(std::map<unsigned short, char> *grid, unsigned short int place, char value);
		static char checkThrees(std::map<unsigned short int, char> *grid, Vec place, Vec dir, char value);
		static std::list<unsigned short> canBeEat(std::map<unsigned short int, char> *grid, Vec place);
		static unsigned short int checkEat(std::map<unsigned short int, char> *grid, Vec place, Vec dir);
		static char SeqIsThree(unsigned short int seq);
		static unsigned short int SeqFromTo(std::map<unsigned short, char> grid, Vec from, Vec to, Vec dir, char value, Vec skip);
};

#endif
