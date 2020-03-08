#pragma once

#include "Tool.h"

class Pointer
	:
	public Tool
{
public:
	Pointer()
		:
		Tool( Surface{ "Icons/Pointer.bmp" },'V' )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		mouseDown = true;
		return( ReturnType::None );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		mouseDown = false;
		oldPos = Vei2{ -1,-1 };
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		ReturnType type = ReturnType::None;
		if( mouseDown )
		{
			if( oldPos != Vei2{ -1,-1 } )
			{
				const auto diff = pos - oldPos;
				// canv->GetImagePos() += diff;
				activeLayer->Move( activeLayer->GetRect(),
					diff.GetOneized() );
				type = ReturnType::Repaint;
			}
			oldPos = pos;
		}
		return( type );
	}
private:
	bool mouseDown = false;
	Vei2 oldPos = Vei2{ -1,-1 };
};