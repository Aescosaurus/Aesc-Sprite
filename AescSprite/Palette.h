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
	void LoadDefaultPalette( const Surface& pal );
	void GeneratePalette( const std::string& src );
	void OnWindowResize( const RectI& area );
	void OnPaint( HDC hdc );

	int GetBottom() const;

	const HBRUSH* GetBrush( Color c ) const;
	const HBRUSH* GetDefaultBrush( Color c ) const;
	Color GetColor( int i ) const;
private:
	RectI area;
	std::vector<ColorItem> colors;
	std::vector<ColorItem> defaultColors;
};