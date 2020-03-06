#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Palette.h"
#include "LayerMenu.h"
#include "Canvas.h"
#include <string>
#include "Tool.h"
#include <memory>

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

	void OpenFile();
	void RegenImage();
	bool GetReturnType( Tool::ReturnType type );
private:
	RectI canvasArea;
	RectI toolbarArea;
	RectI sidebarArea;

	Vei2 canvSize = { 8,8 };

	Palette pal;
	LayerMenu layers;
	Canvas canv;
	std::vector<std::unique_ptr<Tool>> tools;
	int curTool = 1;
};