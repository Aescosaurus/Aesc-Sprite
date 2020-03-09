#pragma once

#include "Tool.h"

class Brush
	:
	public Tool
{
public:
	Brush()
		:
		Tool( "Icons/Brush.bmp",'B' )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		mouseDown = true;
		PutPixel( pos );
		return( ReturnType::RegenImage );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		mouseDown = false;
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		ReturnType type = ReturnType::None;
		if( mouseDown && oldPos != pos )
		{
			oldPos = pos;
			PutPixel( pos );
			type = ReturnType::RegenImage;
		}
		return( type );
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
	bool mouseDown = false;
	Vei2 oldPos = Vei2::Zero();
};