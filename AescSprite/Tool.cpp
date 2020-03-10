#include "stdafx.h"
#include "Tool.h"

Surface* Tool::activeLayer = nullptr;
Canvas* Tool::canv = nullptr;
Palette* Tool::pal = nullptr;
RectI Tool::selectArea;

Tool::Tool( const std::string& icon,unsigned char swapKey,
	unsigned char tempSelectKey )
	:
	icon( icon ),
	swapKey( swapKey ),
	tempSelectKey( tempSelectKey )
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

Tool::ReturnType Tool::OnMouseDown( const Vei2& pos )
{
	mouseDown = true;
	return( ReturnType::None );
}

Tool::ReturnType Tool::OnMouseUp( const Vei2& pos )
{
	mouseDown = false;
	return( ReturnType::None );
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

unsigned char Tool::GetTempSelectKey() const
{
	return( tempSelectKey );
}

const RectI& Tool::GetArea() const
{
	return( area );
}
