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

const double dtg::daySecs = 86400.0;	// 24 * 60 * 60
//---------------------------------------------------------------------------
GJVParams::GJVParams( HANDLE f )
      : diskBlock( 1 ), fd( f ), count( 0 )
{}
GJVParams::~GJVParams()
{}
//============================================================
locator::locator( void ) : valRes( ERR_NOLOC )
{
   loc.setInitialValue( "        " );
}
// default versions are good enough for now!
//locator::locator(const locator&)
//{
//}
//locator& locator::operator =(const locator&)
//{
//}
locator::~locator()
{}
char locator::validate( double &lon, double &lat )
{
   valRes = lonlat( loc.getValue(), lon, lat );
   return valRes;
}
char locator::validate( void )
{
   double longitude = 0.0;
   double latitude = 0.0;

   valRes = lonlat( loc.getValue(), longitude, latitude );

   return valRes;
}
//============================================================
//---------------------------------------------------------------------------
/*static*/TDateTime dtg::getRawUTC( )
{
   // TDateTime(0) is 12/30/1899
   SYSTEMTIME syst;
   GetSystemTime( &syst );

   // we need it as a FILETIME so we can do calculations...
   // FILETIME is 100-nanosecond intervals since January 1, 1601.
   FILETIME SysFileTime;  // 100 nanosec resolution
   SystemTimeToFileTime( &syst, &SysFileTime );

   ULARGE_INTEGER utc;
   utc.LowPart = SysFileTime.dwLowDateTime;
   utc.HighPart = SysFileTime.dwHighDateTime;

   __int64 lgTemp = utc.QuadPart;
   __int64 SecsTo1970 = 116444736000000000LL; // actually 100 ns units - from MSDN
   lgTemp -= SecsTo1970;

   lgTemp /= 10000;  //now millisecs
   lgTemp /= 1000;  //now secs

   double t;
   t = lgTemp;

   TDateTime tdt( t / daySecs );
   tdt += TDateTime( 1970, 1, 1 );
   return tdt;
}
/*static*/TDateTime dtg::getCorrectedUTC(  )
{
   TDateTime tdt = getRawUTC();
   double correction = MinosParameters::getMinosParameters() ->getBigClockCorrection();
   if ( correction )
      tdt += TDateTime( correction );
   return tdt;
}
// We really need to hold FULL dates internally!
dtg::dtg( bool now )
{
   if ( now )
   {
      TDateTime tdt = dtg::getCorrectedUTC();
      setDate( tdt.FormatString( "dd/mm/yy" ).c_str(), DTGDISP );
      setTime( tdt.FormatString( "hh:nn:ss" ).c_str(), DTGDISP );
   }
   else
   {
      setDate( "", DTGDISP );
      setTime( "", DTGDISP );
   }
}
void dtg::setIsoDTG( const std::string &d )
{
   // Untested! No errror handling!
   std::string curDate = d.substr( 2, 6 );
   std::string curTime = d.substr( 9, 8 );
   setDate( curDate, DTGLOG );
   setTime( curTime, DTGDISP );
}
std::string dtg::getIsoDTG( bool &d ) const
{
   // Untested! No error handling!
   std::string temp_date;
   std::string prefix = "20";

   bool dateDirty = false;
   bool timeDirty = false;

   std::string dateValue = sdate.getValue( dateDirty );
   dateValue += "            ";

   std::string timeValue = stime.getValue( timeDirty );
   timeValue += "            ";

   if ( dateValue [ 0 ] >= '8' )
      prefix = "19";

   temp_date = prefix + sdate.getValue();
   temp_date += "T";
   temp_date += timeValue [ 0 ];
   temp_date += timeValue [ 1 ];
   temp_date += ':';
   temp_date += timeValue [ 2 ];
   temp_date += timeValue [ 3 ];
   temp_date += ':';
   temp_date += timeValue [ 4 ];
   temp_date += timeValue [ 5 ];

   for ( int i = 0; i < 17; i++ )
      if ( temp_date[ i ] == 0 )
         temp_date[ i ] = ' ';

   d = dateDirty || timeDirty;
   return std::string( temp_date );
}
std::string dtg::getIsoDTG( ) const
{
   bool dirty;
   return getIsoDTG( dirty );
}
std::string dtg::getDate( DTG dstyle, bool &d ) const
{
   std::string temp_date;
   std::string dateValue = sdate.getValue( d );
   dateValue += "            ";
   if ( dstyle == DTGFULL )
   {
      std::string prefix = "20";

      if ( dateValue [ 0 ] >= '8' )
         prefix = "19";
      temp_date = prefix + dateValue;
      temp_date = temp_date.substr(0, 8);
   }
   else
      if ( dstyle == DTGLOG )
      {
         temp_date = dateValue;
         temp_date = temp_date.substr(0, 8);
      }
      else
         if ( dstyle == DTGReg1Test )
         {
            temp_date = dateValue;
            temp_date = temp_date.substr(0, 8);
         }
         else
         {
            temp_date += dateValue [ 4 ];
            temp_date += dateValue [ 5 ];
            temp_date += '/';
            temp_date += dateValue [ 2 ];
            temp_date += dateValue [ 3 ];
            temp_date += '/';
            temp_date += dateValue [ 0 ];
            temp_date += dateValue [ 1 ];

            for ( int i = 0; i < 8; i++ )
               if ( temp_date[ i ] == 0 )
                  temp_date[ i ] = ' ';
            temp_date = temp_date.substr(0, 8);
         }
   return temp_date;
}
std::string dtg::getDate( DTG dstyle ) const
{
   bool dirty;
   return getDate( dstyle, dirty );
}
std::string dtg::getTime( DTG dstyle, bool &d ) const
{
   std::string temp_time;
   std::string timeValue = stime.getValue( d );
   timeValue += "            ";

   if ( dstyle == DTGLOG )
   {
      temp_time = timeValue.substr(0, 6);
   }
   else
      if ( dstyle == DTGReg1Test )
      {
         temp_time += timeValue [ 0 ];
         temp_time += timeValue [ 1 ];
         temp_time += timeValue [ 2 ];
         temp_time += timeValue [ 3 ];
      }
      else
      {
         temp_time += timeValue [ 0 ];
         temp_time += timeValue [ 1 ];
         temp_time += ':';
         temp_time += timeValue [ 2 ];
         temp_time += timeValue [ 3 ];

         for ( int i = 0; i < 5; i++ )
            if ( temp_time[ i ] == 0 )
               temp_time[ i ] = ' ';
      }
   return temp_time;
}
std::string dtg::getTime( DTG dstyle ) const
{
   bool dirty;
   return getTime( dstyle, dirty );
}
bool dtg::getDtg( time_t &cttime, bool &d ) const
{
   std::string dateValue = sdate.getValue( d );
   dateValue += "            ";

   std::string timeValue = stime.getValue( d );
   timeValue += "            ";

   struct tm tmstr;
   for ( int i = 0; i < 6; i++ )
      if ( !isdigit( dateValue [ i ] ) )
         return false;
   for ( int i = 0; i < 4; i++ )
      if ( !isdigit( timeValue [ i ] ) )
         return false;

   tmstr.tm_sec = ( timeValue [ 4 ] - '0' ) * 10 + ( timeValue [ 5 ] - '0' );
   tmstr.tm_min = ( timeValue [ 2 ] - '0' ) * 10 + ( timeValue [ 3 ] - '0' );
   tmstr.tm_hour = ( timeValue [ 0 ] - '0' ) * 10 + ( timeValue [ 1 ] - '0' );

   tmstr.tm_mday = ( dateValue [ 4 ] - '0' ) * 10 + ( dateValue [ 5 ] - '0' );
   tmstr.tm_mon = ( dateValue [ 2 ] - '0' ) * 10 + ( dateValue [ 3 ] - '0' ) - 1;	// month is 0-11
   tmstr.tm_year = ( dateValue [ 0 ] - '0' ) * 10 + ( dateValue [ 1 ] - '0' );

   if ( tmstr.tm_year < 80 )
      tmstr.tm_year += 100;	// Y2K windowing

   tmstr.tm_isdst = 0;

   cttime = mktime( &tmstr );

   return true;
}
bool dtg::getDtg( time_t &cttime ) const
{
   bool dirty;
   return getDtg( cttime, dirty );
}
void dtg::setDate( const std::string &d, DTG dstyle )
{
   std::string temp;
   if ( d.length() == 0 || !d[ 0 ] || ( d[ 0 ] == ' ' ) || ( d[ 0 ] == '/' )
        || ( ( dstyle != DTGLOG ) && ( dstyle != DTGReg1Test ) && ( ( d[ 2 ] != '/' ) || ( d[ 5 ] != '/' )
              || d.length() != 8 ) ) )
   {
      temp = "      ";
   }
   else
      if ( dstyle == DTGDISP )
      {
         temp = d[ 6 ];
         temp += d[ 7 ];
         temp += d[ 3 ];
         temp += d[ 4 ];
         temp += d[ 0 ];
         temp += d[ 1 ];
      }
      else // LOG or Reg1Test
      {
         temp = d.substr( 0, 6 );
      }
   sdate.setValue( temp );
}

void dtg::setTime( const std::string &t, DTG dstyle )
{
   std::string temp;
   if ( t.length() == 0 || ( !t[ 0 ] ) || ( t[ 0 ] == ' ' ) || ( t[ 0 ] == ':' ) )
   {
      temp = "    ";
   }
   else
      if ( dstyle == DTGDISP )
      {
         std::string t2 = t + ":00:00:00";
         temp = t2[ 0 ];
         temp += t2[ 1 ];

         temp += t2[ 3 ];
         temp += t2[ 4 ];

         temp += t2[ 6 ];
         temp += t2[ 7 ];
      }
      else   // Log or Reg1Test (which should be a 4 char time)
      {
         std::string t2 = t + "000000";
         temp = t2.substr( 0, 6 );
      }
   stime.setValue( temp );
}
void dtg::setDtg( time_t cttime )
{
   struct tm * tms = localtime( &cttime );  // NB static data area!
   if ( tms->tm_isdst != 0 )
      tms->tm_hour -= 1;

   std::string curTime;
   std::string curDate;

   curTime = ( boost::format( "%02.2d:%02.2d:%02.2d" ) % tms->tm_hour % tms->tm_min % tms->tm_sec ).str();

   int year = tms->tm_year;
   while ( year > 99 )
      year -= 100;			// take it back to two chars

   curDate = ( boost::format( "%02.2d/%02.2d/%02.2d" ) % tms->tm_mday % ( tms->tm_mon + 1 ) % year ).str();
   setTime( curTime, DTGDISP );
   setDate( curDate, DTGDISP );
}
int dtg::notEntered( void )
{
   int i;
   bool te = false;
   bool de = false;
   std::string temp_date = getDate( DTGDISP );
   std::string temp_time = getTime( DTGDISP );

   for ( i = 0; i < DATELENGTH; i++ )
   {
      if ( !temp_date[ i ] )
         break;

      if ( temp_date[ i ] != ' ' && temp_date[ i ] != '/' )
      {
         de = true;
         break;
      }
   }
   for ( i = 0; i < TIMELENGTH; i++ )
   {
      if ( !temp_time[ i ] )
         break;

      if ( temp_time[ i ] != ' ' && temp_time[ i ] != ':' )
      {
         te = true;
         break;
      }
   }
   if ( !de && !te )
      return 0;   // neither entered, will fill in when cs entered

   if ( !de )
      return 1;   // time, no date

   if ( !te )
      return 2;   // date, no time

   return -1;     // both entered
}
// default versions are good enough for now!
//dtg::dtg(const dtg&)
//{
//}
//dtg& dtg::operator =(const dtg&)
//{
//}
dtg::~dtg()
{}
//============================================================
callsign::callsign( ) : valRes( CS_NOT_VALIDATED )
{}
callsign::callsign( const std::string &pcs ) : valRes( CS_NOT_VALIDATED )
{
   fullCall.setValue( pcs );
}
// default versions are good enough for now!
//callsign::callsign(const callsign&)
//{
//}
//callsign& callsign::operator =(const callsign&)
//{
//}
callsign::~callsign()
{
   // nothing needed
}
char callsign::validate( )
{
   if ( valRes != CS_NOT_VALIDATED )
      return valRes;

   // NB that leading spaces may be genuine wildcards, and so may
   // actually be significant; we should reject as invalid
   // calls with leading spaces, even though this isn't very friendly

   TEMPBUFF( csv, CALLSIGNLENGTH + 1 );
   strncpy( csv, fullCall.getValue().c_str(), CALLSIGNLENGTH );          // set up a modifiable buffer
   csv[ CALLSIGNLENGTH ] = 0;
   trimr( csv );

   valRes = ERR_NOCS;
   if ( csv[ 0 ] == 0 )   			// all spaces
      return valRes;

   // first check no funny characters; only alphanum and / allowed
   // (spaces are now invalid)

   valRes = ERR_INVCS;

   int cslen = strlen( csv );
   for ( int i = 0; i < cslen; i++ )
   {
      if ( ( csv[ i ] != '/' ) && ( !isalnum( csv[ i ] ) ) )
      {
         return valRes;
      }
   }

   prefix = "";
   prefix2 = "";
   suffix = "";
   number = "";
   body = "";

   const char *f = csv;

   char *spt = 0;          // CEPT separator
   char *spt2 = 0;         // suffix separator

   char *cse = &csv[ cslen ];  // zero termination

   if ( cse - f < 3 )
   {
      return valRes;   // cs too short
   }

   // find any separators

   spt = strchr( csv, '/' );
   if ( spt )
   {
      *spt = 0;
      spt++;
      spt2 = strchr( spt, '/' );
      if ( cse - spt <= 0 )
         return valRes; // nothing after end
      if ( spt == f )
         return valRes; // nothing before first /

      if ( spt2 )
      {
         *spt2 = 0;
         spt2++;

         if ( strchr( spt2, '/' ) )
            return valRes; // too many separators

         if ( spt2 >= cse )
            return valRes; // nothing after end

         if ( spt2 - spt < 3 )
            return valRes;   // too little in the middle
      }

   }
   if ( spt && spt2 )
   {
      // two separators.
      // Usually F/G0GJV/P
      //What about F0ABC/RVI/P The /RVI is a country...

      if ( spt2 - spt <= TRAILBITLENGTH + 1 )   	// assume the RVI case, allow for the '/'
      {
         prefix = "/";
         prefix += std::string( spt ).substr( 1, BITLENGTH );
         spt = csv;			// main body is at the start
      }
      else
      {
         // current country prefix at the start
         prefix = std::string( spt ).substr( 0, BITLENGTH );
      }
      suffix = std::string( spt2 ).substr( 0, TRAILBITLENGTH );
   }
   else
      if ( spt && ( cse - spt <= TRAILBITLENGTH ) )
      {
         // tail shorter than 4 chars, therefor e.g. /p or /mm - or /RVI
         // we will eventually search for mults on suffix first then prefix
         suffix = std::string( spt ).substr( 0, TRAILBITLENGTH );
         *spt = 0;
         spt = csv;
      }
      else
         if ( spt )
         {
            // long tail, must be normal prefix only, no suffix

            prefix = std::string( csv ).substr( 0, BITLENGTH );
         }
         else
         {
            spt = csv;
         }

   // spt now points to main callsign, no CEPT/, no /P
   // suffix and prefix are set up

   f = spt;
   int j = 0;

   while ( isdigit( *f ) && ( j < BITLENGTH ) )     // leading digits (2 of 2E0...)
   {
      prefix2 += *f++;
   }

   while ( *f && ( prefix2.length() < BITLENGTH ) )           // prefix letters (E of 2E0)
   {
      if ( isdigit( *f ) )
      {
         break ;
      }
      else
      {
         prefix2 += *f++;
      }
   }

   j = 0;

   while ( isdigit( *f ) && ( number.length() < NUMBITLENGTH ) )     // central number (0 of 2E0)
      number += *f++;

   // main CS letters
   body = std::string( f ).substr( 0, BITLENGTH );

   if ( prefix.length() == 0 )
   {
      prefix = prefix2.substr( 0, BITLENGTH );   // prefix is country of location
      // prefix2 is country of issue
   }

   valRes = ERR_CSDIGIT;
   if ( number.length() == 0 )
      return valRes;

   // trim any numbers off tail of prefix (CEPT)

   // But with e.g. HB0 and HB9 both being new countries, HB0/G0GJV is valid(?)
   // So I think that we have to trust the user(YUCK!)

   // Replace real prefix2 with the dup check prefix2 - for dup checking
   // Need to implement the G list here...
   CountrySynonym *syn = MultLists::getMultLists() ->searchCountrySynonym( prefix2 );
   if ( syn )
   {
      syn->getDupPrefix( prefix2 );
   }

   // We maybe ought to do the WGV trick here - apart from /P, etc the shortest
   // part of prefix/suffix is the country of operation
   // But I think we have it reasonably correct... until someone comes up
   // with a counter-example

   valRes = CS_OK;
   if ( ( prefix.length() == 0 ) || ( prefix2.length() == 0 ) || ( number.length() == 0 )
        || ( body.length() == 0 ) )   	// suffix can be null
   {
      valRes = ERR_INVCS;
   }

   return ( valRes );
}
//============================================================

