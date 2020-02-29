#include "stdafx.h"
#include "Palette.h"
#include "Surface.h"
#include <cassert>

Palette::Palette( const RectI& area )
	:
	area( area )
{
	OnWindowResize( area );
}

void Palette::LoadPalette( const std::string& src )
{
	const Surface pal{ src };
	assert( pal.GetHeight() == 1 );
	for( int x = 0; x < pal.GetWidth(); ++x )
	{
		colors.emplace_back( ColorItem{ pal.GetPixel( x,0 ),
			RectI{ 0,0,0,0 } } );
	}
	colors.emplace_back( ColorItem{ Color{ 255,0,255 },
		RectI{ 0,0,0,0 } } );

	Surface::CacheBrushes( pal,*this );

	OnWindowResize( area );
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
	for( auto& item : colors )
	{
		const auto rc = RECT( item.area );
		if( item.solidBrush == nullptr )
		{
			item.solidBrush = CreateSolidBrush( item.col.dword );
		}
		FillRect( hdc,&rc,item.solidBrush );
		// FillRect( hdc,&rc,CreateSolidBrush( item.c.dword ) );
	}
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
