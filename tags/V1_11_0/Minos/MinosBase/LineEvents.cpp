/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "base_pch.h"
#pragma hdrstop

#include "LineEvents.h"
#include "tinyxml.h"

//---------------------------------------------------------------------------
#define MAX_FILE_SIZE 512 
//---------------------------------------------------------------------------
void ( *LineSet::lsLog ) ( const std::string & ) = 0;
static void Log( const std::string &msg )
{
   if ( LineSet::lsLog )
   {
      ( *LineSet::lsLog ) ( msg );
   }
}
class LineFileBlock
{
   private:
      static HANDLE LineProtectionMutex;
      static DWORD ownerThreadId;
      bool releaseOnExit;

   public:
      LineFileBlock();
      ~LineFileBlock();
};
HANDLE LineFileBlock::LineProtectionMutex = 0;
DWORD LineFileBlock::ownerThreadId = 0;

LineFileBlock::LineFileBlock() : /* first( false ),*/ releaseOnExit( false )
{
   if ( !LineProtectionMutex )
   {
      DWORD eno;

      // We faff about with security descriptors etc to allow other processes
      // and users to see the mutex
      SetLastError( 0 );
      SECURITY_DESCRIPTOR sd;
      if ( !InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION ) )
      {
         eno = GetLastError();
         throw( Exception( "InitializeSecurityDescriptor failed" ) );
      }
      if ( !SetSecurityDescriptorDacl( &sd, TRUE, 0, FALSE ) )
      {
         eno = GetLastError();
         throw( Exception( "SetSecurityDescriptorDacl failed" ) );
      }

      // create a mutex using the security descriptor
      SECURITY_ATTRIBUTES sa;
      sa.nLength = sizeof( sa );
      sa.lpSecurityDescriptor = &sd;
      sa.bInheritHandle = FALSE;

      LineProtectionMutex = CreateMutex( &sa, FALSE, "minos Line Protection Mutex" ); // Mutex, not initially owned

      eno = GetLastError();
      // it should be the initial creator of the mutex
      if ( eno )
      {
         if ( eno != ERROR_ALREADY_EXISTS )
         {
            throw( Exception( "Failed to create mutex \"minos Line Protection Mutex\"" ) );
         }
      }
   }

   if ( ownerThreadId == GetCurrentThreadId() )
   {
      // This thread already owns the mutex; don't grab it again. and don't release it early
      releaseOnExit = false;
   }
   else
   {
      // We may want to time out the wait, and whinge if we time out
      if ( WaitForSingleObject( LineProtectionMutex, INFINITE ) != WAIT_OBJECT_0 )
      {
         releaseOnExit = false;
         ownerThreadId = 0;
         //the mutex was abandoned
         throw Exception( "minos Line Protection Mutex abandoned" );
         // throw is safe as we don't need to destroy anything
      }
      releaseOnExit = true;
      ownerThreadId = GetCurrentThreadId();
   }
}
LineFileBlock::~LineFileBlock()
{
   // if we were not the first to try to get the mutex in this thread
   // then we mustn't release it either
   if ( releaseOnExit )
   {
      ownerThreadId = 0;
      ReleaseMutex( LineProtectionMutex );
   }
}
//===================================================================
class LineFileMapper
{
   private:
      HANDLE hFile;
      HANDLE hMap;
      char *lpPtr;
      GJV_thread *mappedLineEventThread;
      bool killThread;
   public:
      std::map < String, int > LineMap;

      LineFileMapper();
      ~LineFileMapper();
      bool Initialise();
      void Close();
      void Flush()
      {
         FlushViewOfFile( lpPtr, 0 );
      }
      char *GetMap()
      {
         return lpPtr;
      }
      void __fastcall ProcessLineFile();
      void doProcLineEvents( );
};
LineFileMapper::LineFileMapper() : hMap( 0 ), lpPtr( 0 ),
      mappedLineEventThread( 0 ), killThread( false )
{}
LineFileMapper::~LineFileMapper()
{
   Close();
}
void LineFileMapper::Close()
{
   killThread = true;
   if ( mappedLineEventThread )
   {
      mappedLineEventThread->GJV_join();
      delete mappedLineEventThread;
      mappedLineEventThread = 0;
   }

   if ( lpPtr )
   {
      Flush();
      UnmapViewOfFile( lpPtr );
   }
   lpPtr = 0;
   if ( hMap )
      CloseHandle( hMap );
   hMap = 0;
   if ( hFile )
      CloseHandle( hFile );
   hFile = 0;
}

void LineFileMapper::doProcLineEvents( )
{
   LineSet * ls = LineSet::GetLineSet();
   while ( !killThread )
   {
      if ( WaitForSingleObject( ls ->LineChangedEvent, 500 ) == WAIT_OBJECT_0 )
      {
         ls->lineChanged = true;
      }
   }
}
void procLineEvents( void *p )
{
   //---- Place thread code here ----
   LineFileMapper * f = ( LineFileMapper* ) p;
   f->doProcLineEvents();
}
bool LineFileMapper::Initialise()
{
   LineFileBlock cfb;
   hFile = INVALID_HANDLE_VALUE;
   // hFile == INVALID_HANDLE_VALUE says use the paging file
   hMap = CreateFileMapping( hFile, 0, PAGE_READWRITE,   /*size high*/0,   /*size low*/MAX_FILE_SIZE, "minosSharedLineMeter" );
   if ( !hMap )
   {
      //      DWORD eno = GetLastError();
      throw( "Failed to create mapping for credit file." );
   }

   lpPtr = ( char * ) MapViewOfFile( hMap, FILE_MAP_ALL_ACCESS, 0, 0, MAX_FILE_SIZE );   // Map whole file up to max size
   if ( !lpPtr )
   {
      //      DWORD eno = GetLastError();
      throw( "Failed to map view of line file." );
   }

   mappedLineEventThread = new GJV_thread( "mappedLineEventThread", procLineEvents, ( void * ) this, true ) ;
   return true;
}
//---------------------------------------------------------------------------
LineSet::LineSet() : FileMap( new LineFileMapper ), closing( false )
{
}
LineSet *LineSet::myLineSet = 0;
LineSet *LineSet::GetLineSet()
{
   if ( !myLineSet )
   {
      LineFileBlock guard;
      myLineSet = new LineSet( );
      myLineSet->initialise();
   }

   return myLineSet;
}
bool LineSet::initialise()
{
   DWORD eno;

   SetLastError( 0 );
   SECURITY_DESCRIPTOR sd;
   if ( !InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION ) )
   {
      eno = GetLastError();
      return false;
   }
   if ( !SetSecurityDescriptorDacl( &sd, TRUE, 0, FALSE ) )
   {
      eno = GetLastError();
      return false;
   }

   // create a mutex using the security descriptor
   SECURITY_ATTRIBUTES sa;
   sa.nLength = sizeof( sa );
   sa.lpSecurityDescriptor = &sd;
   sa.bInheritHandle = FALSE;

   LineChangedEvent = CreateEvent( &sa, TRUE, FALSE, "minos fast line Event" ); // Manual reset

   eno = GetLastError();
   if ( eno && eno != ERROR_ALREADY_EXISTS )
   {
      return false;
   }

   bool ret = FileMap->Initialise();
   if ( ret )
   {
      return readSet();
   }
   return false;
}
LineSet::~LineSet()
{
   closing = true;
   delete FileMap;
}
void LineSet::writeSet()
{
   std::string cxml;
   //  cxml += "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
   cxml += "<Lines>";
   for ( std::map < std::string, bool > ::iterator i = LineMap.begin(); i != LineMap.end(); i++ )
   {
      std::string name = i->first;
      std::string val = ( i->second ? "1" : "0" );

      cxml += std::string( "<" ) + name + " state=\"" + val + "\"/>";
   }
   cxml += "</Lines>\0\0";
   // modify memory mapped file
   char linebuff[ MAX_FILE_SIZE ];
   memset( linebuff, 0, MAX_FILE_SIZE );
   memcpy( linebuff, cxml.c_str(), cxml.size() ); //include zero termination
   linebuff[ cxml.size() ] = 0;
   memcpy( FileMap->GetMap(), linebuff, MAX_FILE_SIZE ); //include zero termination
   Log( std::string( "writeSet (publish)  " ) + FileMap->GetMap() );
   FileMap->Flush();
}
bool LineSet::readSet()
{
   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument linedoc;
   linedoc.Parse( FileMap->GetMap() );
   TiXmlElement *root = linedoc.RootElement();
   if ( !root )
   {
      //err = "No XML root element (does file exist?)";
      return false;
   }
   std::string rootName = root->Value();
   if ( !( rootName == "Lines" ) )
   {
      Log( "readSet Invalid XML root element " + rootName );
      return false;
   }
   for ( TiXmlElement * e = root->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      std::string name = e->Value();
      const char * a = e->Attribute( "state" );
      if ( a[ 0 ] == '1' )
      {
         LineMap[ name ] = true;
      }
      else
      {
         LineMap[ name ] = false;
      }
   }
   return true;
}

bool LineSet::waitForChange( unsigned int ms )
{
   // Wait for line changed event
   // wait for 10 seconds at a time to allow threads to terminate

   while ( true )
   {
      DWORD timeStart = GetTickCount();

      if ( closing || WaitForSingleObject( GetLineSet() ->LineChangedEvent, 100 ) == WAIT_OBJECT_0 )
      {
         return true;
      }
      if ( lineChanged )
      {
         lineChanged = false;
         return true;
      }
      if ( GetTickCount() - timeStart >= ms )
      {
         return false;
      }
   }
}
void LineSet::publish( const std::string &name, bool state )
{
   LineFileBlock guard;
   readSet();
   Log( std::string( "readSet  (publish)  " ) + FileMap->GetMap() );
   LineMap[ name ] = state;
   writeSet();
   ::PulseEvent( LineChangedEvent );
}
void LineSet::readLines()
{
   LineFileBlock guard;
   readSet();
   Log( std::string( "readSet (readLines) " ) + FileMap->GetMap() );

}
bool LineSet::getState( const std::string name )
{
   for ( std::map <std::string, bool>::iterator i = LineMap.begin(); i != LineMap.end(); i++ )
   {
      if ( name == ( *i ).first )
      {
         return ( *i ).second;
      }
   }

   return false;
}
//---------------------------------------------------------------------------

