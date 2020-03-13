#include "stdafx.h"
#include "Surface.h"
#include <cassert>
#include <fstream>
#include "Palette.h"

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
	*this = Surface{ newSize.x,newSize.y };

	Fill( Colors::Magenta );

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

void Surface::Overlay( const Surface& other,const Vei2& pos )
{
	for( int y = 0; y < min( height,other.height ); ++y )
	{
		for( int x = 0; x < min( width,other.width ); ++x )
		{
			const auto pix = other.GetPixel( x,y );
			if( pix != Colors::Magenta )
			{
				PutPixel( pos.x + x,pos.y + y,pix );
			}
		}
	}
}

void Surface::Fill( Color fill )
{
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			PutPixel( x,y,fill );
		}
	}
}

void Surface::FillNonMagenta( Color fill )
{
	for( auto& pix : pixels )
	{
		if( pix != Colors::Magenta )
		{
			pix = fill;
		}
	}
}

void Surface::DrawRect( int x,int y,int width,int height,Color c )
{
	for( int yc = y; yc < y + height; ++yc )
	{
		for( int xc = x; xc < x + width; ++xc )
		{
			PutPixel( xc,yc,c );
		}
	}
}

void Surface::DrawRect( const RectI& area,Color c )
{
	DrawRect( area.left,area.top,area.GetWidth(),area.GetHeight(),c );
}

void Surface::Move( const RectI& selection,const Vei2& movement )
{
	const int xStart = max( selection.left,0 );
	const int yStart = max( selection.top,0 );
	const int xEnd = min( selection.right,width );
	const int yEnd = min( selection.bottom,height );

	Surface temp = *this;
	DrawRect( selection,Colors::Magenta );

	for( int y = yStart; y < yEnd; ++y )
	{
		for( int x = xStart; x < xEnd; ++x )
		{
			PutPixel( x + movement.x,y + movement.y,
				temp.GetPixel( x,y ) );
		}
	}
}

void Surface::Draw( HDC hdc,const Vei2& pos,float scale ) const
{
	static auto& colorRefs = GetColorPal();
	const int iscale = int( round( scale ) );
	const RECT start = RECT( RectI{ pos,iscale,iscale } );
	RECT rc = start;
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			const auto pix = GetPixel( x,y );
			if( pix != Colors::Magenta )
			{
				rc.left = start.left + int( float( x ) * scale );
				rc.top = start.top + int( float( y ) * scale );
				rc.right = rc.left + iscale + 1;
				rc.bottom = rc.top + iscale + 1;
				// const auto r = pix.GetR();
				// const auto g = pix.GetG();
				// const auto b = pix.GetB();
				FillRect( hdc,&rc,*colorRefs[pix] );
			}
		}
	}
}

void Surface::DrawDefault( HDC hdc,const Vei2& pos,float scale ) const
{
	static auto& colorRefs = GetDefaultColorPal();
	const int iscale = int( round( scale ) );
	const RECT start = RECT( RectI{ pos,iscale,iscale } );
	RECT rc = start;
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			const auto pix = GetPixel( x,y );
			assert( pix != 0 );
			if( pix != Colors::Magenta )
			{
				// rc.left = start.left + int( float( x ) * scale );
				// rc.top = start.top + int( float( y ) * scale );
				// rc.right = rc.left + iscale;
				// rc.bottom = rc.top + iscale;
				rc.left = start.left + int( float( x ) * scale );
				rc.top = start.top + int( float( y ) * scale );
				rc.right = rc.left + iscale + 1;
				rc.bottom = rc.top + iscale + 1;
				FillRect( hdc,&rc,*colorRefs[pix] );
			}
		}
	}
}

void Surface::CacheBrushes( const Surface& test,const Palette& pal )
{
	static auto& colorRefs = GetColorPal();
	colorRefs.clear();
	// colorRefs.insert( std::make_pair<unsigned int,const HBRUSH*>(
	// 	Colors::Magenta,pal.GetBrush( Colors::Magenta ) ) );
	for( auto pix : test.pixels )
	{
		if( colorRefs.find( pix ) == colorRefs.end() )
		{
			colorRefs.insert( std::make_pair<unsigned int,const HBRUSH*>(
				pix,pal.GetBrush( pix ) ) );
		}
	}
}

void Surface::CacheDefaultBrushes( const Surface& colors,const Palette& pal )
{
	static auto& refs = GetDefaultColorPal();
	refs.insert( std::make_pair<unsigned int,const HBRUSH*>(
		Colors::Magenta,pal.GetBrush( Colors::Magenta ) ) );
	for( auto pix : colors.pixels )
	{
		refs.insert( std::make_pair<unsigned int,const HBRUSH*>(
			pix,pal.GetDefaultBrush( pix ) ) );
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

Vei2 Surface::GetSize() const
{
	return( Vei2{ width,height } );
}

RectI Surface::GetRect() const
{
	return( RectI{ 0,width,0,height } );
}

std::vector<Color> Surface::FindUniqueColors() const
{
	std::vector<Color> uniqueColors;

	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			const auto pix = GetPixel( x,y );
			if( std::find( uniqueColors.begin(),uniqueColors.end(),
				pix ) == uniqueColors.end() )
			{
				uniqueColors.emplace_back( pix );
			}
		}
	}
	
	return( uniqueColors );
}

Surface Surface::GenerateOutline( const Surface& src,Color outlineCol )
{
	Surface outline{ src.width,src.height };
	outline.Fill( Colors::Magenta );

	for( int y = 0; y < src.height; ++y )
	{
		for( int x = 0; x < src.width; ++x )
		{
			if( src.GetPixel( x,y ) != Colors::Magenta )
			{
				if( x == 0 || x == src.width - 1 ||
					y == 0 || y == src.height - 1 )
				{
					outline.PutPixel( x,y,outlineCol );
				}
				else
				{
					std::vector<Color> pixs;
					pixs.emplace_back( src.GetPixel( x,y - 1 ) );
					pixs.emplace_back( src.GetPixel( x,y + 1 ) );
					pixs.emplace_back( src.GetPixel( x - 1,y ) );
					pixs.emplace_back( src.GetPixel( x + 1,y ) );

					for( auto pix : pixs )
					{
						if( pix == Colors::Magenta )
						{
							outline.PutPixel( x,y,outlineCol );
							break;
						}
					}
				}
			}
		}
	}
	return( outline );
}

Surface Surface::GetExpanded( float scale ) const
{
	Surface scaled{ int( float( width ) * scale ),
		int( float( height ) * scale ) };

	// static auto& colorRefs = GetColorPal();
	const int iscale = int( round( scale ) );
	const RectI start = RectI{ Vei2{ 0,0 },iscale,iscale };
	RectI rc = start;
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			const auto pix = GetPixel( x,y );
			if( pix != Colors::Magenta )
			{
				rc.left = start.left + int( float( x ) * scale );
				rc.top = start.top + int( float( y ) * scale );
				rc.right = rc.left + iscale;
				rc.bottom = rc.top + iscale;
				// FillRect( hdc,&rc,*colorRefs[pix] );
				scaled.DrawRect( rc,pix );
			}
		}
	}

	return( scaled );
}

std::unordered_map<unsigned int,const HBRUSH*>& Surface::GetColorPal()
{
	static std::unordered_map<unsigned int,const HBRUSH*> colorRefs;
	return( colorRefs );
}

std::unordered_map<unsigned int,const HBRUSH*>& Surface::GetDefaultColorPal()
{
	static std::unordered_map<unsigned int,const HBRUSH*> refs;
	return( refs );
}
