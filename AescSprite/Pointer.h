#pragma once

#include "Tool.h"

class Pointer
	:
	public Tool
{
public:
	Pointer()
		:
		Tool( "Icons/Pointer.bmp",'V',"Icons/Mouse/Pointer.bmp" )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		Tool::OnMouseDown( pos );
		oldPos = canv->Mouse2CanvPos( pos );
		return( ReturnType::None );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		Tool::OnMouseUp( pos );
		oldPos = Vei2{ -1,-1 };
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		Tool::OnMouseMove( pos );
		ReturnType type = ReturnType::None;
		if( mouseDown )
		{
			if( oldPos != Vei2{ -1,-1 } )
			{
				const auto canvPos = canv->Mouse2CanvPos( pos );
				const auto diff = canvPos - oldPos;
				const auto move = diff.GetOneized();
				if( selectArea.GetMovedBy( move )
					.IsContainedBy( activeLayer->GetRect() ) )
				{
					activeLayer->Move( selectArea,move );
					selectArea.MoveBy( move );
					type = ReturnType::RegenImage;
				}
			}
			oldPos = canv->Mouse2CanvPos( pos );
		}
		return( type );
	}
private:
	Vei2 oldPos = Vei2{ -1,-1 };
};