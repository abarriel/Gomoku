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

		unsigned short five;
	};

	public:
		Heuristic( std::map<unsigned short int, char> &grid, std::stack<unsigned short int> &history, char id, bool onPlay );
		virtual ~Heuristic( void );

		Heuristic::playerData getPlayerdata(int i) const;
		int getScore() const;
		Heuristic& run( void );

		void countLine( void );

		void deductScore();

	private:

		std::map<unsigned short int, char> &grid;
		std::stack<unsigned short int> &history;
		char id;
		bool onPlay;
		unsigned int nbUsless;
		int score;
		Heuristic::playerData p[2];

		int usless(unsigned short pos);
		int linePoint(int firInPlay, int secInPlay);
		void seqToLine(unsigned short cur, unsigned short dir);
		char deductLine(unsigned short seq);
		char deductEat(unsigned short seq);

};

 std::ostream &	operator<<( std::ostream & o, Heuristic const & i );

#endif
