#pragma once

#include "Tool.h"

class Hand
	:
	public Tool
{
public:
	Hand()
		:
		Tool( "Icons/Hand.bmp",'H' )
	{}

	ReturnType OnMouseDown( const Vei2& pos ) override
	{
		mouseDown = true;
		oldPos = pos;
		return( ReturnType::None );
	}
	ReturnType OnMouseUp( const Vei2& pos ) override
	{
		mouseDown = false;
		return( ReturnType::None );
	}
	ReturnType OnMouseMove( const Vei2& pos ) override
	{
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
	bool mouseDown = false;
	Vei2 oldPos = Vei2::Zero();
};