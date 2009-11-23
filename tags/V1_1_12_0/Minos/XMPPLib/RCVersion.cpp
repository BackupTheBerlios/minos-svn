/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "XMPP_pch.h"
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMyRCVersion::TMyRCVersion()
      : FhInstance( 0 ), FInfoFrom( ifEXE ), FVData( 0 )
{}
//---------------------------------------------------------------------------
__fastcall TMyRCVersion::~TMyRCVersion()
{
   if ( FVData )
      delete[] FVData;
   FVData = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMyRCVersion::setValue( String )
{
   ::MessageBeep( 0 );
}
//---------------------------------------------------------------------------
void __fastcall TMyRCVersion::SetInstance( HINSTANCE n )
{
   FhInstance = n;
   if ( FhInstance )
      FInfoFrom = ifDLL;
   else
      FInfoFrom = ifEXE;
   initialise();
}
//---------------------------------------------------------------------------
void __fastcall TMyRCVersion::initialise( void )
{
   delete[] FVData;
   FVData = 0;

   DWORD fvHandle;
   unsigned int vSize;
   char appFName[ 255 ];
   char * subBlockName = new char[ 255 ];
   HINSTANCE hInstance = 0; // default to application
   switch ( FInfoFrom )
   {
      case ifDLL:
         hInstance = FhInstance;
         break;
      case ifPackage:
         hInstance = HInstance; // DLL Module Instance for package
         break;
   }
   int nLen = ::GetModuleFileName( hInstance, appFName, 255 );
   appFName[ nLen ] = '\0';
   OemToChar( appFName, appFName );
   DWORD dwSize = ::GetFileVersionInfoSize( appFName, &fvHandle );
   if ( dwSize )
   {
      FVData = ( void * ) new char[ dwSize ];
      if ( ::GetFileVersionInfo( appFName, fvHandle, dwSize, FVData ) )
      {
         // Copy string to buffer so if the -dc compiler switch(Put constant strings in code segments)
         // is on VerQueryValue will work under Win16.  This works around a problem in Microsoft's ver.dll
         // which writes to the string pointed to by subBlockName.
         //
         strcpy( subBlockName, "\\VarFileInfo\\Translation" );
         if ( !::VerQueryValue( FVData, subBlockName, ( void * * ) & TransBlock, &vSize ) )
         {
            delete[] FVData;
            FVData = 0;
         }
         else
            // Swap the words so sprintf will print the lang-charset in the correct format.
            //
            *TransBlock = MAKELONG( HIWORD( *TransBlock ), LOWORD( *TransBlock ) );
      }
   }
   delete [] subBlockName;
}
//---------------------------------------------------------------------------
void __fastcall TMyRCVersion::SetInfoFrom( eInfoFrom e )
{
   if ( FInfoFrom == e )
      return ;

   FInfoFrom = e;
   initialise();
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::getValue( const char * itemName )
{
   unsigned int vSize;
   char * subBlockName = new char[ 255 ];
   char * ItemValue = new char[ 2000 ];
   char *p1 = &ItemValue[ 0 ];

   ItemValue[ 0 ] = '\0';
   String itemValue;
   if ( FVData )
   {
      sprintf( subBlockName, "\\StringFileInfo\\%08lx\\%s", *TransBlock, ( LPSTR ) itemName );
      if ( ::VerQueryValue( FVData, subBlockName, ( void * * ) & ItemValue, &vSize ) )
         itemValue = ItemValue;
   }
   delete [] subBlockName;
   delete [] p1;

   return itemValue;
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetLegalCopyright( void )
{
   return getValue( "LegalCopyright" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetCompanyName( void )
{
   return getValue( "CompanyName" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetFileDescription( void )
{
   return getValue( "FileDescription" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetFileVersion( void )
{
   return getValue( "FileVersion" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetInternalName( void )
{
   return getValue( "InternalName" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetLegalTrademarks( void )
{
   return getValue( "LegalTrademarks" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetOriginalFilename( void )
{
   return getValue( "OriginalFilename" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetProductName( void )
{
   return getValue( "ProductName" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetProductVersion( void )
{
   return getValue( "ProductVersion" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetComments( void )
{
   return getValue( "Comments" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetSpecialBuild( void )
{
   return getValue( "SpecialBuild" );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetMajorVersion( void )
{
   String s = GetFileVersion();
   // s = major.minor.release.build
   int n = s.Pos( '.' );
   if ( n )
      return s.SubString( 1, n - 1 );
   return s;
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetMinorVersion( void )
{
   String s = GetFileVersion();
   int nStart = s.Pos( '.' );
   if ( !nStart )
      return s;
   s[ nStart ] = ' ';
   int nEnd = s.Pos( '.' );
   return s.SubString( nStart + 1, nEnd - 1 - nStart );
}
//---------------------------------------------------------------------------
String __fastcall TMyRCVersion::GetRelease( void )
{
   String s = GetFileVersion();
   int nStart = s.Pos( '.' );
   if ( !nStart )
      return s;
   s[ nStart ] = ' ';
   nStart = s.Pos( '.' );
   if ( !nStart )
      return s;
   s[ nStart ] = ' ';
   int nEnd = s.Pos( '.' );
   return s.SubString( nStart + 1, nEnd - 1 - nStart );
}
//---------------------------------------------------------------------------
