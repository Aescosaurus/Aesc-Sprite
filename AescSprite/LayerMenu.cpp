#include "stdafx.h"
#include "LayerMenu.h"

LayerMenu::LayerMenu( const RectI& area,const Vei2& canvSize,
	const Palette& pal )
	:
	area( area ),
	bgColor( pal.GetBrush( pal.GetColor( 8 ) ) ),
	layerColor( pal.GetBrush( pal.GetColor( 15 ) ) )
{
	OnWindowResize( area );
	
	layers.emplace_back( Layer{ Surface{ "Icons/Pointer.bmp" },
		RectI{ 0,0,0,0 } } );
	ResizeCanvas( layers.back().surf.GetSize() );
}

void LayerMenu::OnWindowResize( const RectI& area )
{
	this->area = area;

	if( area.GetHeight() != 0 )
	{
		const int layerPadding = int( float( area.GetHeight() ) * 0.005f );
		const int maxLayerHeight = int( float( area.GetHeight() ) * 0.1f );
		const int layerHeight = min( maxLayerHeight,
			area.GetHeight() / int( layers.size() ) );

		layers[0].area = area;
		layers[0].area.bottom = area.top + layerHeight;
		for( int i = 1; i < int( layers.size() ); ++i )
		{
			auto& lArea = layers[i].area;
			lArea = area;
			lArea.top = layers[i - 1].area.bottom;
			lArea.bottom = lArea.top + layerHeight;
		}

		for( auto& lay : layers )
		{
			lay.area = lay.area.GetExpandedY( -layerPadding );
		}
	}
}

void LayerMenu::OnPaint( HDC hdc )
{
	const auto rc = RECT( area );
	// if( bgColor == nullptr ) bgColor = HBRUSH( CreateSolidBrush( RGB( 70,70,70 ) ) );
	// buffer.DrawRect( area,Color{ 70,70,70 } );
	FillRect( hdc,&rc,*bgColor );

	for( auto& lay : layers )
	{
		const auto rc = RECT( lay.area );
		FillRect( hdc,&rc,*layerColor );
	}
}

void LayerMenu::ResizeCanvas( const Vei2& canvSize )
{
	for( auto& layer : layers )
	{
		layer.surf.Resize( canvSize );
	}
}

void LayerMenu::OpenImage( const std::string& imgPath )
{
	const auto loadedImage = Surface{ imgPath };
	ResizeCanvas( loadedImage.GetSize() );
	layers.emplace_back( Layer{ loadedImage,RectI{ 0,0,0,0 } } );
	++selectedLayer;
	OnWindowResize( area );
}

Surface& LayerMenu::GetCurLayer()
{
	return( layers[selectedLayer].surf );
}

Surface LayerMenu::GenerateFinalImage() const
{
	Surface temp = Surface{ layers[0].surf.GetWidth(),layers[0].surf.GetHeight() };
	for( const auto& layer : layers )
	{
		temp.Overlay( layer.surf );
	}
	return( temp );
}
