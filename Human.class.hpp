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
		virtual ~Human( void );
		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value ) const;

};


#endif
