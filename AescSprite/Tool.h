#pragma once

#include "Vec2.h"
#include "Surface.h"
#include "Canvas.h"

class Tool
{
public:
	enum class ReturnType
	{
		None,
		Repaint,
		RegenImage
	};
public:
	Tool( const Surface& icon )
		:
		icon( icon )
	{}

	void SetCanvasRef( Canvas& canv )
	{
		this->canv = &canv;
	}

	virtual ReturnType OnMouseDown( const Vei2& pos ) { return( ReturnType::None ); }
	virtual ReturnType OnMouseUp( const Vei2& pos ) { return( ReturnType::None ); }
	virtual ReturnType OnMouseMove( const Vei2& pos ) { return( ReturnType::None ); }

	virtual ReturnType OnKeyDown( unsigned char key ) { return( ReturnType::None ); }
	virtual ReturnType OnKeyUp( unsigned char key ) { return( ReturnType::None ); }

	void OnWindowResize( const RectI& area )
	{
		drawScale = float( area.GetHeight() ) / icon.GetHeight();
		this->area = area;
		this->area.right = area.left + icon.GetWidth() * int( drawScale );
	}

	virtual void OnPaint( HDC hdc )
	{
		icon.Draw( hdc,area.GetTopLeft(),drawScale );
		// buffer.Overlay( icon,area.GetTopLeft() );
	}

	void CacheImage( Surface& img )
	{
		activeLayer = &img;
	}

	RectI GetNextRect() const
	{
		return( area.GetMovedBy( Vei2::Right() *
			icon.GetWidth() * int( drawScale ) ) );
	}
protected:
	float drawScale = 0.0f;
	Surface icon;
	RectI area = RectI{ 0,0,0,0 };
	Surface* activeLayer = nullptr;
	Canvas* canv = nullptr;
};