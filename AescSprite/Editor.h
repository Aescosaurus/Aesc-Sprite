#pragma once

#include "Vec2.h"

class Editor
{
public:
	void HandleMouseDown( const Vei2& pos );
	void HandleMouseUp( const Vei2& pos );
	void HandleMouseMove( const Vei2& pos );

	void HandleKeyDown( unsigned char key );
	void HandleKeyUp( unsigned char key );

	void HandleWindowResize( const Vei2& windowSize );
};