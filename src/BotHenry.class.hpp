#ifndef BOTHENRY_CLASS_H
# define BOTHENRY_CLASS_H

# include <iostream>
# include <string>
# include "APlayer.class.hpp"
# include "GameManager.class.hpp"
# include "Heuristic.class.hpp"

# define MAX_DEPTH 2      // need to be even

class BotHenry : public APlayer{

	public:
		BotHenry( std::string name );
		BotHenry( void );
		virtual ~BotHenry( void );
		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		unsigned int getScore(std::map<unsigned short int, char> &grid, char value, char mode, bool noDouble, char currentPoint, char oponentPoint, char depth);


		virtual bool wantSwap( std::map<unsigned short int, char> grid ) const;
		virtual bool wantDoublePlay( std::map<unsigned short int, char> grid ) const;
};


#endif
