#include "stdafx.h"
#include "Editor.h"
#include "FileOpener.h"

Editor::Editor( const Vei2& windowSize )
	:
	canvasArea( 0,0,0,0 ),
	toolbarArea( 0,0,0,0 ),
	sidebarArea( 0,0,0,0 ),
	pal( sidebarArea ),
	layers( sidebarArea,canvSize ),
	canv( canvasArea,layers.GenerateFinalImage() )
{
	pal.LoadPalette( Surface{ "Palettes/Gr8.bmp" } );
	HandleWindowResize( windowSize );
}

bool Editor::HandleMouseDown( const Vei2& pos )
{
	// update cur tool
	// if tool returns true we return true
	return false;
}

bool Editor::HandleMouseUp( const Vei2& pos )
{
	return false;
}

bool Editor::HandleMouseMove( const Vei2& pos )
{
	return false;
}

bool Editor::HandleKeyDown( unsigned char key )
{
	return false;
}

bool Editor::HandleKeyUp( unsigned char key )
{
	return false;
}

void Editor::HandleWindowResize( const Vei2& windowSize )
{
	const auto wndSize = Vec2( windowSize );
	static constexpr float canvasWidth = 0.85f;
	static constexpr float toolbarHeight = 0.08f;

	// TODO: Padding.
	toolbarArea = RectI{ 0,windowSize.x,
		0,int( wndSize.y * toolbarHeight ) };
	canvasArea = RectI{ 0,int( wndSize.x * canvasWidth ),
		toolbarArea.bottom,windowSize.y };
	sidebarArea = RectI{ canvasArea.right,windowSize.x,
		toolbarArea.bottom,int( windowSize.y ) };

	pal.OnWindowResize( sidebarArea );
	sidebarArea.top = pal.GetBottom();
	layers.OnWindowResize( sidebarArea );
	canv.OnWindowResize( canvasArea );
	tools[0].OnWindowResize( toolbarArea );
	for( int i = 1; i < int( tools.size() ); ++i )
	{
		tools[i].OnWindowResize( tools[i - 1].GetNextRect() );
	}
}

void Editor::HandlePaint( HDC hdc )
{
	pal.OnPaint( hdc );
	layers.OnPaint( hdc );
	canv.OnPaint( hdc );
	for( auto& tool : tools )
	{
		tool.OnPaint( hdc );
	}
}

void Editor::OpenFile()
{
	const auto path = FileOpener::OpenFile();
	if( path.length() > 0 )
	{
		pal.GeneratePalette( path );
		layers.OpenImage( path );
		canv.CacheImage( layers.GenerateFinalImage() );
		auto& curLayer = layers.GetCurLayer();
		for( auto& tool : tools )
		{
			tool.CacheImage( curLayer );
		}
	}
}
