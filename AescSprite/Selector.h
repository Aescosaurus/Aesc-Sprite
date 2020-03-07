#pragma once

#include "Tool.h"

class Selector
	:
	public Tool
{
public:
	Selector()
		:
		Tool( Surface{ "Icons/Pointer.bmp" } )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		mouseDown = true;
		return( ReturnType::None );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		mouseDown = false;
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
				activeLayer->PutPixel( canvPos.x,canvPos.y,
					pal->GetColor( 0 ) );
				type = ReturnType::RegenImage;
			}
		}

		return( type );
	}
private:
	bool mouseDown = false;
};