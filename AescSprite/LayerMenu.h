#pragma once

#include "Rect.h"
#include <vector>
#include "Surface.h"
#include "Palette.h"

class LayerMenu
{
private:
	class Layer
	{
	public:
		Layer( const Surface& surf,const RectI& area )
			:
			surf( surf ),
			area( area )
		{}
	public:
		Surface surf;
		RectI area;
	};
public:
	LayerMenu( const RectI& area,const Vei2& canvSize,
		const Palette& pal );

	void OnWindowResize( const RectI& area );
	void OnPaint( HDC hdc );
	void ResizeCanvas( const Vei2& canvSize );
	void OpenImage( const std::string& imgPath );

	Surface& GetCurLayer();
	Surface GenerateFinalImage() const;
private:
	RectI area;
	std::vector<Layer> layers;
	int selectedLayer = 0;
	
	const HBRUSH* bgColor;
	const HBRUSH* layerColor;
};