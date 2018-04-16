#ifndef APLAYER_CLASS_H
# define APLAYER_CLASS_H

# include <iostream>
# include <string>
# include <map>

class APlayer {

	public:
		APlayer( std::string nm );
		virtual ~APlayer( void );
		virtual unsigned short int play( std::map<unsigned short int, char> grid, char value) const = 0;
		void victory( void ) const;
		void increasePoint( char pnt );
		bool haveWin( void ) const;
		std::string getName( void ) const;
	private:
		std::string name;
		char point;
};

#endif
