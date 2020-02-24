#pragma once

#include "Rect.h"
#include "Surface.h"

class Canvas
{
public:
	Canvas( const RectI& area,const Surface& img );

	void OnWindowResize( const RectI& area );
	void OnPaint( HDC hdc );

	void CacheImage( const Surface& img );
private:
	RectI area;
	Surface image;
	Vei2 imagePos = Vei2::Zero();
	float imageScale = 1.0f;

	HBRUSH bgColor = nullptr;
};