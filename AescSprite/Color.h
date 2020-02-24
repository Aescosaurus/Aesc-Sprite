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