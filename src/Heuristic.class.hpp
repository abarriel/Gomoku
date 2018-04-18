#ifndef HEURASTIC_CLASS_H
# define HEURASTIC_CLASS_H

# include <map>
# include <stack>

# include <iostream>
# include "GameManager.class.hpp"

class Heuristic {

	struct playerData {
		unsigned short threeFree;
		unsigned short threeHalf;

		unsigned short fourFree;
		unsigned short fourHalf;
	};

	public:
		Heuristic( std::map<unsigned short int, char> &grid, std::stack<unsigned short int> &history, char id );
		virtual ~Heuristic( void );

		unsigned int getScore() const;
		void run( void );

		void countLine( void );

		void deductScore();

	private:

		std::map<unsigned short int, char> &grid;
		std::stack<unsigned short int> &history;
		char id;
		unsigned int score;
		Heuristic::playerData p[2];

		void seqToLine(unsigned short cur, unsigned short dir);
		char deductLine(unsigned short seq);

};

// std::ostream &	operator<<( std::ostream & o, Heuristic const & i );

#endif
