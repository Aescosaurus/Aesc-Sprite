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

	virtual bool OnMouseDown( const Vei2& pos ) {}
	virtual bool OnMouseUp( const Vei2& pos ) {}
	virtual bool OnMouseMove( const Vei2& pos ) {}

	virtual bool OnKeyDown( unsigned char key ) {}
	virtual bool OnKeyUp( unsigned char key ) {}

	void OnWindowResize( const RectI& area )
	{
		drawScale = area.GetHeight();
		this->area = area;
		this->area.right = area.left + icon.GetWidth() * drawScale;
	}

	virtual bool OnPaint( HDC hdc )
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
			icon.GetWidth() * drawScale ) );
	}
private:
	float drawScale = 0.0f;
	Surface icon;
	RectI area = RectI{ 0,0,0,0 };
	Surface* activeLayer;
};