/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef CalcsH
#define CalcsH 
//----------------------------------------------------------------------------
extern int normBrg( int mbear );
extern int varBrg( int mbear );
extern char lonlat( const std::string &qra, double &lon, double &lat );
extern std::string WtodbW( const std::string &W );
extern std::string dbWtoW( const std::string &dbW );

#endif
