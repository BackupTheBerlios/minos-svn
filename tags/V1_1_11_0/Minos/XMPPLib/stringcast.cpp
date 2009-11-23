/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "XMPP_pch.h"
#pragma hdrstop

template < typename T >
std::string string_cast( const T& o )
{
   std::ostringstream oss;
   oss << o;
   if ( !oss )
   {
      throw /*...*/;
   }
   return oss.str();
}

/*
std::string makeStr( DWORD i )
{
   return string_cast<int>( i );
}
*/
std::string makeStr( unsigned long i )
{
   return string_cast<unsigned long>( i );
}
std::string makeStr( long i )
{
   return string_cast<long>( i );
}
std::string makeStr( int i )
{
   return string_cast<int>( i );
}
std::string makeStr( double i )
{
   return string_cast<double>( i );
}
std::string makeStr( unsigned int i )
{
   return string_cast<unsigned int>( i );
}
std::string makeStr( bool i )
{
   return ( i ? "true" : "false" );
}
