#pragma once

class Color
{
	typedef unsigned int uint;
public:
	Color() = default;
	constexpr Color( int r,int g,int b )
		:
		dword( RGB( r,g,b ) )
	{}
public:
	uint dword;
};