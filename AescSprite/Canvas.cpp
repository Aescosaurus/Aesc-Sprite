#include "stdafx.h"
#include "Canvas.h"

Canvas::Canvas( const RectI& area,const Surface& img,
	const Palette& pal )
	:
	area( area ),
	image( img ),
	bgColor( pal.GetBrush( pal.GetColor( 7 ) ) )
{
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
