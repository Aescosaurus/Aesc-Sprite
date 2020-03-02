#pragma once

#include "Rect.h"
#include <vector>
#include "Surface.h"

class LayerMenu
{
public:
	LayerMenu( const RectI& area,const Vei2& canvSize );

	void OnWindowResize( const RectI& area );
	void OnPaint( HDC hdc );
	void ResizeCanvas( const Vei2& canvSize );
	void OpenImage( const std::string& imgPath );

	Surface& GetCurLayer();
	Surface GenerateFinalImage() const;
private:
	RectI area;
	std::vector<Surface> layers;
	int selectedLayer = 0;
	
	HBRUSH bgColor = nullptr;
};