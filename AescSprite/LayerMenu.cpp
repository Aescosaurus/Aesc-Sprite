#include "stdafx.h"
#include "LayerMenu.h"

LayerMenu::LayerMenu( const RectI& area,const Vei2& canvSize )
	:
	area( area )
{
	OnWindowResize( area );
	
	layers.emplace_back( Surface{ 0,0 } );
	ResizeCanvas( canvSize );
}

void LayerMenu::OnWindowResize( const RectI& area )
{
	this->area = area;
}

void LayerMenu::OnPaint( HDC hdc )
{
	const auto rc = RECT( area );
	if( bgColor == nullptr ) bgColor = HBRUSH( CreateSolidBrush( RGB( 70,70,70 ) ) );
	FillRect( hdc,&rc,bgColor );
}

void LayerMenu::ResizeCanvas( const Vei2& canvSize )
{
	for( auto& layer : layers )
	{
		layer.Resize( canvSize );
	}
}

Surface& LayerMenu::GetCurLayer()
{
	return( layers[selectedLayer] );
}

Surface LayerMenu::GenerateFinalImage() const
{
	Surface temp = Surface{ layers[0].GetWidth(),layers[0].GetHeight() };
	for( const auto& layer : layers )
	{
		temp.Overlay( layer );
	}
	return( temp );
}
