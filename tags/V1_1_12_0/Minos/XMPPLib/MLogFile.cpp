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
//---------------------------------------------------------------------------
#pragma package(smart_init) 
//---------------------------------------------------------------------------
static std::ofstream &getLogFile( std::string name )
{
   static std::ofstream logFile( name.c_str() );
   return logFile;
}

//---------------------------------------------------------------------------
std::ostream & MLogFile::createLogFile( const std::string & prefix, int keepDays )
{
   //Close();
   TDateTime dt = TDateTime::CurrentDateTime();
   String tidyPrefix = prefix.c_str() + String( "*" );
   tidyFiles( tidyPrefix.c_str(), keepDays );
   fLogFileName = generateLogFileName( prefix ).c_str();

   std::string sDir = ExtractFileDir( fLogFileName.c_str() ).c_str();
   if ( !DirectoryExists( sDir.c_str() ) )
      CreateDir( sDir.c_str() );

   unsigned short h, m, s, ms;
   dt.DecodeTime( &h, &m, &s, &ms );
   AnsiString sdt;
	AnsiString dtg = dt.FormatString( "yyyy mmm dd hh:nn:ss" );
	sdt.printf( "%s.%04.4d", dtg.c_str(), ms );

	//   getLogFile().open( fLogFileName.c_str() );
	getLogFile( fLogFileName );
	return log() << sdt.c_str() << std::endl;
}
//---------------------------------------------------------------------------
std::ostream & MLogFile::log( void )
{
	return getLogFile( "" );
}
//---------------------------------------------------------------------------
std::ostream & MLogFile::logT( void )
{
	GJV_scoped_lock scoped_lock;
	TDateTime dt = TDateTime::CurrentDateTime();
	unsigned short h, m, s, ms;
	dt.DecodeTime( &h, &m, &s, &ms );
	AnsiString sdt;
	DWORD id = GetCurrentThreadId();
	AnsiString time = dt.FormatString( "hh:nn:ss" );
	sdt.printf( "%s.%04.4d <%4.4d>", time.c_str(), ms, id );
   return log() << sdt.c_str();
}
//---------------------------------------------------------------------------
void MLogFile::close( void )
{
   getLogFile( "" ).close();
}
//---------------------------------------------------------------------------
std::ostream & MLogFile::log( const std::string &s )
{
   GJV_scoped_lock scoped_lock;
   return logT() << s.c_str() << std::endl;
}

//---------------------------------------------------------------------------
/*static */String MLogFile::generateLogFileName(   const std::string & prefix )
{
   TDateTime dt = TDateTime::CurrentDateTime();
   String s( prefix.c_str() );
   s += dt.FormatString( "yyyymmdd_hhnnss" ) + ".log";
   return s;
}
//---------------------------------------------------------------------------
/*static */void MLogFile::tidyFiles(   const std::string & prefix,   int keepDays )
{
   String s = String( prefix.c_str() );

   String sDir = ExtractFileDir( s );
   if ( sDir != "" )
   {
      if ( sDir[ sDir.Length() ] != '\\' && sDir[ sDir.Length() ] != '/' )
         sDir += '\\';
   }
   TDateTime kdt = TDateTime::CurrentDateTime() - keepDays;

   TSearchRec sr;
   int iAttributes = faDirectory | faSysFile | faHidden;

   if ( FindFirst( s, faAnyFile	, sr ) == 0 )
   {
      do
      {
         if ( sr.Name != "" && sr.Attr & ~iAttributes )
         {
            TDateTime dt;
            dt = dt.FileDateToDateTime( sr.Time );
            if ( dt < kdt )
            {
               String sName = sDir + sr.Name;
               DeleteFile( sName.c_str() );
            }
         }
      }
      while ( FindNext( sr ) == 0 );
      FindClose( sr );
   }
}

