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
	FillRect( hdc,&rc,HBRUSH( CreateSolidBrush( RGB( 50,50,50 ) ) ) );
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
