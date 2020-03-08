#pragma once

#include "Rect.h"
#include "Surface.h"
#include "Palette.h"

class Canvas
{
public:
	Canvas( const RectI& area,const Surface& img,
		const Palette& pal );

	void OnWindowResize( const RectI& area );
	void OnPaint( HDC hdc );

	void CacheImage( const Surface& img );

	Vei2& GetImagePos();
	Vei2 Mouse2CanvPos( const Vei2& mousePos ) const;
	Vei2 CalcImagePos() const;
	float GetImageScaling();
private:
	RectI area;
	Surface image;
	Vei2 imagePos = Vei2::Zero();
	float imageScale = 15.0f;

	const HBRUSH* bgColor;
};