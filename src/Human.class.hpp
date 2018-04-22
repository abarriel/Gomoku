#ifndef HUMAN_CLASS_H
# define HUMAN_CLASS_H

# include <iostream>
# include <string>
# include "APlayer.class.hpp"
# include "GameManager.class.hpp"
# include <SDL2/SDL.h>

class Human : public APlayer{

	public:
		Human( std::string name );
		Human( void );
		virtual ~Human( void );
		virtual unsigned short int debugPlay( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		virtual bool wantSwap( std::map<unsigned short int, char> grid ) const;
		virtual bool wantDoublePlay( std::map<unsigned short int, char> grid ) const;
		static bool goodAwnser(SDL_Event *event);
		static bool goodInput(std::map<unsigned short int, char> *grid, char value, SDL_Event *event, char mode, bool noDouble);
};


#endif
