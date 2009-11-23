/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
// we need to override LEAN AND MEAN as shbrowse requires it
#define NO_WIN32_LEAN_AND_MEAN
#pragma hdrstop

#include <vcl.h>
#include <string>
#include "shbrowse.h"

// wrapper for SHBrowseForFolder call

int _stdcall change_dir_callback( HWND hwnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData )
{
   if ( uMsg == BFFM_INITIALIZED )
   {
      SendMessage( hwnd, BFFM_SETSELECTION, 1, lpData );
   }
   return 0;
}

bool SimpleBrowseDirectory( std::string &Dest,
                                       Controls::TWinControl* const AParent,
                                       const std::string &Title )
{
   LPMALLOC g_pMalloc;

   if ( !SUCCEEDED( SHGetMalloc( &g_pMalloc ) ) )
      return false;

   BROWSEINFO BrowsingInfo;
   char DirPath[ MAX_PATH ];
   char FolderName[ MAX_PATH ];
   LPCITEMIDLIST ItemID;

   memset( &BrowsingInfo, 0, sizeof( BROWSEINFO ) );
   memset( DirPath, 0, MAX_PATH );

   BrowsingInfo.hwndOwner = AParent->Handle;
   BrowsingInfo.pszDisplayName = FolderName;
   BrowsingInfo.lpszTitle = Title.c_str();
   if ( Dest.size() )
   {
      BrowsingInfo.lpfn = change_dir_callback;
      BrowsingInfo.lParam = ( LPARAM ) Dest.c_str();
   }

   ItemID = SHBrowseForFolder( &BrowsingInfo );

   if ( ItemID != NULL )
   {
      SHGetPathFromIDList( ItemID, DirPath );
      Dest = DirPath;
      g_pMalloc->Free( ( void * ) ItemID );
      g_pMalloc->Release();
      return true;
   }
   g_pMalloc->Release();
   return false;
}
//---------------------------------------------------------------------------
