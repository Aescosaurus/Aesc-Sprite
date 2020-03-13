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
	Vei2 oldPos = Vei2::Zero();
};