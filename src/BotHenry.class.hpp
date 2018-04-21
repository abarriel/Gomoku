#ifndef BOTHENRY_CLASS_H
# define BOTHENRY_CLASS_H

# include <iostream>
# include <string>
# include "APlayer.class.hpp"
# include "GameManager.class.hpp"
# include "Heuristic.class.hpp"
# include <chrono>
# include <thread>
#include <vector>
# define MAX_DEPTH 2

struct resThread{
    int curScore;
    unsigned short res;
};

class BotHenry : public APlayer{

	public:
		BotHenry( std::string name );
		BotHenry( void );
		virtual ~BotHenry( void );
		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		static int getScore(std::map<unsigned short int, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth, unsigned short &pos);
		static unsigned int getFirstScore(resThread &restT, std::map<unsigned short int, char> grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth,  unsigned short cur);
        static bool getSquare(std::map<unsigned short int, char> &grid, unsigned short value);
		virtual bool wantSwap( std::map<unsigned short int, char> grid ) const;
		virtual bool wantDoublePlay( std::map<unsigned short int, char> grid ) const;
};


#endif
