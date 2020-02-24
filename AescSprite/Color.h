#pragma once

class Color
{
	typedef unsigned int uint;
public:
	Color() = default;
	constexpr Color( uint r,uint g,uint b )
		:
		dword( RGB( r,g,b ) )
	{}

	constexpr int GetR() const
	{
		return( ( dword >> 16u ) & 0xFFu );
	}
	constexpr int GetG() const
	{
		return( ( dword >> 8u ) & 0xFFu );
	}
	constexpr int GetB() const
	{
		return( dword & 0xFFu );
	}

	constexpr operator uint() const
	{
		return( dword );
	}

	constexpr bool operator==( Color rhs ) const
	{
		return( dword == rhs.dword );
	}
	constexpr bool operator!=( Color rhs ) const
	{
		return( dword != rhs.dword );
	}
public:
	uint dword;
};

namespace Colors
{
	static constexpr Color Magenta = Color{ 255,0,255 };
}