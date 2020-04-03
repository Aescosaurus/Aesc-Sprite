#include "stdafx.h"
#include "Tool.h"
#include "Palette.h"
#include <cassert>

Surface* Tool::activeLayer = nullptr;
Canvas* Tool::canv = nullptr;
Palette* Tool::pal = nullptr;
RectI Tool::selectArea;
// Color Tool::cursorCol = Colors::Magenta;
int Tool::curMouseIcon = 0;

Tool::Tool( const std::string& icon,unsigned char swapKey,
	const std::string& mouseIcon,unsigned char tempSelectKey )
	:
	icon( icon ),
	swapKey( swapKey ),
	tempSelectKey( tempSelectKey )
{
	cursorCols[0] = pal->GetDefaultColor( 0 );
	cursorCols[1] = pal->GetDefaultColor( 12 );
	if( mouseIcon.length() > 0 )
	{
		assert( pal != nullptr );
		for( int i = 0; i < 2; ++i )
		{
			mouseIcons[i] = Surface{ mouseIcon };
			mouseIcons[i].FillNonMagenta( cursorCols[i] );
		}
		// this->mouseIcon = Surface{ mouseIcon };
		// this->mouseIcon2 = mouseIcon;
		// this->mouseIcon2.FillNonMagenta( cursorCol );
	}
}

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

void Tool::UpdateSelectArea()
{
	selectArea = activeLayer->GetRect();
}

void Tool::UpdateCursorCol( const Vei2& mousePos,HDC hdc )
{
	Color pix{ 0,0,0 };
	pix.dword = GetPixel( hdc,mousePos.x,mousePos.y );

	// cursorCol = pal->GetDefaultColor( pix.GetBrightestColor() > 127 ? 0 : 12 );
	curMouseIcon = pix.GetBrightestColor() > 127 ? 0 : 1;
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

Tool::ReturnType Tool::OnMouseMove( const Vei2& pos )
{
	mousePos = pos;
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

void Tool::PaintIcon( HDC hdc )
{
	mouseIcons[curMouseIcon].DrawDefault( hdc,mousePos,drawScale );
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
