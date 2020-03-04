#pragma once

#include "Rect.h"
#include "Surface.h"

class Canvas
{
public:
	Canvas( const RectI& area,const Surface& img );

	void OnWindowResize( const RectI& area );
	void OnPaint( Surface& buffer );

	void CacheImage( const Surface& img );

	Vei2& GetImagePos();
	Vei2 Mouse2CanvPos( const Vei2& mousePos ) const;
private:
	RectI area;
	Surface image;
	Vei2 imagePos = Vei2::Zero();
	float imageScale = 15.0f;

	HBRUSH bgColor = nullptr;
};