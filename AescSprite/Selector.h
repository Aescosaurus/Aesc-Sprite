#pragma once

#include "Tool.h"

class Selector
	:
	public Tool
{
public:
	Selector()
		:
		Tool( "Icons/Selector.bmp",'M',"Icons/Mouse/Selector.bmp" )
	{
		selectArea = activeLayer->GetRect();
	}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		if( !mouseDown )
		{
			Tool::OnMouseDown( pos );
			selectArea = RectI{ 0,0,0,0 };
			const auto canvPos = canv->Mouse2CanvPos( pos );
			selectArea.MoveTo( canvPos );
		}
		return( ReturnType::None );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		Tool::OnMouseUp( pos );
		selectArea.Fix();
		// selection = RectI{ 0,0,0,0 };
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		Tool::OnMouseMove( pos );
		ReturnType type = ReturnType::None;
		if( mouseDown )
		{
			const auto canvPos = canv->Mouse2CanvPos( pos );
			if( activeLayer->GetRect().ContainsPoint( canvPos ) )
			{
				selectArea.right = canvPos.x + 1;
				selectArea.bottom = canvPos.y + 1;
				type = ReturnType::Repaint;
			}
		}

		return( type );
	}
	void OnPaint( HDC hdc ) override
	{
		Tool::OnPaint( hdc );

		if( selectArea.GetWidth() != 0 &&
			selectArea.GetHeight() != 0 )
		{
			const auto scale = canv->GetImageScaling();
			auto rsel = Rect( selectArea );
			rsel.Scale( scale );
			rsel.MoveBy( canv->CalcImagePos() );
			const auto sel = RectI( rsel );
			sel.Draw( hdc,pal->GetDefaultColor( 11 ) );
		}
	}
private:
	// RectI selection = RectI{ 0,0,0,0 };
};