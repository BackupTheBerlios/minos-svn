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

CRITICAL_SECTION *CsGuard::m_CritSec = 0;
//---------------------------------------------------------------------------
/*static*/
void GJV_thread::setThreadName( const char *name )
{
   struct TThreadNameInfo
   {
      DWORD FType;     // must be 0x1000
      const char *FName;        // pointer to name (in user address space)
      DWORD FThreadID; // thread ID (-1 indicates caller thread)
      DWORD FFlags;    // reserved for future use, must be zero
   };
   TThreadNameInfo threadNameInfo;
   threadNameInfo.FType = 0x1000;
   threadNameInfo.FName = name;
   threadNameInfo.FThreadID = 0xFFFFFFFF;
   threadNameInfo.FFlags = 0;

   __try
   {
      RaiseException( 0x406D1388, 0, sizeof( TThreadNameInfo ) / sizeof( DWORD ), ( CONST ULONG_PTR * ) & threadNameInfo );
   }
   __except( EXCEPTION_CONTINUE_EXECUTION )
   {}
}

GJV_scoped_lock::GJV_scoped_lock()
{}
GJV_scoped_lock::~GJV_scoped_lock()
{}


GJV_thread::GJV_thread( const std::string &name ) : threadName( name )
{}
static unsigned __stdcall threadMain( void *arg )
{
   GJV_thread * t = reinterpret_cast< GJV_thread * > ( arg );

   GJV_thread::setThreadName( t->getThreadName() );

   ( *( t->tex ) ) ( t->arg );
   return 0;
}
GJV_thread::GJV_thread( const std::string &name, void ( *tex ) ( void * ),
                        void *arg, bool aboveNormalPriority )
      : Terminated( false ), arg( arg ) , tex( tex ), threadName( name )
{
   unsigned int dwOutputThreadId;
   hThread = ( HANDLE ) _beginthreadex( NULL,   // security
                                        0,                           // stacksize
                                        threadMain,                  // start
                                        this,                           // arg
                                        aboveNormalPriority ? ABOVE_NORMAL_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS,       // createflags
                                        &dwOutputThreadId          // threadid
                                      );
}
GJV_thread::~GJV_thread()
{
   if ( hThread )
      CloseHandle( hThread );
}

void GJV_thread::GJV_join()
{
   // called from main thread
   Terminated = true;
   // Wait for request to close or for a read thread to terminate.
   /*DWORD dwWait = */
   WaitForSingleObject( hThread, 10000 );

   // either 10 secs or all threads terminated...

   CloseHandle( hThread );
   hThread = 0;
}


