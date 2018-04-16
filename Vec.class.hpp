#ifndef VEC_CLASS_H
# define VEC_CLASS_H

# include <iostream>

class Vec {

	public:
		Vec( Vec const & src );
		Vec( float xe, float ye );
		virtual ~Vec( void );

		Vec &	operator=( Vec const & rhs );

		int			operator==( Vec const & cs ) const;

		Vec 		operator+( Vec const & cs ) const;
		Vec 		operator-( Vec const & cs ) const;
		Vec 		operator*( Vec const & cs ) const;
		Vec 		operator*( int const & i ) const;

		float		x;
		float		y;

	private:
		Vec( void );

};

std::ostream &	operator<<( std::ostream & o, Vec const & i );

#endif
