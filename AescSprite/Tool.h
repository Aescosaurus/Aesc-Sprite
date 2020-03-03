#pragma once

#include "Vec2.h"
#include "Surface.h"

class Tool
{
public:
	Tool( const Surface& icon )
		:
		icon( icon )
	{}

	void SetCanvasRef( Canvas& canv )
	{
		this->canv = &canv;
	}

	virtual bool OnMouseDown( const Vei2& pos ) { return( false ); }
	virtual bool OnMouseUp( const Vei2& pos ) { return( false ); }
	virtual bool OnMouseMove( const Vei2& pos ) { return( false ); }

	virtual bool OnKeyDown( unsigned char key ) { return( false ); }
	virtual bool OnKeyUp( unsigned char key ) { return( false ); }

	void OnWindowResize( const RectI& area )
	{
		drawScale = float( area.GetHeight() ) / icon.GetHeight();
		this->area = area;
		this->area.right = area.left + icon.GetWidth() * int( drawScale );
	}

	virtual void OnPaint( HDC hdc )
	{
		icon.Draw( hdc,area.GetTopLeft(),drawScale );
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