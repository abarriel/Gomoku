#ifndef GAMEMANAGER_CLASS_H
# define GAMEMANAGER_CLASS_H

# include <iostream>
# include <map>
# include <list>
# include <algorithm>
# include <stack>
# include <string>
#include <iomanip>
#include <sstream>
# include "SDLManager.class.hpp"
# include "APlayer.class.hpp"
# include "Human.class.hpp"
# include "BotHenry.class.hpp"
# include "Vec.class.hpp"

class GameManager {

	public:
		GameManager(bool asking = true);
		virtual ~GameManager( void );
		void printGrid(SDLManager *SDLMan, std::map<unsigned short int, char> gridE);
		void printGrid( SDLManager *SDLMan );
		char playTurn( SDLManager *SDLMan );
		unsigned short int threeTurn(APlayer *player, SDLManager *SDLMan);
		void twoTurn(APlayer *player, SDLManager *SDLMan);
		bool cantContinue( void );
		void replay( SDLManager *SDLMan );

		std::stack<unsigned short int> &getHistory( void ) const;

		bool getEnding( void );
		char otherPoint(char o);

		APlayer &getPlayer(char);
        void fillMap(void);
		static Vec UsiToVec(unsigned short int usi);
		static unsigned short int VecToUsi(Vec vec);
		static bool goodInput(std::map<unsigned short int, char> &grid, char value, unsigned short int pos, char mode, bool noDouble);
		static unsigned short int SeqFromTo(std::map<unsigned short, char>& grid, unsigned short from, unsigned short to, unsigned short dir, char value, unsigned short skip);
		static char capture(std::map<unsigned short int, char> *grid, unsigned short int place, char replace = 0);
		static void undoCapture(std::map<unsigned short int, char> *grid, unsigned short int place, char replace = 0);
		static GameManager *instance(bool = true);
        static char checkBoard(std::map<unsigned short int, char> *grid, bool ending);
        static char checkPosVictory(std::map<unsigned short int, char> *grid, unsigned short pos, bool ending);
        void debugGrid(void);
        static void debugGrid(std::map<unsigned short int, char> &grid);
	private:

		std::map<unsigned short int, char> grid;
		std::stack<unsigned short int> *history;
        static GameManager *p_instance;
		unsigned short int	turn;
		APlayer *PlayerOne;
		APlayer *PlayerTwo;
		bool canCapture;
		bool endingCapture;
		bool noDoubleThrees;
		char	gameMode;
		bool	swapOportunity;

		static char checkFive(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, bool ending);
        static char checkFiveRe(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, bool ending);
		static char checkCapture(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, char replace);
		static void removeCapture(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir, char replace);
		static bool doubleThrees(std::map<unsigned short, char> &grid, unsigned short int place, char value);
		static char checkThrees(std::map<unsigned short int, char>*grid, unsigned short place, unsigned short dir, char value);
        static bool canBeEat(std::map<unsigned short int, char> *grid, unsigned short place);
		static unsigned short int checkEat(std::map<unsigned short int, char> *grid, unsigned short place, unsigned short dir);
		static char SeqIsThree(unsigned short int seq);
		static unsigned short int SeqFromTo(std::map<unsigned short, char> &grid, Vec from, Vec to, Vec dir, char value, Vec skip);
};

#endif
