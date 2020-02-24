#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Palette.h"
#include "LayerMenu.h"

class Editor
{
public:
	Editor( const Vei2& windowSize );

	bool HandleMouseDown( const Vei2& pos );
	bool HandleMouseUp( const Vei2& pos );
	bool HandleMouseMove( const Vei2& pos );

	bool HandleKeyDown( unsigned char key );
	bool HandleKeyUp( unsigned char key );

	void HandleWindowResize( const Vei2& windowSize );

	void HandlePaint( HDC hdc );
private:
	RectI canvasArea;
	RectI toolbarArea;
	RectI sidebarArea;

	Vei2 canvSize = { 8,8 };

	Palette pal;
	LayerMenu layers;
};