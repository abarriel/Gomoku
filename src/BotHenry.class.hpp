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

// # define MAX_DEPTH 2

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
        static void generateAttack(std::map<unsigned short, char> &grid, std::vector<int> &moves, char &value, char &mode, bool &noDouble, char depth);

		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		virtual unsigned short int debugPlay( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;

		static int getScore(std::map<unsigned short int, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta, bool &is_done);
        static int getAttack(std::map<unsigned short, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos, int alpha, int beta, char already);
		static unsigned int getFirstScore(resThread &restT, std::map<unsigned short int, char> grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth,  unsigned short cur);
        static bool getSquare(std::map<unsigned short int, char> &grid, unsigned short value);
		virtual bool wantSwap( std::map<unsigned short int, char> grid ) const;
		virtual bool wantDoublePlay( std::map<unsigned short int, char> grid ) const;
};


#endif
