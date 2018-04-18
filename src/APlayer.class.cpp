#include "APlayer.class.hpp"

APlayer::APlayer( std::string nm ) : name(nm), point(0), stopUntilPlace(0) {
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

std::list<unsigned short int> APlayer::getStopUntilPlace(void) const { return this->stopUntilPlace; }

 std::list<unsigned short int>& APlayer::setStopUntilPlace(void) { return this->stopUntilPlace; }

bool APlayer::checkForceToPlace(unsigned short int newPos) const {
    (void)newPos;
    std::list<unsigned short int>currentList = this->getStopUntilPlace();
    if (currentList.empty())
        return true;
    std::list<unsigned short int>::iterator iter = std::find(currentList.begin(), currentList.end(), newPos);
    if (currentList.end() == iter)
        return false;
    return true;
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
	return this->name;
}

char APlayer::getPoint() const {
	return this->point;
}
