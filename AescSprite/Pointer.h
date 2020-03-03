#pragma once

#include "Tool.h"

class Pointer
	:
	public Tool
{
public:
	Pointer()
		:
		Tool( Surface{ "Icons/Pointer.bmp" } )
	{}

	bool OnMouseDown( const Vei2& pos ) override
	{
		mouseDown = true;
		return( false );
	}
	bool OnMouseUp( const Vei2& pos ) override
	{
		mouseDown = false;
		oldPos = Vei2{ -1,-1 };
		return( false );
	}
	bool OnMouseMove( const Vei2& pos ) override
	{
		bool repaint = false;
		if( mouseDown )
		{
			if( oldPos != Vei2{ -1,-1 } )
			{
				const auto diff = pos - oldPos;
				// canv->GetImagePos() += diff;
				activeLayer->Move( activeLayer->GetRect(),
					diff.GetOneized() );
				repaint = true;
			}
			oldPos = pos;
		}
		return( repaint );
	}
private:
	bool mouseDown = false;
	Vei2 oldPos = Vei2{ -1,-1 };
};