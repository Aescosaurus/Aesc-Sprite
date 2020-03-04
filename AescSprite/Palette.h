#pragma once

#include "Rect.h"
#include <vector>
#include <string>
#include "Color.h"
#include "Surface.h"

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

	void LoadPalette( const Surface& pal );
	void GeneratePalette( const std::string& src );
	void OnWindowResize( const RectI& area );
	void OnPaint( Surface& buffer );

	int GetBottom() const;

	const HBRUSH* GetBrush( Color c ) const;
private:
	RectI area;
	std::vector<ColorItem> colors;
};