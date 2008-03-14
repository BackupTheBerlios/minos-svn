/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		VHF Contest Adjudication
//
// COPYRIGHT         M. J. Goodey, 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
#ifndef MTraceH
#define MTraceH 
//---------------------------------------------------------------------------
std::string lastError( DWORD erno );
std::string lastError( void );
//---------------------------------------------------------------------------
void trace( char *mess );
void trace( const std::string & mess );
void enableTrace( const std::string &where );
void disableTrace( );
#endif
