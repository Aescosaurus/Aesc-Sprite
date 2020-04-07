// AescSprite.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AescSprite.h"
#include "Vec2.h"
#include "Editor.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass( HINSTANCE hInstance );
BOOL                InitInstance( HINSTANCE,int );
LRESULT CALLBACK    WndProc( HWND,UINT,WPARAM,LPARAM );
INT_PTR CALLBACK    About( HWND,UINT,WPARAM,LPARAM );
INT_PTR CALLBACK    Resize( HWND,UINT,WPARAM,LPARAM );
INT_PTR CALLBACK    AskPal( HWND,UINT,WPARAM,LPARAM );

static constexpr Vei2 initWindowSize = { 960,540 };
Editor editor{ initWindowSize };

void Repaint( HWND hWnd )
{
	RECT clientRect;
	if( GetClientRect( hWnd,&clientRect ) )
	{
		InvalidateRect( hWnd,&clientRect,FALSE );
	}
}

void ResizeWnd( HWND hWnd )
{
	RECT clientRect;
	if( GetClientRect( hWnd,&clientRect ) )
	{
		editor.HandleWindowResize( Vei2{
			clientRect.right - clientRect.left,
			clientRect.bottom - clientRect.top } );
	}
}

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW( hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING );
	LoadStringW( hInstance,IDC_AESCSPRITE,szWindowClass,MAX_LOADSTRING );
	MyRegisterClass( hInstance );

	// Perform application initialization:
	if( !InitInstance( hInstance,nCmdShow ) )
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators( hInstance,MAKEINTRESOURCE( IDC_AESCSPRITE ) );

	MSG msg;

	// Main message loop:
	while( GetMessage( &msg,nullptr,0,0 ) )
	{
		if( !TranslateAccelerator( msg.hwnd,hAccelTable,&msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return ( int )msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance,MAKEINTRESOURCE( IDI_AESCSPRITE ) );
	wcex.hCursor = LoadCursor( nullptr,IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = MAKEINTRESOURCEW( IDC_AESCSPRITE );
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon( wcex.hInstance,MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassExW( &wcex );
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance( HINSTANCE hInstance,int nCmdShow )
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW( szWindowClass,szTitle,WS_OVERLAPPEDWINDOW,
		// CW_USEDEFAULT,0,CW_USEDEFAULT,0,
		150,350,initWindowSize.x,initWindowSize.y,
		nullptr,nullptr,hInstance,nullptr );

	if( !hWnd )
	{
		return FALSE;
	}

	ShowWindow( hWnd,nCmdShow );
	UpdateWindow( hWnd );

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc( HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )
{
	const auto pos = Vei2{
		GET_X_LPARAM( lParam ),
		GET_Y_LPARAM( lParam ) };

	switch( message )
	{
	case WM_COMMAND:
	{
		editor.ShowMouse( TRUE );
		editor.ToggleMouseHiding( false );
		int wmId = LOWORD( wParam );
		// Parse the menu selections:
		switch( wmId )
		{
		case IDM_ABOUT:
			DialogBox( hInst,MAKEINTRESOURCE( IDD_ABOUTBOX ),hWnd,About );
			break;
		case ID_FILE_RESIZE:
			DialogBox( hInst,MAKEINTRESOURCE( IDD_RESIZE ),hWnd,Resize );
			Repaint( hWnd );
			break;
		case ID_FILE_LOADPALETTE:
		{
			const int options = editor.TryLoadPal();
			if( options >= 0 )
			{
				LPARAM dialogInfo = options;
				const INT_PTR result = DialogBoxParam( hInst,
					MAKEINTRESOURCE( IDD_ASKPAL ),hWnd,AskPal,dialogInfo );
				if( result >= 0 ) editor.LoadPalette( int( result ) );
				Repaint( hWnd );
				ResizeWnd( hWnd );
			}
		}
		break;
		case ID_FILE_OPEN:
		{
			const int options = editor.TryOpenFile();
			if( options >= 0 )
			{
				LPARAM dialogInfo = options;
				const INT_PTR result = DialogBoxParam( hInst,
					MAKEINTRESOURCE( IDD_ASKPAL ),hWnd,AskPal,dialogInfo );
				if( result >= 0 ) editor.OpenFile( int( result ) );
				Repaint( hWnd );
				ResizeWnd( hWnd );
			}
		}
		break;
		case ID_FILE_SAVE:
			editor.SaveFile();
			break;
		case ID_FILE_SAVEAS:
			editor.SaveFileAs();
			break;
		case IDM_EXIT:
			DestroyWindow( hWnd );
			break;
		default:
			return DefWindowProc( hWnd,message,wParam,lParam );
		}
		editor.ToggleMouseHiding( true );
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint( hWnd,&ps );
		// TODO: Add any drawing code that uses hdc here...

		RECT clientRect;
		GetClientRect( hWnd,&clientRect );

		HDC buffer = CreateCompatibleDC( hdc );
		HBITMAP hBmp = CreateCompatibleBitmap( hdc,
			clientRect.right - clientRect.left,
			clientRect.bottom - clientRect.top );
		DeleteObject( SelectObject( buffer,hBmp ) );

		editor.HandlePaint( buffer );

		BitBlt( hdc,clientRect.left,clientRect.top,
			clientRect.right - clientRect.left,
			clientRect.bottom - clientRect.top,
			buffer,0,0,SRCCOPY );
		ReleaseDC( hWnd,buffer );
		DeleteObject( buffer );
		EndPaint( hWnd,&ps );
	}
	break;
	case WM_LBUTTONDOWN:
		if( editor.HandleMouseDown( pos ) ) Repaint( hWnd );
	break;
	case WM_LBUTTONUP:
		if( editor.HandleMouseUp( pos ) ) Repaint( hWnd );
		break;
	case WM_MOUSEMOVE:
		if( editor.HandleMouseMove( pos ) ) Repaint( hWnd );
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		// No auto repeat.
		if( !( lParam & 0x40000000 ) )
		{
			if( editor.HandleKeyDown( static_cast<unsigned char>( wParam ) ) ) Repaint( hWnd );
		}
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		if( editor.HandleKeyUp( static_cast<unsigned char>( wParam ) ) ) Repaint( hWnd );
		break;
	case WM_SIZE:
		ResizeWnd( hWnd );
		Repaint( hWnd );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd,message,wParam,lParam );
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About( HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch( message )
	{
	case WM_INITDIALOG:
		return ( INT_PTR )TRUE;

	case WM_COMMAND:
		if( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
		{
			EndDialog( hDlg,LOWORD( wParam ) );
			return ( INT_PTR )TRUE;
		}
		break;
	}
	return ( INT_PTR )FALSE;
}

// Message handler for resize box.
INT_PTR CALLBACK Resize( HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam )
{
	RECT wnd;
	RECT rc;
	GetClientRect( hDlg,&rc );
	GetWindowRect( GetParent( hDlg ),&wnd );
	SetWindowPos( hDlg,HWND_TOP,
		( wnd.left + wnd.right ) / 2 - ( rc.left + rc.right ) / 2,
		( wnd.top + wnd.bottom ) / 2 - ( rc.top + rc.bottom ) / 2,
		0,0,SWP_NOSIZE );

	switch( message )
	{
	case WM_INITDIALOG:
		const auto canvSize = editor.GetCanvSize();
		SetDlgItemInt( hDlg,IDC_WIDTHINPUT,canvSize.x,FALSE );
		SetDlgItemInt( hDlg,IDC_HEIGHTINPUT,canvSize.y,FALSE );
		return( INT_PTR( TRUE ) );
	case WM_COMMAND:
		if( LOWORD( wParam ) == IDC_ACCEPT )
		{
			const int width = GetDlgItemInt( hDlg,IDC_WIDTHINPUT,NULL,FALSE );
			const int height = GetDlgItemInt( hDlg,IDC_HEIGHTINPUT,NULL,FALSE );
			editor.ResizeImage( Vei2{ width,height } );
			EndDialog( hDlg,LOWORD( wParam ) );
			return( INT_PTR( TRUE ) );
		}
		if( LOWORD( wParam ) == IDCANCEL )
		{
			EndDialog( hDlg,LOWORD( wParam ) );
			return( INT_PTR( TRUE ) );
		}
		break;
	}
	return( INT_PTR( FALSE ) );
}

// Message handler for ask pal box.
INT_PTR CALLBACK AskPal( HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
	{
		const wchar_t* text = L"Keep existing palette, replace with loaded image's palette, or append to the current palette?";
		SetDlgItemText( hDlg,IDC_STATIC,text );
		switch( lParam )
		{
		case 0:
			break;
		case 1:
			// disable keep
			EnableWindow( GetDlgItem( hDlg,IDC_KEEP ),FALSE );
			break;
		case 2:
			// disable keep and replace
			EnableWindow( GetDlgItem( hDlg,IDC_KEEP ),FALSE );
			EnableWindow( GetDlgItem( hDlg,IDC_REPLACE ),FALSE );
			break;
		}
		return ( INT_PTR )TRUE;
	}
	case WM_COMMAND:
	{
		int result = -1;
		switch( LOWORD( wParam ) )
		{
		case IDCANCEL:
			result = -1;
			break;
		case IDC_KEEP:
			result = 0;
			break;
		case IDC_REPLACE:
			result = 1;
			break;
		case IDC_APPEND:
			result = 2;
			break;
		}
		EndDialog( hDlg,INT_PTR( result ) );
		return ( INT_PTR )TRUE;
	}
	break;
	}
	return ( INT_PTR )FALSE;
}