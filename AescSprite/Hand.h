#pragma once

#include "Tool.h"

class Hand
	:
	public Tool
{
public:
	Hand()
		:
		Tool( "Icons/Hand.bmp",'H',"Icons/Mouse/Hand.bmp",' ' )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		Tool::OnMouseDown( pos );
		oldPos = pos;
		return( ReturnType::None );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		Tool::OnMouseUp( pos );
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
		Tool::OnMouseMove( pos );
		auto type = ReturnType::None;
		if( mouseDown )
		{
			const auto diff = pos - oldPos;
			canv->GetImagePos() += diff;
			oldPos = pos;
			type = ReturnType::Repaint;
		}
		return( type );
	}
private:
	Vei2 oldPos = Vei2::Zero();
};