#include "stdafx.h"
#include "Surface.h"
#include <cassert>
#include <fstream>

Surface::Surface( int width,int height )
	:
	width( width ),
	height( height ),
	pixels( width * height )
{}

Surface::Surface( const std::string& filename )
{
	std::ifstream file( filename,std::ios::binary );
	assert( file );

	BITMAPFILEHEADER bmFileHeader;
	file.read( reinterpret_cast< char* >( &bmFileHeader ),
		sizeof( bmFileHeader ) );

	BITMAPINFOHEADER bmInfoHeader;
	file.read( reinterpret_cast< char* >( &bmInfoHeader ),
		sizeof( bmInfoHeader ) );

	assert( bmInfoHeader.biBitCount == 24 ||
		bmInfoHeader.biBitCount == 32 );
	assert( bmInfoHeader.biCompression == BI_RGB );

	const bool is32b = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	// Test for reverse row order and
	//  control y loop accordingly.
	int yStart;
	int yEnd;
	int dy;
	if( bmInfoHeader.biHeight < 0 )
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pixels.resize( width * height );

	file.seekg( bmFileHeader.bfOffBits );
	// Padding is for 24 bit depth only.
	const int padding = ( 4 - ( width * 3 ) % 4 ) % 4;

	for( int y = yStart; y != yEnd; y += dy )
	{
		for( int x = 0; x < width; ++x )
		{
			PutPixel( x,y,Color( file.get(),file.get(),file.get() ) );
			if( is32b )
			{
				file.seekg( 1,std::ios::cur );
			}
		}
		if( !is32b )
		{
			file.seekg( padding,std::ios::cur );
		}
	}
}

Surface::Surface( Surface&& donor )
{
	*this = std::move( donor );
}

Surface& Surface::operator=( Surface&& rhs )
{
	width = rhs.width;
	height = rhs.height;
	pixels = std::move( rhs.pixels );

	rhs.width = 0;
	rhs.height = 0;

	return( *this );
}

void Surface::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	pixels.data()[y * width + x] = c;
}

void Surface::Resize( const Vei2& newSize )
{
	Surface old = *this;
	*this = Surface( newSize.x,newSize.y );

	const auto minWidth = min( GetWidth(),old.GetWidth() );
	const auto minHeight = min( GetHeight(),old.GetHeight() );
	for( int y = 0; y < minHeight; ++y )
	{
		for( int x = 0; x < minWidth; ++x )
		{
			PutPixel( x,y,old.GetPixel( x,y ) );
		}
	}
}

Color Surface::GetPixel( int x,int y ) const
{
	return( pixels[y * width + x] );
}

int Surface::GetWidth() const
{
	return( width );
}

int Surface::GetHeight() const
{
	return( height );
}