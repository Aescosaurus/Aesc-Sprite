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
	Tool( const std::string& icon,unsigned char swapKey );

	static void SetCanvasRef( Canvas& canv );
	static void SetPalRef( Palette& pal );
	static void CacheImage( Surface& img );

	virtual ReturnType OnMouseDown( const Vei2& pos ) { return( ReturnType::None ); }
	virtual ReturnType OnMouseUp( const Vei2& pos ) { return( ReturnType::None ); }
	virtual ReturnType OnMouseMove( const Vei2& pos ) { return( ReturnType::None ); }

	virtual ReturnType OnKeyDown( unsigned char key ) { return( ReturnType::None ); }
	virtual ReturnType OnKeyUp( unsigned char key ) { return( ReturnType::None ); }

	void OnWindowResize( const RectI& area );

	virtual void OnPaint( HDC hdc );

	RectI GetNextRect() const;
	unsigned char GetSwapKey() const;
protected:
	float drawScale = 0.0f;
	Surface icon;
	unsigned char swapKey;
	RectI area = RectI{ 0,0,0,0 };
	static Surface* activeLayer;
	static Canvas* canv;
	static Palette* pal;
	static RectI selectArea;
};