#ifndef APLAYER_CLASS_H
# define APLAYER_CLASS_H

# include <iostream>
# include <string>
# include <map>

class APlayer {

	public:
		APlayer( std::string nm );
		APlayer( void );
		virtual ~APlayer( void );
		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value, char mode, bool noDouble) const = 0;
		virtual bool wantSwap( std::map<unsigned short int, char> grid ) const = 0;
		virtual bool wantDoublePlay( std::map<unsigned short int, char> grid ) const = 0;
		void victory( void ) const;
		void increasePoint( char pnt );
		bool haveWin( void ) const;
		std::string getName( void ) const;
	private:
		std::string name;
		char point;
};

#endif
