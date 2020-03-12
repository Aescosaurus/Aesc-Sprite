#include "stdafx.h"
#include "Palette.h"
#include "Surface.h"
#include <cassert>
#include <algorithm>

Palette::Palette( const RectI& area )
	:
	area( area )
{
	LoadDefaultPalette( Surface{ "Palettes/Theme.bmp" } );
	OnWindowResize( area );
}

Tool::ReturnType Palette::OnMouseDown( const Vei2& pos )
{
	for( int i = 0; i < int( colors.size() ); ++i )
	{
		if( colors[i].area.ContainsPoint( pos ) )
		{
			selectedColor = i;
			return( Tool::ReturnType::Repaint );
		}
	}
	
	return( Tool::ReturnType::None );
}

void Palette::LoadPalette( const Surface& pal )
{
	colors.clear();
	assert( pal.GetHeight() == 1 );
	for( int x = 0; x < pal.GetWidth(); ++x )
	{
		const auto pix = pal.GetPixel( x,0 );
		if( pix != Colors::Magenta )
		{
			colors.emplace_back( ColorItem{ pix,
				RectI{ 0,0,0,0 } } );
		}
	}
	colors.emplace_back( ColorItem{ Color{ 255,0,255 },
		RectI{ 0,0,0,0 } } );

	Surface::CacheBrushes( pal,*this );

	OnWindowResize( area );
}

void Palette::LoadDefaultPalette( const Surface& pal )
{
	assert( pal.GetHeight() == 1 );
	for( int x = 0; x < pal.GetWidth(); ++x )
	{
		const auto pix = pal.GetPixel( x,0 );
		if( pix != Colors::Magenta )
		{
			defaultColors.emplace_back( ColorItem{ pix,
				RectI{ 0,0,0,0 } } );
		}
	}
	colors.emplace_back( ColorItem{ Color{ 255,0,255 },
		RectI{ 0,0,0,0 } } );

	Surface::CacheDefaultBrushes( pal,*this );

	LoadPalette( pal );
}

void Palette::GeneratePalette( const std::string& src )
{
	selectedColor = 0;
	const Surface img{ src };
	std::vector<Color> uniqueColors;
	for( int y = 0; y < img.GetHeight(); ++y )
	{
		for( int x = 0; x < img.GetWidth(); ++x )
		{
			const auto pix = img.GetPixel( x,y );
			if( std::find( uniqueColors.begin(),uniqueColors.end(),
				pix ) == uniqueColors.end() )
			{
				uniqueColors.emplace_back( pix );
			}
		}
	}

	Surface tempPal{ int( uniqueColors.size() ),1 };
	for( int i = 0; i < int( uniqueColors.size() ); ++i )
	{
		tempPal.PutPixel( i,0,uniqueColors[i] );
	}
	LoadPalette( tempPal );
}

void Palette::OnWindowResize( const RectI& area )
{
	// TODO: Calculate padding.
	this->area = area;
	const float itemSize = float( area.GetWidth() ) /
		float( max( 1,colors.size() / 2 ) );

	const Vei2 start = area.GetTopLeft();
	auto pos = Vec2( start );
	for( auto& col : colors )
	{
		col.area = RectI{ Vei2( pos ),
			int( ceil( itemSize ) ),int( ceil( itemSize ) ) };
		pos.x += itemSize;
		if( pos.x >= area.right - itemSize / 2 )
		{
			pos.x = float( start.x );
			pos.y += itemSize;
		}
	}
}

void Palette::OnPaint( HDC hdc )
{
	if( defaultColors[0].solidBrush == nullptr ||
		colors[0].solidBrush == nullptr )
	{
		for( auto& item : defaultColors )
		{
			const auto rc = RECT( item.area );
			if( item.solidBrush == nullptr )
			{
				item.solidBrush = CreateSolidBrush( item.col.dword );
			}
			FillRect( hdc,&rc,item.solidBrush );
		}
	}
	// else
	{
		for( int i = 0; i < int( colors.size() ) - 1; ++i )
		{
			auto& item = colors[i];
			const auto rc = RECT( item.area );
			if( item.solidBrush == nullptr )
			{
				item.solidBrush = CreateSolidBrush( item.col.dword );
			}
			// buffer.DrawRect( item.area,item.col );
			FillRect( hdc,&rc,item.solidBrush );
		}
	}
}

void Palette::SelectColor( Color c )
{
	for( int i = 0; i < int( colors.size() ); ++i )
	{
		if( colors[i].col == c )
		{
			selectedColor = i;
			return;
		}
	}
	assert( false );
}

const RectI& Palette::GetArea() const
{
	return( area );
}

int Palette::GetBottom() const
{
	return( colors[colors.size() - 2].area.bottom );
}

const HBRUSH* Palette::GetBrush( Color c ) const
{
	for( const auto& item : colors )
	{
		if( item.col == c )
		{
			return( &item.solidBrush );
		}
	}
	assert( false );
	return( nullptr );
}

const HBRUSH* Palette::GetDefaultBrush( Color c ) const
{
	for( const auto& item : defaultColors )
	{
		if( item.col == c )
		{
			return( &item.solidBrush );
		}
	}
	assert( false );
	return( nullptr );
}

Color Palette::GetColor( int i ) const
{
	assert( i >= 0 );
	assert( i < int( colors.size() ) );
	return( colors[i].col );
}

Color Palette::GetDefaultColor( int i ) const
{
	assert( i >= 0 );
	assert( i < int( defaultColors.size() ) );
	return( defaultColors[i].col );
}

Color Palette::GetSelectedColor() const
{
	return( colors[selectedColor].col );
}

Palette::ColorItem::ColorItem( Color c,const RectI& area )
	:
	col( c ),
	area( area ),
	solidBrush( nullptr )
{}

Palette::ColorItem::~ColorItem()
{
	DeleteObject( solidBrush );
}
