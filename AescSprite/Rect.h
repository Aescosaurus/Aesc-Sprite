#pragma once

#include "Vec2.h"
#include <windef.h>
#include <algorithm>
#include "Color.h"

template<typename T>
class Rect_
{
public:
	constexpr Rect_() = default;
	constexpr Rect_( T left_in,T right_in,T top_in,T bottom_in )
		:
		left( left_in ),
		right( right_in ),
		top( top_in ),
		bottom( bottom_in )
	{}
	constexpr Rect_( const Vec2_<T>& topLeft,const Vec2_<T>& bottomRight )
		:
		Rect_( topLeft.x,bottomRight.x,topLeft.y,bottomRight.y )
	{}
	constexpr Rect_( const Vec2_<T>& topLeft,T width,T height )
		:
		Rect_( topLeft,topLeft + Vec2_<T>( width,height ) )
	{}

	template<typename U>
	constexpr operator Rect_<U>() const
	{
		return Rect_<U>( U( left ),U( right ),U( top ),U( bottom ) );
	}
	constexpr operator RECT() const
	{
		return( RECT{ LONG( left ),LONG( top ),LONG( right ),LONG( bottom ) } );
	}

	void Draw( HDC hdc,Color c ) const
	{
		HPEN pen = CreatePen( PS_SOLID,1,c );
		SelectObject( hdc,pen );
		MoveToEx( hdc,left,top,nullptr );
		LineTo( hdc,right,top );
		LineTo( hdc,right,bottom );
		LineTo( hdc,left,bottom );
		LineTo( hdc,left,top );
		DeleteObject( pen );
	}

	constexpr bool IsOverlappingWith( const Rect_& other ) const
	{
		return right > other.left && left < other.right
			&& bottom > other.top && top < other.bottom;
	}
	constexpr bool IsContainedBy( const Rect_& other ) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	constexpr bool ContainsPoint( const Vec2_<T>& pos ) const
	{
		return pos.x > left && pos.x < right &&
			pos.y > top && pos.y < bottom;
	}
	
	constexpr Rect_ MoveTo( const Vec2_<T>& point )
	{
		right += point.x - left;
		bottom += point.y - top;
		left = point.x;
		top = point.y;

		return( *this );
	}
	constexpr Rect_ MoveBy( const Vec2_<T>& amount )
	{
		left += amount.x;
		right += amount.x;
		top += amount.y;
		bottom += amount.y;

		return( *this );
	}
	constexpr Rect_ Scale( T amount )
	{
		left *= amount;
		right *= amount;
		top *= amount;
		bottom *= amount;
		
		return( *this );
	}
	constexpr Rect_ Fix()
	{
		if( left > right ) std::swap( left,right );
		if( top > bottom ) std::swap( top,bottom );

		return( *this );
	}
	constexpr Rect_ GetMovedBy( const Vec2_<T>& amount ) const
	{
		auto temp = *this;
		temp.MoveBy( amount );
		return( temp );
	}
	constexpr Rect_ GetMovedTo( const Vec2_<T>& pos ) const
	{
		auto temp = *this;
		temp.MoveTo( pos );
		return( temp );
	}
	constexpr Rect_ Resize( const Vec2_<T>& size )
	{
		right = left + size.x;
		bottom = top + size.y;

		return( *this );
	}

	static constexpr Rect_ FromCenter( const Vec2_<T>& center,
		T halfWidth,T halfHeight )
	{
		const Vec2_<T> half( halfWidth,halfHeight );
		return Rect( center - half,center + half );
	}
	constexpr Rect_ GetExpanded( T offset ) const
	{
		return Rect_( left - offset,right + offset,top - offset,bottom + offset );
	}
	constexpr Rect_ GetExpandedTL( T offset ) const
	{
		return Rect_( left - offset,right,top - offset,bottom );
	}
	constexpr Rect_ GetExpandedX( T offset ) const
	{
		return( Rect_{ left - offset,right + offset,top,bottom } );
	}
	constexpr Rect_ GetExpandedY( T offset ) const
	{
		return( Rect_{ left,right,top - offset,bottom + offset } );
	}
	constexpr Vec2_<T> GetCenter() const
	{
		return Vec2_<T>( ( left + right ) / T( 2 ),( top + bottom ) / T( 2 ) );
	}
	constexpr Vec2_<T> GetSize() const
	{
		return( Vec2_<T>{ GetWidth(),GetHeight() } );
	}
	constexpr Vec2_<T> GetTopLeft() const
	{
		return( Vec2_<T>{ left,top } );
	}

	constexpr T GetWidth() const
	{
		return right - left;
	}
	constexpr T GetHeight() const
	{
		return bottom - top;
	}
public:
	T left;
	T right;
	T top;
	T bottom;
};

typedef Rect_<float> Rect;
typedef Rect_<int> RectI;