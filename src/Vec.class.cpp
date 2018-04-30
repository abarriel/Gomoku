#include "Vec.class.hpp"

Vec::Vec( void ) {

	return;
}

Vec::Vec( float xe, float ye ) : x(xe), y(ye){
	return;
}

Vec::Vec( Vec const & src ) {
	*this = src;
	return;
}

Vec::~Vec( void ) {

	return;
}

Vec Vec::operator+(Vec const & cs) const {
	return Vec(cs.x + this->x, cs.y + this->y);
}

Vec Vec::operator-(Vec const & cs) const {
	return Vec(this->x - cs.x, this->y - cs.y);
}

Vec Vec::operator*(Vec const & cs) const {
	return Vec(cs.x * this->x, cs.y * this->y);
}

Vec Vec::operator*(int const & i) const {
	return Vec(i * this->x, i * this->y);
}

int			Vec::operator==( Vec const & cs ) const {
	return ((int)this->x == (int)cs.x && (int)this->y == (int)cs.y);
}

int			Vec::operator!=( Vec const & cs ) const {
	return ((int)this->x != (int)cs.x || (int)this->y != (int)cs.y);
}

Vec &	Vec::operator=(Vec const &rhs) {
	if (this != &rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
	}
	return *this;
}

std::ostream &	operator<<( std::ostream & o, Vec const & i ) {
    (void)i;
	return o;
}
