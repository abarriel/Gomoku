#ifndef BOTHENRY_CLASS_H
# define BOTHENRY_CLASS_H

# include <iostream>
# include <string>
# include "APlayer.class.hpp"
# include "GameManager.class.hpp"
# include "Heuristic.class.hpp"
# include <chrono>
# include <thread>
# include <deque>
# include <queue>
# include <future>
# include <vector>

struct resThread{
    int curScore;
    unsigned short res;
};

class BotHenry : public APlayer{

	public:
		BotHenry( std::string name );
		BotHenry( void );
		virtual ~BotHenry( void );
        static void popScore(std::map<unsigned short, char> &grid, unsigned short pos, unsigned short dir, char &bestMoveScore);

		static void generateMove(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble, char depth);
        static void generateAttack(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble, char depth, bool &flag);

		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		virtual unsigned short int debugPlay( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;

		static void undoPlay(std::map<unsigned short, char> &grid, unsigned short pos);
		static char simulatePlay(std::map<unsigned short, char> &grid, unsigned short pos, char value);

		static int getScore(std::map<unsigned short int, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta);
        static int getAttack(std::map<unsigned short, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta, char already);
		static unsigned int getFirstScore(resThread &restT, std::map<unsigned short int, char> grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth,  unsigned short cur);
        static bool getSquare(std::map<unsigned short int, char> &grid, unsigned short value);
		virtual bool wantSwap( std::map<unsigned short int, char> grid ) const;
		virtual bool wantDoublePlay( std::map<unsigned short int, char> grid ) const;
		static unsigned short int initiate(std::map<unsigned short, char> grid);

	private:
		static char nbMove;
};


#endif
