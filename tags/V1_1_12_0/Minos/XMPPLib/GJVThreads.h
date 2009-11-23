/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef GJVThreadsH
#define GJVThreadsH
#include <string>
class CsGuard
{
      static CRITICAL_SECTION *m_CritSec;
   public:
      CsGuard()
      {
         if ( !m_CritSec )
         {
            m_CritSec = new CRITICAL_SECTION;
            InitializeCriticalSection( m_CritSec );
         }
         EnterCriticalSection( m_CritSec );
      }

      ~CsGuard()
      {
         if ( m_CritSec )
            LeaveCriticalSection( m_CritSec );
      }
      static void ClearDown()
      {
         if ( m_CritSec )
         {
            DeleteCriticalSection( m_CritSec );
            delete m_CritSec;
            m_CritSec = 0;
         }
      }
};
//---------------------------------------------------------------------------
class GJV_scoped_lock
         : public CsGuard
{
   public:
      GJV_scoped_lock();
      ~GJV_scoped_lock();
};

class GJV_thread
{
   private:
      HANDLE hThread;
      std::string threadName;
   protected:
      bool Terminated;
   public:
      GJV_thread( const std::string &name );
      GJV_thread( const std::string &name, void ( *tex ) ( void * ), void *arg, bool aboveNormalPriority = false );
      ~GJV_thread();

      static void setThreadName( const char *name );

      void *arg;
      void ( *tex ) ( void * );

      void GJV_join();
      const char *getThreadName()
      {
         return threadName.c_str();
      }
};

#endif
