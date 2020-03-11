#include "stdafx.h"
#include "WriteToBitmap.h"
#include <fstream>
#include <cassert>

void WriteToBitmap::Write( const Surface& data,
	const std::string& name )
{
	std::ofstream out{ name,std::ios::out | std::ios::binary };
	assert( out.good() );

	const int mcuWidth = ( data.GetWidth() + 7 ) / 8;
	const int mcuHeight = ( data.GetHeight() + 7 ) / 8;
	const int paddingSize = data.GetWidth() % 4;
	const int size = 14 + 40 +
		( data.GetWidth() * data.GetHeight() * 3 ) +
		( paddingSize * data.GetHeight() );

	// First part of the header.
	out.put( 'B' ); // Tells that this is a bitmap.
	out.put( 'M' );
	PutInt( out,size ); // Total file size.
	PutShort( out,0 ); // Four unused values as four zeroes.
	PutShort( out,0 ); // Four unused values as four zeroes.
	PutInt( out,14 + 40 ); // Starting address of pixel array.

	// DIB header.
	PutInt( out,40 ); // Size of the DIB header.
	PutInt( out,data.GetWidth() ); // Width and height as
	PutInt( out,data.GetHeight() ); //  4 byte integers.
	PutShort( out,1 ); // Number of planes.
	PutShort( out,24 ); // Number of bits per pixel.
	PutInt( out,0 ); // Compression.
	PutInt( out,0 ); // Size of raw pixel data.
	PutInt( out,0 ); // Pixel per meter BS.
	PutInt( out,0 );
	PutInt( out,0 ); // Number of colors in palette.
	PutInt( out,0 ); // Important colors.

	// Write all the rows to the output file.
	for( int y = data.GetHeight() - 1; y >= 0; --y )
	{
		for( int x = 0; x < data.GetWidth(); ++x )
		{
			// PutInt( out,int( data.GetPixel( x,y ).dword ) );
			const auto pix = data.GetPixel( x,y );
			out.put( pix.GetR() );
			out.put( pix.GetG() );
			out.put( pix.GetB() );
		}
		for( int i = 0; i < paddingSize; ++i )
		{
			out.put( 0 );
		}
	}
}

void WriteToBitmap::PutShort( std::ofstream& out,uint v )
{
	out.put( ( v >> 0 ) & 0xFF );
	out.put( ( v >> 8 ) & 0xFF );
}

void WriteToBitmap::PutInt( std::ofstream& out,uint v )
{
	out.put( ( v >> 0 ) & 0xFF );
	out.put( ( v >> 8 ) & 0xFF );
	out.put( ( v >> 16 ) & 0xFF );
	out.put( ( v >> 24 ) & 0xFF );
}