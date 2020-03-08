#pragma once

#include <cmath>

template<typename T>
class Vec2_
{
public:
	constexpr Vec2_() = default;
	constexpr Vec2_( T x,T y )
		:
		x( x ),
		y( y )
	{}
	// constexpr Vec2_( T amount )
	// 	:
	// 	x( amount ),
	// 	y( amount )
	// {}
	constexpr Vec2_( T amount ) = delete;

	template<typename U>
	constexpr operator Vec2_<U>() const
	{
		return Vec2_<U>( U( x ),U( y ) );
	}

	constexpr Vec2_ operator+( const Vec2_& rhs ) const
	{
		return Vec2_( x + rhs.x,y + rhs.y );
	}
	constexpr Vec2_& operator+=( const Vec2_& rhs )
	{
		return *this = *this + rhs;
	}
	constexpr Vec2_ operator*( T rhs ) const
	{
		return Vec2_( x * rhs,y * rhs );
	}
	constexpr Vec2_& operator*=( T rhs )
	{
		return *this = *this * rhs;
	}
	constexpr Vec2_ operator-( const Vec2_& rhs ) const
	{
		return Vec2_( x - rhs.x,y - rhs.y );
	}
	constexpr Vec2_& operator-=( const Vec2_& rhs )
	{
		return *this = *this - rhs;
	}
	constexpr Vec2_ operator/( T rhs ) const
	{
		return Vec2_{ x / rhs,y / rhs };
	}
	constexpr Vec2_& operator/=( T rhs )
	{
		*this = ( *this ) / rhs;
		return *this;
	}
	constexpr Vec2_ operator%( T rhs ) const
	{
		return Vec2_{ x % rhs,y % rhs };
	}
	constexpr Vec2_& operator%=( T rhs ) const
	{
		*this = ( *this ) % rhs;
		return( *this );
	}
	constexpr Vec2_ operator-() const
	{
		return( Vec2_<T>{ -x,-y } );
	}

	constexpr bool operator==( const Vec2_<T>& rhs ) const
	{
		return( x == rhs.x && y == rhs.y );
	}
	constexpr bool operator!=( const Vec2_<T>& rhs ) const
	{
		return( !( *this == rhs ) );
	}

	constexpr T GetLength() const
	{
		return T( std::sqrt( GetLengthSq() ) );
	}
	constexpr T GetLengthSq() const
	{
		return x * x + y * y;
	}
	constexpr T GetAngle() const
	{
		return( std::atan2( y,x ) );
	}

	constexpr Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	constexpr Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if( len != T( 0.0f ) )
		{
			return *this * ( 1.0f / len );
		}
		return *this;
	}
	// Returns vec2 where each component is -1, 0, or 1.
	constexpr Vec2_ GetOneized() const
	{
		Vec2_<T> temp = *this;
		if( temp.x != T( 0.0 ) ) temp.x /= abs( temp.x );
		if( temp.y != T( 0.0 ) ) temp.y /= abs( temp.y );
		return( temp );
	}

	static constexpr Vec2_ Up()
	{
		return Vec2_{ T( 0.0f ),T( -1.0f ) };
	}
	static constexpr Vec2_ Down()
	{
		return Vec2_{ T( 0.0f ),T( 1.0f ) };
	}
	static constexpr Vec2_ Left()
	{
		return Vec2_{ T( -1.0f ),T( 0.0f ) };
	}
	static constexpr Vec2_ Right()
	{
		return Vec2_{ T( 1.0f ),T( 0.0f ) };
	}
	static constexpr Vec2_ Zero()
	{
		return( Vec2_<T>{ T( 0.0 ),T( 0.0 ) } );
	}
	static constexpr Vec2_ One()
	{
		return( Vec2_<T>{ T( 1.0 ),T( 1.0 ) } );
	}
	static constexpr Vec2_ FromAngle( float angle )
	{
		return( Vec2_{ T( std::cos( angle ) ),
			T( std::sin( angle ) ) } );
	}

	constexpr Vec2_ X() const
	{
		return( Vec2_{ x,T( 0 ) } );
	}
	constexpr Vec2_ Y() const
	{
		return( Vec2_{ T( 0 ),y } );
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2;
typedef Vec2_<int> Vei2;