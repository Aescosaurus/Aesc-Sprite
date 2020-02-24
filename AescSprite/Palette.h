#pragma once

#include "Rect.h"
#include <vector>
#include <string>
#include "Color.h"

class Palette
{
private:
	class ColorItem
	{
	public:
		ColorItem( Color c,const RectI& area );
		~ColorItem();
	public:
		Color col;
		RectI area;
		HBRUSH solidBrush;
	};
public:
	Palette( const RectI& area );

	void LoadPalette( const std::string& src );
	void OnWindowResize( const RectI& area );
	void OnPaint( HDC hdc );

	int GetBottom() const;
private:
	RectI area;
	std::vector<ColorItem> colors;
};