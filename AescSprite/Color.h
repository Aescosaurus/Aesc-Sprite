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
public:
	uint dword;
};