#pragma once

#include "Rect.h"
#include <vector>
#include "Surface.h"
#include "Palette.h"
#include "Tool.h"

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

	Tool::ReturnType OnMouseDown( const Vei2& pos );
	Tool::ReturnType OnMouseMove( const Vei2& pos );
	Tool::ReturnType OnMouseUp( const Vei2& pos );

	void OnWindowResize( const RectI& area );
	void OnPaint( HDC hdc );
	void ResizeCanvas( const Vei2& canvSize );
	void OpenImage( const std::string& imgPath );

	Surface& GetCurLayer();
	Surface GenerateFinalImage( bool magentaBG = false ) const;
private:
	void DrawLayer( HDC hdc,int i,const HBRUSH* col ) const;
private:
	RectI area;
	std::vector<Layer> layers;
	int selectedLayer = 0;
	Vei2 dragStartPos = Vei2{ -1,-1 };
	Vei2 dragDiff = Vei2{ -1,-1 };
	
	const HBRUSH* bgColor;
	const HBRUSH* layerColor;
	const HBRUSH* selectedLayerColor;
	const Color onCol;
	const Color offCol;
};