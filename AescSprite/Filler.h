#pragma once

#include "Tool.h"

class Filler
	:
	public Tool
{
public:
	Filler()
		:
		Tool( "Icons/Filler.bmp",'G',"Icons/Mouse/Filler.bmp" )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		ReturnType type = ReturnType::None;
		if( !mouseDown )
		{
			const auto start = canv->Mouse2CanvPos( pos );
			const auto baseCol = activeLayer->GetPixel( start.x,start.y );
			const auto targetCol = pal->GetSelectedColor();
			if( baseCol != targetCol )
			{
				TryFillPlusAt( *activeLayer,start,
					baseCol,targetCol );
				type = ReturnType::RegenImage;
			}
		}
		Tool::OnMouseDown( pos );

		return( type );
	}
private:
	void TryFillPlusAt( Surface& surf,const Vei2& start,
		Color baseColor,Color fillColor )
	{
		if( start.x >= 0 && start.x < surf.GetWidth() &&
			start.y >= 0 && start.y < surf.GetHeight() &&
			surf.GetPixel( start.x,start.y ) == baseColor &&
			selectArea.GetExpandedTL( 1 ).ContainsPoint( start ) )
		{
			surf.PutPixel( start.x,start.y,fillColor );

			Vei2 rot = Vei2::Left();
			Vei2 testPos;
			for( int i = 0; i < 4; ++i )
			{
				testPos = start + rot.Rotate();
				TryFillPlusAt( surf,testPos,baseColor,fillColor );
			}
		}
	}
};