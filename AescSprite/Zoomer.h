#pragma once

#include "Tool.h"

class Zoomer
	:
	public Tool
{
public:
	Zoomer()
		:
		Tool( "Icons/Zoomer.bmp",'Z',"Icons/Mouse/Zoomer.bmp" )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		Tool::OnMouseDown( pos );
		oldX = pos.x;
		return( ReturnType::None );
	}

	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		Tool::OnMouseMove( pos );
		ReturnType type = ReturnType::None;

		if( mouseDown )
		{
			const auto diff = pos.x - oldX;
			canv->GetImageScale() += float( diff ) * scalingSpeed;

			oldX = pos.x;
			type = ReturnType::Repaint;
		}

		return( type );
	}
private:
	int oldX;
	static constexpr float scalingSpeed = 0.05f;
};