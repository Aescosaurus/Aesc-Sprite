#include "stdafx.h"
#include "Canvas.h"

Canvas::Canvas( const RectI& area,const Surface& img )
	:
	area( area ),
	image( img )
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
	if( bgColor == nullptr ) bgColor = HBRUSH( CreateSolidBrush( RGB( 40,40,40 ) ) );
	FillRect( hdc,&rc,bgColor );
	image.Draw( hdc,area.GetTopLeft() + imagePos,imageScale );
	// TODO: Make sure to clip to window area.
}

void Canvas::CacheImage( const Surface& img )
{
	image = img;
}
