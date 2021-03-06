#include "stdafx.h"
#include "LayerMenu.h"
#include <cassert>

LayerMenu::LayerMenu( const RectI& area,const Vei2& canvSize,
	const Palette& pal )
	:
	area( area ),
	bgColor( pal.GetDefaultBrush( pal.GetDefaultColor( 15 ) ) ),
	layerColor( pal.GetDefaultBrush( pal.GetDefaultColor( 15 ) ) ),
	selectedLayerColor( pal.GetDefaultBrush( pal.GetDefaultColor( 14 ) ) )
{
	OnWindowResize( area );
	
	layers.emplace_back( Layer{ Surface{ canvSize.x,canvSize.y },
		RectI{ 0,0,0,0 } } );
	layers.back().surf.Fill( Colors::Magenta );
	ResizeCanvas( layers.back().surf.GetSize() );
}

Tool::ReturnType LayerMenu::OnMouseDown( const Vei2& pos )
{
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( layers[i].area.ContainsPoint( pos ) )
		{
			selectedLayer = i;
			dragStartPos = layers[i].area.GetTopLeft();
			dragDiff = dragStartPos - pos;
		}
	}
	return( Tool::ReturnType::None );
}

Tool::ReturnType LayerMenu::OnMouseMove( const Vei2& pos )
{
	if( dragStartPos != Vei2{ -1,-1 } )
	{
		layers[selectedLayer].area.MoveTo( pos + dragDiff );
		return( Tool::ReturnType::Repaint );
	}
	return( Tool::ReturnType::None );
}

Tool::ReturnType LayerMenu::OnMouseUp( const Vei2& pos )
{
	bool swapped = false;
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( i != selectedLayer &&
			layers[i].area.ContainsPoint( pos ) )
		{
			std::swap( layers[i],layers[selectedLayer] );
			swapped = true;
		}
	}
	if( !swapped )
	{
		layers[selectedLayer].area.MoveTo( dragStartPos );
	}
	dragStartPos = Vei2{ -1,-1 };
	return( Tool::ReturnType::RegenImage );
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

		const auto tempLayer = layers[selectedLayer];

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

		if( dragStartPos != Vei2{ -1,-1 } ) layers[selectedLayer] = tempLayer;
	}
}

void LayerMenu::OnPaint( HDC hdc )
{
	const auto rc = RECT( area );
	// if( bgColor == nullptr ) bgColor = HBRUSH( CreateSolidBrush( RGB( 70,70,70 ) ) );
	// buffer.DrawRect( area,Color{ 70,70,70 } );
	FillRect( hdc,&rc,*bgColor );
	
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( i != selectedLayer )
		{
			DrawLayer( hdc,i,layerColor );
		}
	}
	DrawLayer( hdc,selectedLayer,selectedLayerColor );
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
	// layers.emplace_back( Layer{ loadedImage,RectI{ 0,0,0,0 } } );
	layers.emplace( layers.begin(),Layer{ loadedImage,RectI{ 0,0,0,0 } } );
	// ++selectedLayer;
	OnWindowResize( area );
}

Surface& LayerMenu::GetCurLayer()
{
	return( layers[selectedLayer].surf );
}

Surface LayerMenu::GenerateFinalImage() const
{
	Surface temp = Surface{ layers[0].surf.GetWidth(),layers[0].surf.GetHeight() };

	temp.Fill( Colors::Magenta );

	for( auto l = layers.rbegin(); l != layers.rend(); ++l )
	{
		temp.Overlay( l->surf );
	}
	return( temp );
}

int LayerMenu::GetLayerCount() const
{
	return( int( layers.size() ) );
}

const Surface& LayerMenu::GetLayer( int i ) const
{
	assert( i >= 0 );
	assert( i < int( layers.size() ) );
	
	return( layers[i].surf );
}

void LayerMenu::DrawLayer( HDC hdc,int i,const HBRUSH* col ) const
{
	const auto& lay = layers[i];

	const auto rc = RECT( lay.area );
	FillRect( hdc,&rc,*col );

	lay.surf.Draw( hdc,lay.area.GetTopLeft(),
		float( lay.area.GetHeight() ) / lay.surf.GetHeight() );
}
