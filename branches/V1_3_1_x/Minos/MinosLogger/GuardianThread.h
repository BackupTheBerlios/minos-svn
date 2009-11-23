/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef GuardianThreadH
#define GuardianThreadH 
//---------------------------------------------------------------------------
#include <Classes.hpp> 
//---------------------------------------------------------------------------
class TGuardianThread
{
   private:
      //      std::string rundir;
      //      std::string runfile;
      TConfigElement *ce;
   protected:
      bool runApp( bool wait );
      void runDirectApp( bool wait );
      DWORD FindProcess( String ptf );
   public:
      TGuardianThread( TConfigElement *ce );
      void execute();
};
//---------------------------------------------------------------------------
#endif
