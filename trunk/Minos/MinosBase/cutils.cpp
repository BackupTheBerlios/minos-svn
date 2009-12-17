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

bool checkFileOK( std::ifstream &istr, const std::string &fname, const std::string &fmess )
{
   if ( !istr )
   {
      std::string ebuff = ( boost::format( "Failed to open %s (%s)" ) % fmess % fname ).str();
      char *emess = _strerror( ebuff.c_str() );
      MinosParameters::getMinosParameters() ->mshowMessage( emess );
      return false;
   }
   return true;
}
char diskBuffer[ bsize + 1 ];
//char *lbuff = &diskBuffer[ 0 ];
int buffpt = 0;

void clearBuffer( void )
{
   memset( diskBuffer, '#', bsize );
   diskBuffer[ bsize ] = 0;
}
static char *noeditstr = "\r\n!!BINARY FILE DO NOT EDIT!!\r\n";
const int noeditlength = 32;

void strtobuf( const std::string &str )
{
   buffpt += strcpysp( &diskBuffer[ buffpt ], str, str.length() ) + 1;
}
void strtobuf( const MinosItem<std::string> &str )
{
   strtobuf( str.getValue() );
}
void strtobuf()
{
   char * s1 = &diskBuffer[ buffpt ];

   // null fill the rest of the buffer
   memset( s1, 0, bsize - buffpt );
   if ( buffpt + noeditlength < ( int ) ( bsize - 3 ) )
   {
      memcpy( &diskBuffer[ bsize - noeditlength - 1 ], noeditstr, noeditlength );
   }
   diskBuffer[ bsize - 1 ] = 0;   // force termination
}
void opyn( bool b )
{
   strtobuf( b ? "Y" : "N" );
}
void opyn( const MinosItem<bool> &b )
{
   opyn( b.getValue() );
}

void buftostr( std::string &str )
{
   int i;
   str = "";
   int len = strlen( &diskBuffer[ buffpt ] );
   for ( i = 0; i < 1024 && i < len; i++ )
   {
      str += diskBuffer[ buffpt + i ];
   }
   buffpt += len + 1;   // step past null terminator
}
void buftostr( MinosItem<std::string> &str )
{
   std::string temp;
   buftostr( temp );
   str.setInitialValue( temp );
}
bool inyn()
{
   std::string temp;
   buftostr( temp );
   if ( temp.size() && toupper( temp[ 0 ] ) == 'Y' )
      return true;

   return false;
}
/*
void *mymemcpy(void *dest, const void *src, size_t n)
{
	void *vret = memcpy(dest, src, n);
   ((char *)dest)[n] = 0;
   return vret;
}
 
char *mystrncpy(char *s1, const char *s2, int maxlen)
{
	char *ret = strncpy(s1, s2, maxlen);
   s1[maxlen] = 0;
   return ret;
}
*/
int strcpysp( std::string &s1, const std::string &ss2, int maxlen )
{
   char * temp = new char[ maxlen + 1 ];
   int ret = strcpysp( temp, ss2, maxlen );
   s1 = temp;
   delete [] temp;
   return ret;
}
int strcpysp( char *s1, const std::string &ss2, int maxlen )
{
   int i = 0;
   const char *s2 = ss2.c_str();

   if ( ss2.length() )
   {
      for ( i = 0; i < maxlen && s2[ i ]; i++ )
      {}

      // step past leading spaces

      while ( ( i > 0 ) && ( *s2 == ' ' ) )
      {
         i--;
         s2++;
      }

      // step back past trailing spaces
      while ( ( i > 0 ) && ( ( s2[ i - 1 ] == 0 ) || ( s2[ i - 1 ] == ' ' ) ) )
      {
         i--;
      }

      if ( i > 0 )
      {
         memcpy( s1, s2, i );
      }
      else
      {
         i = 0;
      }
   }
   s1[ i ] = 0;

   return i;
}

//============================================================

int strcmpsp( const std::string &s1, const std::string &s2 )
{
   TEMPBUFF( sp1, 256 );
   TEMPBUFF( sp2, 256 );

   strcpysp( sp1, s1, 255 );
   strcpysp( sp2, s2, 255 );

   return strcmp( sp1, sp2 );
}
//============================================================
int stricmpsp( const std::string &s1, const std::string &s2 )
{
   TEMPBUFF( sp1, 256 );
   TEMPBUFF( sp2, 256 );

   strcpysp( sp1, s1, 255 );
   strcpysp( sp2, s2, 255 );

   return stricmp( sp1, sp2 );
}
//============================================================

int placestr( char *buff, const std::string &str, int start, int len )
{
   // if len is -ve, it means to R justify
   int i = 0;
   int j = 0;

   if ( len > 0 )
   {
      for ( j = start; j < start + len; j++ )
      {
         buff[ j ] = ( str.length() == 0 || str[ i ] == 0 ) ? ' ' : str[ i++ ];
      }
   }
   else
   {
      len = -len;
      char *buff2 = new char[ 256 ];
      i = strcpysp( buff2, str.c_str(), len );	// strip spaces, return length
      if ( i > 0 )
      {
         j = len - i;
         if ( j < 0 )
            j = 0;

         memcpy( &buff[ start + j ], buff2, i );
      }
      delete [] buff2;
   }
   return start + len;
}
//      int scnt = parseLine( buffer, '=', a, 2, 0, sep2seen );
int parseLine( char *buff, char sep, char **a, int count, char sep2, bool &sep2seen )
{
   int i = 0;
   int sep_count = 0;
   sep2seen = false;

   int len = strlen( buff );
   for ( int j = 0; j < count; j++ )
   {
      // do it this way so we strip spaces off the start of every element,
      // including the first

      if ( j != 0 )
      {
         // terminate the previous entry on a '<sep>'
         while ( i < len && buff[ i ] && buff[ i ] != sep && buff[ i ] != sep2 )
            i++;

         if ( buff[ i ] == sep || ( sep2 != 0 && buff[ i ] == sep2 ) )
         {
            if ( buff[ i ] == sep2 )
               sep2seen = true;
            sep_count++;
            buff[ i++ ] = 0;
         }
      }
      while ( ( i < len ) && buff[ i ] && ( buff[ i ] == ' ' ) )
         i++;

      a[ j ] = &buff[ i ];
   }
   return sep_count;
}
writer::writer( HANDLE f ) :  /*lbuff( diskBuffer ),*/ expfd( f )
{}
writer::~writer()
{}
/*
void writer::lwrite( void )
{
   lwrite(lbuff);
 
}
*/
void writer::lwrite( const char *b )
{
   std::string l = std::string( b ) + "\r\n";
   DWORD ret = 0;
   bool res = WriteFile( expfd, l.c_str(), l.size(), &ret, 0 );
   if ( !res || ret != l.size() )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
   }
}
void writer::lwriteLine()
{
   std::string l( 80, 0x5F );					// horizontal line
   lwrite( l.c_str() );
}
void writer::lwriteNl()
{
   //   ::write( expfd, "\r\n", 2 );
   DWORD ret = 0;
   bool res = WriteFile( expfd, "\r\n", 2, &ret, 0 );
   if ( !res || ret != 2 )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
   }
}
void writer::lwriteFf()
{
   //   ::write( expfd, "\f", 1 );
   DWORD ret = 0;
   bool res = WriteFile( expfd, "\f", 1, &ret, 0 );
   if ( !res || ret != 1 )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
   }
}
// wild card comparison, search string e for the wild card string in s
// At the moment we are using "space" as the wildcard.
// we always scan down s for the first char in e
bool wildComp( const std::string &ss, const std::string &ee )
{
   const char * s = ss.c_str();
   const char *e = ee.c_str();
   while ( *s == ' ' )
      s++;
   if ( !*s )
      return false;
   while ( *e == ' ' )
      e++;
   if ( !*e )
      return false;


   const char *estart = e;

   // scan for first char of e in s

   const char *sstart = s;	// where to restart search

   while ( *sstart )
   {
      s = sstart;		// position moving pointer
      e = estart;		// go back to the start of the searching string

      while ( *s && *e && ( *s != *e ) )
         s++;
      if ( !*s )
         return false;		// s has ended without a match on char 1 of e

      sstart = ++s;			// next time start one on from this match
      e++;						// first char has matched
      // now attempt to match
      while ( *s && *e )
      {
         if (
            ( *s == *e )
            || ( ( *e == ' ' ) || ( *e == '*' ) || ( *e == '?' ) )
         )
         {
            s++;
            e++;
            continue;
         }
         break;		// match failed, break out
      }
      if ( !*e )
         return true;		// we are at the end of the searching string, so matched

      // otherwise try again at next matching start char
   }
   return false;
}
//=============================================================================
std::string trimr( const std::string &s )
{
   int i;
   for ( i = s.length() - 1; i >= 0; i-- )
   {
      char c = s[i];
      if ( !iscntrl( c ) && !isspace( c ) )
         break;
   }
   std::string s2;
   if ( i >= 0 )
      s2 = s.substr( 0, i + 1 );
   return s2;
}
std::string trim( const std::string &s )
{
   std::string s2 = trimr( s );
   unsigned int i;
   for ( i = 0; i < s2.length(); i++ )
   {
      if ( !iscntrl( s[ i ] ) && !isspace( s[ i ] ) )
         break;
   }
   std::string s3 = s2.substr( i, s2.length() );
   return s3;
}
//=============================================================================
std::string makeADIFField( const std::string &fieldname, const std::string &content )
{
   std::string tag;
   int len = content.size();
   if ( len )
   {
      std::string buff = ( boost::format( "<%s:%d>" ) % fieldname % len ).str();
      tag = buff + content;
   }
   return tag;
}
std::string makeADIFField( const std::string &fieldname, int content )
{
   std::string buff = ( boost::format( "%d" ) % content ).str();
   return makeADIFField( fieldname, buff );
}
//=============================================================================
std::string strupr( const std::string &s )
{
   std::string s2;
   for ( unsigned int i = 0; i < s.length(); i++ )
   {
      s2 += toupper( s[ i ] );
   }
   //s = s2;
   return s2;
}
//=============================================================================
int stricmp( const std::string &s1, const std::string &s2 )
{
   if ( s2.length() == 0 )
      return -1;
   if ( s1.length() == 0 )
      return 1;
   return strcmpi( s1.c_str(), s2.c_str() );
}
//=============================================================================
int strnicmp( const std::string &s1, const std::string &s2, unsigned int len )
{
   return strncmpi( s1.c_str(), s2.c_str(), len );
}
//=============================================================================
TDateTime CanonicalToTDT( String cdtg )
{
   TDateTime d;
   try
   {
      String year = cdtg.SubString( 1, 4 );
      String month = cdtg.SubString( 5, 2 );
      String day = cdtg.SubString( 7, 2 );
      String hour = cdtg.SubString( 9, 2 );
      String min = cdtg.SubString( 11, 2 );

      d = TDateTime( year.ToIntDef( 0 ), month.ToIntDef( 0 ), day.ToIntDef( 0 ) );
      d += TDateTime( hour.ToIntDef( 0 ), min.ToIntDef( 0 ), 0, 0 );
   }
   catch ( EConvertError & e )
   {
      return TDateTime();
   }
   return d;

}
String TDTToCanonical( TDateTime d )
{
   String s = d.FormatString( "yyyymmddhhnn" );
   return s;
}
//=============================================================================
char *mystrncpy( char *s1, const char *s2, int maxlen )
{
   char * ret = strncpy( s1, s2, maxlen );
   s1[ maxlen ] = 0;
   return ret;
}
//=============================================================================






