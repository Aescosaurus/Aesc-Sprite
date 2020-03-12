#include "stdafx.h"
#include "Canvas.h"
#include "Palette.h"

Canvas::Canvas( const RectI& area,const Surface& img,
	const Palette& pal )
	:
	area( area ),
	image( img ),
	bgColor( pal.GetDefaultBrush( pal.GetDefaultColor( 0 ) ) ),
	onCol( pal.GetDefaultColor( 12 ) ),
	offCol( pal.GetDefaultColor( 13 ) ),
	bgSquares( image.GetWidth(),image.GetHeight() )
{
	for( int y = 0; y < bgSquares.GetHeight(); ++y )
	{
		for( int x = 0; x < bgSquares.GetWidth(); ++x )
		{
			Color col = onCol;
			if( y % 2 == 0 )
			{
				if( x % 2 == 0 ) col = offCol;
			}
			else
			{
				if( x % 2 != 0 ) col = offCol;
			}
			bgSquares.PutPixel( x,y,col );
		}
	}

	OnWindowResize( area );
	CacheImage( img );
}

void Canvas::OnWindowResize( const RectI& area )
{
	this->area = area;
}

void Canvas::OnPaint( HDC hdc )
{
	const auto rc = RECT( area );
	// if( bgColor == nullptr ) bgColor = HBRUSH( CreateSolidBrush( RGB( 40,40,40 ) ) );
	FillRect( hdc,&rc,*bgColor );
	// buffer.DrawRect( area,Color{ 40,40,40 } );
	bgSquares.DrawDefault( hdc,area.GetTopLeft() + imagePos,imageScale );
	image.Draw( hdc,area.GetTopLeft() + imagePos,imageScale );
	// buffer.Overlay( image );
	// TODO: Make sure to clip to window area.
}

void Canvas::CacheImage( const Surface& img )
{
	image = img;
}

Vei2& Canvas::GetImagePos()
{
	return( imagePos );
}

float& Canvas::GetImageScale()
{
	return( imageScale );
}

Vei2 Canvas::Mouse2CanvPos( const Vei2& mousePos ) const
{
	Vei2 temp = mousePos;
	temp -= area.GetTopLeft();
	temp -= imagePos;
	temp /= int( imageScale );
	return( temp );
}

Vei2 Canvas::CalcImagePos() const
{
	return( area.GetTopLeft() + imagePos );
}

float Canvas::GetImageScaling()
{
	return( imageScale );
}
