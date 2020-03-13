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
	Tool( const std::string& icon,unsigned char swapKey,
		const std::string& mouseIcon = "",
		unsigned char tempSelectKey = '\0' );

	static void SetCanvasRef( Canvas& canv );
	static void SetPalRef( class Palette& pal );
	static void CacheImage( Surface& img );
	static void UpdateSelectArea();

	virtual ReturnType OnMouseDown( const Vei2& pos );
	virtual ReturnType OnMouseUp( const Vei2& pos );
	virtual ReturnType OnMouseMove( const Vei2& pos );

	virtual ReturnType OnKeyDown( unsigned char key ) { return( ReturnType::None ); }
	virtual ReturnType OnKeyUp( unsigned char key ) { return( ReturnType::None ); }

	virtual void OnWindowResize( const RectI& area );

	virtual void OnPaint( HDC hdc );
	virtual void PaintIcon( HDC hdc );

	RectI GetNextRect() const;
	unsigned char GetSwapKey() const;
	unsigned char GetTempSelectKey() const;
	const RectI& GetArea() const;
protected:
	float drawScale = 0.0f;
	Surface icon;
	Surface mouseIcon;
	unsigned char swapKey;
	unsigned char tempSelectKey;
	RectI area = RectI{ 0,0,0,0 };
	static Surface* activeLayer;
	static Canvas* canv;
	static Palette* pal;
	static RectI selectArea;
	bool mouseDown = false;
	Vei2 mousePos = Vei2::Zero();
};