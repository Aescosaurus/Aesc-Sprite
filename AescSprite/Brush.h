#pragma once

#include "Tool.h"

class Brush
	:
	public Tool
{
public:
	Brush()
		:
		Tool( "Icons/Brush.bmp",'B',"" )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		Tool::OnMouseDown( pos );
		PutPixel( pos );
		cursorRect.MoveTo( pos - cursorRect.GetSize() );
		return( ReturnType::RegenImage );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		Tool::OnMouseUp( pos );
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		Tool::OnMouseMove( pos );
		ReturnType type = ReturnType::None;
		if( mouseDown && oldPos != pos )
		{
			oldPos = pos;
			PutPixel( pos );
			type = ReturnType::RegenImage;
		}
		
		cursorRect.MoveTo( pos - cursorRect.GetSize() );

		return( type );
	}
	void OnWindowResize( const RectI& area ) override
	{
		Tool::OnWindowResize( area );
		
		// cursorRect = RectI{ 0,int( canv->GetImageScaling() ),
		// 	0,int( canv->GetImageScaling() ) };
		cursorRect.Resize( Vei2::One() * int( canv->GetImageScaling() ) );
		cursorRect.MoveBy( cursorRect.GetSize() / 2 );
	}
	void PaintIcon( HDC hdc ) override
	{
		cursorRect.Draw( hdc,cursorCol );

		const int plusSize = cursorRect.GetWidth() / 4;
		const auto center = cursorRect.GetCenter();
		HPEN pen = CreatePen( PS_SOLID,1,cursorCol );
		SelectObject( hdc,pen );
		MoveToEx( hdc,center.x,cursorRect.top + plusSize,nullptr );
		LineTo( hdc,center.x,cursorRect.bottom - plusSize );
		MoveToEx( hdc,cursorRect.left + plusSize,center.y,nullptr );
		LineTo( hdc,cursorRect.right - plusSize,center.y );
		DeleteObject( pen );
	}
private:
	void PutPixel( const Vei2& pos )
	{
		const auto canvPos = canv->Mouse2CanvPos( pos );
		if( selectArea.GetExpandedTL( 1 ).ContainsPoint( canvPos ) )
		{
			activeLayer->PutPixel( canvPos.x,canvPos.y,
				pal->GetSelectedColor() );
		}
	}
private:
	Vei2 oldPos = Vei2::Zero();
	RectI cursorRect = RectI{ 0,0,0,0 };
	Color cursorCol = pal->GetColor( 12 );
};