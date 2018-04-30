#ifndef HUMANHELP_CLASS_H
# define HUMANHELP_CLASS_H

# include <iostream>
# include <string>
# include "APlayer.class.hpp"
# include "GameManager.class.hpp"
# include <SDL2/SDL.h>

class HumanHelp : public APlayer{

	public:
		HumanHelp( std::string name );
		HumanHelp( void );
		virtual ~HumanHelp( void );
		virtual unsigned short int debugPlay( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble ) const;
		virtual bool wantSwap( std::map<unsigned short int, char> grid ) const;
		virtual bool wantDoublePlay( std::map<unsigned short int, char> grid ) const;
		static bool goodAwnser(SDL_Event *event);
		static bool goodInput(std::map<unsigned short int, char> *grid, char value, SDL_Event *event, char mode, bool noDouble);
};


#endif
