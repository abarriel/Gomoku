#include "Heuristic.class.hpp"

Heuristic::~Heuristic( void ) {

	return;
}

Heuristic::Heuristic( std::map<unsigned short int, char> &gr, std::stack<unsigned short int> &hi, char id ) : grid(gr), history(hi), id(id), score(0)
{
	bzero(this->p, sizeof(Heuristic::playerData) * 2);
	return;
}

unsigned int Heuristic::getScore() const { return this->score; }

void Heuristic::run() {
	(void)this->id;
	(void)this->history;
	this->countThree();
}

void Heuristic::seqToThree(unsigned short cur, unsigned short dir) {
	unsigned short seq;
	char res;

	seq = GameManager::SeqFromTo(this->grid, cur - (dir * 2), cur + (dir * 4), dir, this->grid[cur], cur);
	res = deductThree(seq);
	if (res == 1)
		this->p[this->grid[cur] - 1].threeHalf++;
	else if(res == 2)
		this->p[this->grid[cur] - 1].threeFree++;
	else if(res == 3)
		this->p[this->grid[cur] - 1].fourHalf++;
	else if(res == 4)
		this->p[this->grid[cur] - 1].fourFree++;
}

char Heuristic::deductThree(unsigned short seq) {
    if (((seq | 0xF0FF) == 0xF4FF) || ((seq | 0xFCFF) == 0xFDFF)) {
        return 0;
    }
	if ((seq | 0xFF0F) == 0xFF5F) {
		if ((seq | 0xFC00) == 0xFC54 ) {
			return 4;
		} else if (((seq | 0xFF00) == 0xFC51) || ((seq | 0xFF00) == 0xFC54) || ((seq | 0xFC03) == 0xFC57)) {
			return 3;
		} else if (((seq | 0xFC00) == 0xFC50) || ((seq | 0xF003) == 0xF053)) {
			return 2;
		} else if (((seq | 0xFC03) == 0xF053) || ((seq | 0xFF00) == 0xFF50) || ((seq | 0xF00F) == 0xF05F)) {
			return 1;
		}
	} else if ((seq | 0xFF03) == 0xFF17) {
		if ((seq | 0xFF00) == 0xFF15) {
			return 3;
		} else if ((seq | 0xFC00) == 0xFC14) {
			return 2;
		} else if (((seq | 0xFC03) == 0xFC17) || ((seq | 0xFF00) == 0xFF14)) {
			return 1;
		}
	} else if ((seq | 0xFF03) == 0xFF47) {
		if ((seq | 0xFF00) == 0xFF45) {
			return 3;
		} else if ((seq | 0xFC00) == 0xFC44) {
			return 2;
		} else if (((seq | 0xFC03) == 0xFC47) || ((seq | 0xFF00) == 0xFF44)) {
			return 1;
		}
	}
	return 0;
}

void Heuristic::countThree( void ) {
	unsigned short cur;

	for (char x = 0; x < 19; x++) {
		for (char y = 0; y < 19; y++) {
			cur = y * 256 + x;
			if (this->grid[cur] != 0) {
				this->seqToThree(cur, 1);
				this->seqToThree(cur, 256);
				this->seqToThree(cur, 257);
				this->seqToThree(cur, 255);
			}
		}
	}
}

// std::ostream &	operator<<( std::ostream & o, Heuristic const & i ) {
// 	Heuristic() = i; // delete this SHIT AND ADD FU..... USE OF I
// 	std::cout << "Some information from Heuristic instance" << std::endl;
// 	return o;
// }

/*

three schema:

@ : emplacement que l'on verifie
! : needed to be Free
X : y a des trucs mais on s'en bat les couilles frere
B : bloque (enemi / out of map)

!!@11!! -> X0@1110  FREE FOUR
		-> XX@1101 -> XX@1110 -> X0@111X HALF FOUR
		-> 00@110X -> X0@1100  FREE THREE
		-> X0@110X -> XB@1100 -> 00@11BX  HALF THREE

X!@011! -> XX@0111 HALF FOUR
		-> X0@0110 FREE THREE
		-> X0@011X -> XX@0110 HALF THREE


XXXX 000X 0xFF03

X!@101! -> XX@1011 HALF FOUR
		-> X0@1010 FREE THREE
		-> X0@101X -> XX@1010 HALF THREE


xxxx 000x

XXXX 00XX 65295
XXX0 0000 64512

*/

// 211102