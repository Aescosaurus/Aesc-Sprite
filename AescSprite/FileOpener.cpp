#include "stdafx.h"
#include "FileOpener.h"
#include <Windows.h>
#include <ShObjIdl.h>
#include <ShObjIdl_core.h>
#include <shtypes.h>

std::string FileOpener::OpenFile()
{
	HRESULT hr = CoInitializeEx( NULL,COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE );
	std::string path = "";

	if( SUCCEEDED( hr ) )
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance( CLSID_FileOpenDialog,NULL,CLSCTX_ALL,
			IID_IFileOpenDialog,reinterpret_cast< void** >( &pFileOpen ) );

		if( SUCCEEDED( hr ) )
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show( NULL );

			// Get the file name from the dialog box.
			if( SUCCEEDED( hr ) )
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult( &pItem );
				if( SUCCEEDED( hr ) )
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName( SIGDN_FILESYSPATH,&pszFilePath );

					// Display the file name to the user.
					if( SUCCEEDED( hr ) )
					{
						// MessageBox( NULL,pszFilePath,L"File Path",MB_OK );
						for( int i = 0; pszFilePath[i] != '\0'; ++i )
						{
							path += char( pszFilePath[i] );
						}
						CoTaskMemFree( pszFilePath );
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}

	return( path );
}

std::string FileOpener::SaveFile()
{
	HRESULT hr = CoInitializeEx( NULL,COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE );
	std::string path = "";

	if( SUCCEEDED( hr ) )
	{
		IFileSaveDialog *pFileSave;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance( CLSID_FileSaveDialog,NULL,CLSCTX_ALL,
			IID_IFileSaveDialog,reinterpret_cast< void** >( &pFileSave ) );

		if( SUCCEEDED( hr ) )
		{
			const COMDLG_FILTERSPEC c_rgSaveTypes[] =
			{
				{ L"Bitmap Image (*.bmp)",L"*.bmp" },
				{ L"All Documents (*.*)",L"*.*" }
			};
			const int INDEX_BITMAP = 0;
			hr = pFileSave->SetFileTypes( ARRAYSIZE( c_rgSaveTypes ),c_rgSaveTypes );
			if( SUCCEEDED( hr ) )
			{
				// Set the selected file type index to Word Docs for this example.
				hr = pFileSave->SetFileTypeIndex( INDEX_BITMAP );
				if( SUCCEEDED( hr ) )
				{
					// Show the Open dialog box.
					hr = pFileSave->Show( NULL );

					// Get the file name from the dialog box.
					if( SUCCEEDED( hr ) )
					{
						IShellItem *pItem;
						hr = pFileSave->GetResult( &pItem );
						if( SUCCEEDED( hr ) )
						{
							PWSTR pszFilePath;
							hr = pItem->GetDisplayName( SIGDN_FILESYSPATH,&pszFilePath );

							// Display the file name to the user.
							if( SUCCEEDED( hr ) )
							{
								// MessageBox( NULL,pszFilePath,L"File Path",MB_OK );
								for( int i = 0; pszFilePath[i] != '\0'; ++i )
								{
									path += char( pszFilePath[i] );
								}
								CoTaskMemFree( pszFilePath );
							}
							pItem->Release();
						}
					}
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}

	if( path != "" )
	{
		if( path.substr( path.length() - 4 ) != ".bmp" ) path += ".bmp";
	}

	return( path );
}