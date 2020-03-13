#pragma once

#include "Tool.h"
#include <WinUser.h>

class Sampler
	:
	public Tool
{
public:
	Sampler()
		:
		Tool( "Icons/Sampler.bmp",'I',"Icons/Mouse/Sampler.bmp",VK_MENU )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		const auto canvPos = canv->Mouse2CanvPos( pos );
		pal->SelectColor( activeLayer->GetPixel( canvPos.x,canvPos.y ) );
		return( ReturnType::Repaint );
	}
};