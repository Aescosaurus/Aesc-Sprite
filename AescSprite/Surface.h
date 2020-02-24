#pragma once

#include "Color.h"
#include <string>
#include "Rect.h"
#include <vector>

class Surface
{
public:
	// Create blank surface with width and height.
	Surface( int width,int height );
	// Load a bitmap(.bmp) file with filename into surface.
	Surface( const std::string& filename );
public:
	Surface( const Surface& ) = default;
	Surface& operator=( const Surface& ) = default;

	Surface( Surface&& donor );
	Surface& operator=( Surface&& rhs );

	void PutPixel( int x,int y,Color c );

	Color GetPixel( int x,int y ) const;
	int GetWidth() const;
	int GetHeight() const;
private:
	std::vector<Color> pixels;
	int width;
	int height;
};