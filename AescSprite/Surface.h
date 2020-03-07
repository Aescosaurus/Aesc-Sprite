#pragma once

#include "Color.h"
#include <string>
#include "Rect.h"
#include <vector>
#include <unordered_map>

class Surface
{
public:
	// Create blank surface with width and height.
	Surface( int width,int height );
	// Load a bitmap(.bmp) file with filename into surface.
	Surface( const std::string& filename );
public:
	Surface( const Surface& ) = default;
	Surface& operator=( const Surface& ) = default;

	Surface( Surface&& donor );
	Surface& operator=( Surface&& rhs );

	void PutPixel( int x,int y,Color c );
	void Resize( const Vei2& newSize );
	// Overlay the other surface on top of this one.
	void Overlay( const Surface& other,const Vei2& pos = Vei2::Zero() );
	void Fill( Color fill );
	void DrawRect( int x,int y,int width,int height,Color c );
	void DrawRect( const RectI& area,Color c );
	// Move selection of surface in direction of movement.
	void Move( const RectI& selection,const Vei2& movement );

	void Draw( HDC hdc,const Vei2& pos,float scale ) const;
	void DrawDefault( HDC hdc,const Vei2& pos,float scale ) const;
	static void CacheBrushes( const Surface& test,const class Palette& pal );
	static void CacheDefaultBrushes( const Surface& colors,const class Palette& pal );

	Color GetPixel( int x,int y ) const;
	int GetWidth() const;
	int GetHeight() const;
	Vei2 GetSize() const;
	RectI GetRect() const;
private:
	static std::unordered_map<unsigned int,const HBRUSH*>& GetColorPal();
	static std::unordered_map<unsigned int,const HBRUSH*>& GetDefaultColorPal();
private:
	std::vector<Color> pixels;
	int width;
	int height;
};