#pragma once

#include "Tool.h"

class Filler
	:
	public Tool
{
public:
	Filler()
		:
		Tool( "Icons/Filler.bmp",'G' )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		ReturnType type = ReturnType::None;
		if( !mouseDown )
		{
			const auto start = canv->Mouse2CanvPos( pos );
			TryFillPlusAt( *activeLayer,start,
				activeLayer->GetPixel( start.x,start.y ),
				pal->GetSelectedColor() );
			type = ReturnType::RegenImage;
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