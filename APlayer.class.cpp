#include "APlayer.class.hpp"

APlayer::APlayer( std::string nm ) : name(nm), point(0) {
	return;
}

APlayer::APlayer( void ) : point(0) {
	std::cout << "Name it:" << std::endl;
	std::cin >> this->name;
	return;
}

void APlayer::victory ( void ) const {
	std::cout << name << " win !!" << std::endl;
}

APlayer::~APlayer( void ) {

	return;
}

bool APlayer::haveWin( void ) const {
	if (this->point >= 10)
		return true;
	return false;
}

void APlayer::increasePoint( char pnt ) {
	if (pnt > 0)
		std::cout << this->name << " take " << (int)pnt << " rock" << std::endl;
	this->point += pnt;
}

std::string APlayer::getName() const {
	return name;
}
