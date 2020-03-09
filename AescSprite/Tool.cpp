#include "stdafx.h"
#include "Tool.h"

Surface* Tool::activeLayer = nullptr;
Canvas* Tool::canv = nullptr;
Palette* Tool::pal = nullptr;
RectI Tool::selectArea = RectI{ 0,0,0,0 };

Tool::Tool( const Surface& icon,unsigned char swapKey )
	:
	icon( icon ),
	swapKey( swapKey )
{}

void Tool::SetCanvasRef( Canvas& canv )
{
	Tool::canv = &canv;
}

void Tool::SetPalRef( Palette& pal )
{
	Tool::pal = &pal;
}

void Tool::CacheImage( Surface& img )
{
	Tool::activeLayer = &img;
}

void Tool::OnWindowResize( const RectI& area )
{
	drawScale = float( area.GetHeight() ) / icon.GetHeight();
	this->area = area;
	this->area.right = area.left + icon.GetWidth() * int( drawScale );
}

void Tool::OnPaint( HDC hdc )
{
	icon.DrawDefault( hdc,area.GetTopLeft(),drawScale );
	// buffer.Overlay( icon,area.GetTopLeft() );
}

RectI Tool::GetNextRect() const
{
	auto temp = Rect( area );
	temp.MoveBy( Vec2::Right() * float( icon.GetWidth() ) *
		drawScale );
	return( temp );
	// return( area.GetMovedBy( Vei2::Right() *
	// 	icon.GetWidth() * int( drawScale ) ) );
}

unsigned char Tool::GetSwapKey() const
{
	return( swapKey );
}
