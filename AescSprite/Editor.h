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

	// Check open reset/append dialog; -1=cancel 0=any 1=app/rep 2=app.
	int TryOpenFile();
	// Action: 0=no change, 1=replace pal, 2=append pal.
	void OpenFile( int action );
	void SaveFile();
	void SaveFileAs();
	void RegenImage();
	void ResizeImage( const Vei2& size );

	bool GetReturnType( Tool::ReturnType type );
	const Vei2& GetCanvSize() const;
private:
	RectI canvasArea;
	RectI toolbarArea;
	RectI sidebarArea;

	Vei2 canvSize = { 8,8 };

	Palette pal;
	LayerMenu layers;
	Canvas canv;
	std::vector<std::unique_ptr<Tool>> tools;
	int curTool = 0;
	int oldTool = -1;

	const HBRUSH* toolbarBG;

	std::string fileOpenPath = "";
	std::string savedFilePath = "";
};