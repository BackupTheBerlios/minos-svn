/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H
#define PCH_H

#define NO_WIN32_LEAN_AND_MEAN

#include <vcl.h> 
//#include "base_pch.h"

#include <ctype.h>
#include <dir.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <Mem.h>
#include <shlobj.h>
#include <stdio.h>
#include <StdLib.h>
#include <String.h>
#include <sys\stat.h>
#include <time.h>

#include <windows.h>

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>

#include "boost\format.hpp"
#include "boost\shared_ptr.hpp"


#include "stringcast.h"
#include "mtrace.h"
#include "GJVThreads.h"
class disableInterrupts
{
      static CRITICAL_SECTION intCriticalSection;
   public:
      static void initialise()
      {
         InitializeCriticalSection( &intCriticalSection );
      }
      static void terminate()
      {
         DeleteCriticalSection( &intCriticalSection );
      }
      disableInterrupts()
      {
         EnterCriticalSection( &intCriticalSection );
      }
      ~disableInterrupts()
      {
         LeaveCriticalSection( &intCriticalSection );
      }
};

template < class qe >
class my_deque : public std::deque < qe >
{
   public:
      my_deque()
      {}
      ~my_deque()
      {}

      void freeAll()
      {
         disableInterrupts fred;
         for ( iterator i = begin(); i != end(); i++ )
            delete ( *i );
         clear();
      }

      qe find( const std::string &s )
      {
         disableInterrupts fred;
         for ( iterator i = begin(); i != end(); i++ )
            if ( stricmp( ( *i ) ->pName.c_str(), s.c_str() ) == 0 )
               return ( *i );
         return 0;
      }

      void clear_after ( qe e )
      {
         disableInterrupts fred;
         iterator i = std::find( begin(), end(), e );
         if ( i == end() )
            return ;
         i++;
         if ( i == end() )
            return ;
         for ( iterator i2 = i; i2 != end(); i2++ )
         {
            delete ( *i2 );
         }
         erase( i, end() );
      }
      void free_element ( qe e )
      {
         disableInterrupts fred;
         if ( e )
         {
            iterator i = std::find( begin(), end(), e );
            if ( i != end() )
            {
               erase( i );
            }
            delete e;
         }
      }

      qe next_element( qe q )
      {
         disableInterrupts fred;
         iterator i = std::find( begin(), end(), q );
         if ( i != end() )
         {
            i++;
            if ( i != end() )
               return * i;
         }
         return 0;
      }
};

#endif
