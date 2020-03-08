#pragma once

#include "Tool.h"

class Selector
	:
	public Tool
{
public:
	Selector()
		:
		Tool( Surface{ "Icons/Selector.bmp" },'M' )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		if( !mouseDown )
		{
			selection = RectI{ 0,0,0,0 };
			mouseDown = true;
			const auto canvPos = canv->Mouse2CanvPos( pos );
			selection.MoveTo( canvPos );
		}
		return( ReturnType::None );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		mouseDown = false;
		selection.Fix();
		// selection = RectI{ 0,0,0,0 };
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		ReturnType type = ReturnType::None;
		if( mouseDown )
		{
			const auto canvPos = canv->Mouse2CanvPos( pos );
			if( activeLayer->GetRect().ContainsPoint( canvPos ) )
			{
				selection.right = canvPos.x + 1;
				selection.bottom = canvPos.y + 1;
				type = ReturnType::Repaint;
				// activeLayer->PutPixel( canvPos.x,canvPos.y,
				// 	pal->GetColor( 0 ) );
				// type = ReturnType::RegenImage;
			}
		}

		return( type );
	}
	void OnPaint( HDC hdc ) override
	{
		Tool::OnPaint( hdc );

		if( mouseDown && selection.GetWidth() != 0 &&
			selection.GetHeight() != 0 )
		{
			HPEN pen = CreatePen( PS_SOLID,1,
				pal->GetColor( 11 ) );
			SelectObject( hdc,pen );
			const auto scale = int( canv->GetImageScaling() );
			auto sel = selection;
			sel.Scale( scale );
			sel.MoveBy( canv->CalcImagePos() );
			MoveToEx( hdc,sel.left,sel.top,nullptr );
			LineTo( hdc,sel.right,sel.top );
			LineTo( hdc,sel.right,sel.bottom );
			LineTo( hdc,sel.left,sel.bottom );
			LineTo( hdc,sel.left,sel.top );
		}
	}
private:
	bool mouseDown = false;
	RectI selection = RectI{ 0,0,0,0 };
};