#include "stdafx.h"
#include "Editor.h"
#include "FileOpener.h"
#include "WriteToBitmap.h"
#include <cassert>

#include "Pointer.h"
#include "Selector.h"
#include "Brush.h"
#include "Eraser.h"
#include "Hand.h"
#include "Zoomer.h"
#include "Filler.h"
#include "Sampler.h"

Editor::Editor( const Vei2& windowSize )
	:
	canvasArea( 0,0,0,0 ),
	toolbarArea( 0,0,0,0 ),
	sidebarArea( 0,0,0,0 ),
	pal( sidebarArea ),
	layers( sidebarArea,canvSize,pal ),
	canv( canvasArea,layers.GenerateFinalImage(),pal ),
	toolbarBG( pal.GetDefaultBrush( pal.GetDefaultColor( 15 ) ) )
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
	tools.emplace_back( std::make_unique<Sampler>() );

	HandleWindowResize( windowSize );
}

bool Editor::HandleMouseDown( const Vei2& pos )
{
	auto type = Tool::ReturnType::None;

	while( ShowCursor( FALSE ) > 0 );

	if( sidebarArea.ContainsPoint( pos ) )
	{
		type = layers.OnMouseDown( pos );
	}

	if( pal.GetArea().ContainsPoint( pos ) )
	{
		type = pal.OnMouseDown( pos );
	}

	if( toolbarArea.ContainsPoint( pos ) )
	{
		for( int i = 0; i < int( tools.size() ); ++i )
		{
			if( tools[i]->GetArea().ContainsPoint( pos ) )
			{
				curTool = i;
				return( true );
			}
		}
	}

	if( canvasArea.ContainsPoint( pos ) )
	{
		type = tools[curTool]->OnMouseDown( pos );
	}

	return( GetReturnType( type ) );
}

bool Editor::HandleMouseUp( const Vei2& pos )
{
	auto type = Tool::ReturnType::None;
	
	if( sidebarArea.ContainsPoint( pos ) )
	{
		type = layers.OnMouseUp( pos );
	}

	if( canvasArea.ContainsPoint( pos ) )
	{
		type = tools[curTool]->OnMouseUp( pos );
	}

	return( GetReturnType( type ) );
}

bool Editor::HandleMouseMove( const Vei2& pos )
{
	auto type = Tool::ReturnType::None;

	if( sidebarArea.ContainsPoint( pos ) )
	{
		type = layers.OnMouseMove( pos );
	}

	// if( canvasArea.ContainsPoint( pos ) )
	{
		type = tools[curTool]->OnMouseMove( pos );
	}

	if( type == Tool::ReturnType::None ) type = Tool::ReturnType::Repaint;
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
	static constexpr float toolbarHeight = 0.1f;
	const int padding = int( float( windowSize.x ) * 0.003f );

	toolbarArea = RectI{ 0,windowSize.x,
		0,int( wndSize.y * toolbarHeight ) }.GetExpanded( -padding );
	canvasArea = RectI{ 0,int( wndSize.x * canvasWidth ),
		toolbarArea.bottom,windowSize.y }.GetExpanded( -padding );
	sidebarArea = RectI{ canvasArea.right,windowSize.x,
		toolbarArea.bottom,int( windowSize.y ) }.GetExpanded( -padding );

	pal.OnWindowResize( sidebarArea );
	sidebarArea.top = pal.GetBottom();
	layers.OnWindowResize( sidebarArea );
	canv.OnWindowResize( canvasArea );
	tools[0]->OnWindowResize( toolbarArea );
	for( int i = 1; i < int( tools.size() ); ++i )
	{
		tools[i]->OnWindowResize( tools[i - 1]->GetNextRect() );
	}
	toolbarArea = toolbarArea.GetExpandedX( padding );
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
	tools[curTool]->PaintIcon( hdc );
}

int Editor::TryOpenFile()
{
	const auto path = FileOpener::OpenFile();
	fileOpenPath = path;
	if( path.length() > 0 )
	{
		const auto surf = Surface{ path };
		const auto uniqueCols = surf.FindUniqueColors();
		bool forcePalUpdate = false;
		for( auto& col : uniqueCols )
		{
			if( pal.GetBrush( col ) == nullptr )
			{
				forcePalUpdate = true;
			}
		}

		if( forcePalUpdate )
		{
			bool canReplace = true;
			for( int i = 0; i < layers.GetLayerCount(); ++i )
			{
				const auto layer = layers.GetLayer( i );
				for( auto col : layer.FindUniqueColors() )
				{
					if( std::find( uniqueCols.begin(),
						uniqueCols.end(),col ) == uniqueCols.end() )
					{
						canReplace = false;
					}
				}
			}
			if( canReplace ) return( 1 );
			else return( 2 );
		}
		else
		{
			return( 0 );
		}
	}
	return( -1 );
}

void Editor::OpenFile( int action )
{
	if( fileOpenPath.length() > 0 )
	{
		switch( action )
		{
		case 0:
			break;
		case 1:
			pal.GeneratePalette( fileOpenPath,false );
			break;
		case 2:
			pal.GeneratePalette( fileOpenPath,true );
			break;
		default:
			assert( false );
			break;
		}
		ResizeImage( Surface{ fileOpenPath }.GetSize() );
		layers.OpenImage( fileOpenPath );
		RegenImage();
		fileOpenPath = "";
	}
}

void Editor::SaveFile()
{
	if( savedFilePath.length() > 0 )
	{
		WriteToBitmap::Write( layers.GenerateFinalImage(),
			savedFilePath );
	}
	else
	{
		SaveFileAs();
	}
}

void Editor::SaveFileAs()
{
	savedFilePath = FileOpener::SaveFile();
	if( savedFilePath.length() > 0 )
	{
		WriteToBitmap::Write( layers.GenerateFinalImage(),
			savedFilePath );
	}
}

void Editor::RegenImage()
{
	canv.CacheImage( layers.GenerateFinalImage() );
	Tool::CacheImage( layers.GetCurLayer() );
}

void Editor::ResizeImage( const Vei2& size )
{
	canvSize = size;
	layers.ResizeCanvas( size );
	Tool::UpdateSelectArea();
	RegenImage();
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

const Vei2& Editor::GetCanvSize() const
{
	return( canvSize );
}
