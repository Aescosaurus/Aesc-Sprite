#include "stdafx.h"
#include "Editor.h"
#include "FileOpener.h"

#include "Pointer.h"
#include "Selector.h"
#include "Brush.h"
#include "Eraser.h"
#include "Hand.h"
#include "Zoomer.h"
#include "Filler.h"

Editor::Editor( const Vei2& windowSize )
	:
	canvasArea( 0,0,0,0 ),
	toolbarArea( 0,0,0,0 ),
	sidebarArea( 0,0,0,0 ),
	pal( sidebarArea ),
	layers( sidebarArea,canvSize,pal ),
	canv( canvasArea,layers.GenerateFinalImage(),pal ),
	toolbarBG( pal.GetBrush( pal.GetColor( 14 ) ) )
{
	Tool::SetCanvasRef( canv );
	Tool::SetPalRef( pal );
	Tool::CacheImage( layers.GetCurLayer() );

	tools.emplace_back( std::make_unique<Pointer>() );
	tools.emplace_back( std::make_unique<Selector>() );
	tools.emplace_back( std::make_unique<Brush>() );
	tools.emplace_back( std::make_unique<Eraser>() );
	tools.emplace_back( std::make_unique<Hand>() );
	tools.emplace_back( std::make_unique<Zoomer>() );
	tools.emplace_back( std::make_unique<Filler>() );

	HandleWindowResize( windowSize );
}

bool Editor::HandleMouseDown( const Vei2& pos )
{
	for( int i = 0; i < int( tools.size() ); ++i )
	{
		if( tools[i]->GetArea().ContainsPoint( pos ) )
		{
			curTool = i;
			return( true );
		}
	}
	auto& tool = tools[curTool];
	const auto type = tool->OnMouseDown( pos );
	return( GetReturnType( type ) );
}

bool Editor::HandleMouseUp( const Vei2& pos )
{
	auto& tool = tools[curTool];
	const auto type = tool->OnMouseUp( pos );
	return( GetReturnType( type ) );
}

bool Editor::HandleMouseMove( const Vei2& pos )
{
	auto& tool = tools[curTool];
	const auto type = tool->OnMouseMove( pos );
	return( GetReturnType( type ) );
}

bool Editor::HandleKeyDown( unsigned char key )
{
	for( int i = 0; i < int( tools.size() ); ++i )
	{
		if( key == tools[i]->GetSwapKey() )
		{
			curTool = i;
			oldTool = -1;
			return( true );
		}
		else if( key == tools[i]->GetTempSelectKey() )
		{
			oldTool = curTool;
			curTool = i;
			return( true );
		}
	}
	auto& tool = tools[curTool];
	const auto type = tool->OnKeyDown( key );
	return( GetReturnType( type ) );
}

bool Editor::HandleKeyUp( unsigned char key )
{
	if( oldTool != -1 && key == tools[curTool]->GetTempSelectKey() )
	{
		curTool = oldTool;
		oldTool = -1;
		return( true );
	}
	auto& tool = tools[curTool];
	const auto type = tool->OnKeyUp( key );
	return( GetReturnType( type ) );
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
	tools[0]->OnWindowResize( toolbarArea );
	for( int i = 1; i < int( tools.size() ); ++i )
	{
		tools[i]->OnWindowResize( tools[i - 1]->GetNextRect() );
	}
}

void Editor::HandlePaint( HDC hdc )
{
	pal.OnPaint( hdc );
	layers.OnPaint( hdc );
	canv.OnPaint( hdc );
	auto toolbarRect = RECT( toolbarArea );
	FillRect( hdc,&toolbarRect,*toolbarBG );
	for( auto& tool : tools )
	{
		tool->OnPaint( hdc );
	}
}

void Editor::OpenFile()
{
	const auto path = FileOpener::OpenFile();
	if( path.length() > 0 )
	{
		pal.GeneratePalette( path );
		layers.OpenImage( path );
		RegenImage();
	}
}

void Editor::RegenImage()
{
	canv.CacheImage( layers.GenerateFinalImage() );
	Tool::CacheImage( layers.GetCurLayer() );
}

bool Editor::GetReturnType( Tool::ReturnType type )
{
	switch( type )
	{
	case Tool::ReturnType::None:
		return( false );
		break;
	case Tool::ReturnType::Repaint:
		return( true );
		break;
	case Tool::ReturnType::RegenImage:
		RegenImage();
		return( true );
		break;
	}
	return( false );
}
