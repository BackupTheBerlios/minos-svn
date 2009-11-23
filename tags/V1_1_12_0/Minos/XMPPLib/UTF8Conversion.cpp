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
#include "XMPP_pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------
// All return the number of bytes in the string to be returned (including terminator)
// If output param is zero, then the number of bytes required will be returned

/*
extern int ASCIIToUTF8(const char *input, char *output, int oplen);
extern int UTF8ToASCII(const char *input, char *output, int oplen);
extern int ASCIIToUTF16(const char *input, wchar_t *output, int oplen);
extern int UTF16ToASCII(const wchar_t *input, char *output, int oplen);
extern int UTF16ToUTF8(const wchar_t *input, char *output, int oplen);
extern int UTF8ToUTF16(const char *input, wchar_t *output, int oplen);
*/
int ASCIIToUTF8( const char *input, char *output, int oplen )
{
   //The MultiByteToWideChar function maps a character string to a wide-character (Unicode) string.
   //The character string mapped by this function is not necessarily from a multibyte character set.
   /*
   int MultiByteToWideChar(
     UINT CodePage,         // code page - CP_UTF8
     DWORD dwFlags,         // character-type options - 0
     LPCSTR lpMultiByteStr, // string to map
     int cbMultiByte,       // number of bytes in string
     LPWSTR lpWideCharStr,  // wide-character buffer
     int cchWideChar        // size of buffer
   );

   int WideCharToMultiByte(
     UINT CodePage,            // code page
     DWORD dwFlags,            // performance and mapping flags
     LPCWSTR lpWideCharStr,    // wide-character string
     int cchWideChar,          // number of chars in string
     LPSTR lpMultiByteStr,     // buffer for new string
     int cbMultiByte,          // size of buffer
     LPCSTR lpDefaultChar,     // default for unmappable chars
     LPBOOL lpUsedDefaultChar  // set when default char used
   );
   */
   int utf16len = MultiByteToWideChar( CP_ACP, 0, input, -1, 0, 0 );
   wchar_t *op = new wchar_t[ utf16len + 1 ];
   MultiByteToWideChar( CP_ACP, 0, input, -1, op, utf16len );
   op[ utf16len ] = 0;

   int utf8len = WideCharToMultiByte( CP_UTF8, 0, op, utf16len, 0, 0, 0, 0 );
   if ( output && oplen > 0 )
   {
      char * utf8 = new char[ utf8len + 1 ];
      WideCharToMultiByte( CP_UTF8, 0, op, utf16len, utf8, std::min( oplen, utf8len ), 0, 0 );
      utf8len = std::min( oplen, utf8len + 1 );
      memcpy( output, utf8, utf8len );
      output[ utf8len - 1 ] = 0;
      delete [] utf8;
   }
   delete [] op;
   return utf8len;
}
int UTF8ToASCII( const char *input, char *output, int oplen )
{
   int utf16len = MultiByteToWideChar( CP_UTF8, 0, input, -1, 0, 0 );
   wchar_t *op = new wchar_t[ utf16len + 1 ];
   MultiByteToWideChar( CP_UTF8, 0, input, -1, op, utf16len );
   op[ utf16len ] = 0;

   int asciilen = WideCharToMultiByte( CP_ACP, 0, op, utf16len, 0, 0, 0, 0 );
   if ( output && oplen > 0 )
   {
      char * ascii = new char[ asciilen + 1 ];
      WideCharToMultiByte( CP_ACP, 0, op, utf16len, ascii, std::min( oplen, asciilen ), 0, 0 );
      asciilen = std::min( oplen, asciilen );
      memcpy( output, ascii, asciilen );
      output[ asciilen - 1 ] = 0;
      delete [] ascii;
   }
   delete [] op;
   return asciilen;
}
int ASCIIToUTF16( const char *input, wchar_t *output, int oplen )
{
   if ( oplen == 0 )
   {
      int utf16len = MultiByteToWideChar( CP_ACP, 0, input, -1, 0, 0 );
      return utf16len + 1;
   }
   if ( output && oplen > 0 )
   {
      return MultiByteToWideChar( CP_ACP, 0, input, -1, output, oplen );
   }
   return 0;
}
int UTF16ToASCII( const wchar_t *input, char *output, int oplen )
{
   if ( oplen == 0 )
   {
      int asciilen = WideCharToMultiByte( CP_ACP, 0, input, -1, 0, 0, 0, 0 );
      return asciilen;
   }
   if ( output && oplen > 0 )
   {
      return WideCharToMultiByte( CP_ACP, 0, input, -1, output, oplen, 0, 0 );
   }
   return 0;
}
int UTF16ToUTF8( const wchar_t *input, char *output, int oplen )
{
   if ( oplen == 0 )
   {
      int UTF8len = WideCharToMultiByte( CP_UTF8, 0, input, -1, 0, 0, 0, 0 );
      return UTF8len;
   }
   if ( output && oplen > 0 )
   {
      return WideCharToMultiByte( CP_UTF8, 0, input, -1, output, oplen, 0, 0 );
   }
   return 0;
}
int UTF8ToUTF16( const char *input, wchar_t *output, int oplen )
{
   if ( oplen == 0 )
   {
      int utf16len = MultiByteToWideChar( CP_UTF8, 0, input, -1, 0, 0 );
      return utf16len;
   }
   if ( output && oplen > 0 )
      return MultiByteToWideChar( CP_UTF8, 0, input, -1, output, oplen );
   return 0;
}


std::string AsciiToUTF8( const std::string &s )
{
   int len = ASCIIToUTF8( s.c_str(), 0, 0 );
   char *op = new char[ len + 1 ];
   ASCIIToUTF8( s.c_str(), op, len );
   op[ len ] = 0;
   std::string os( op );
   delete [] op;
   return os;
}
std::string AsciiFromUTF8( const std::string &s )
{
   int len = UTF8ToASCII( s.c_str(), 0, 0 );
   char *op = new char[ len + 1 ];
   UTF8ToASCII( s.c_str(), op, len );
   op[ len ] = 0;
   std::string os( op );
   delete [] op;
   return os;
}
std::string AsciiFromUTF16( const std::wstring &s )
{
   int len = UTF16ToASCII( s.c_str(), 0, 0 );
   char *op = new char[ len + 1 ];
   UTF16ToASCII( s.c_str(), op, len );
   op[ len ] = 0;
   std::string os( op );
   delete [] op;
   return os;
}
std::string UTF16ToUTF8( const std::wstring &s )
{
   int len = UTF16ToUTF8( s.c_str(), 0, 0 );
   char *op = new char[ len + 1 ];
   UTF16ToUTF8( s.c_str(), op, len );
   op[ len ] = 0;
   std::string os( op );
   delete [] op;
   return os;
}
std::wstring UTF16FromUTF8( const std::string &s )
{
   int len = UTF8ToUTF16( s.c_str(), 0, 0 );
   wchar_t *op = new wchar_t[ len + 1 ];
   UTF8ToUTF16( s.c_str(), op, len );
   op[ len ] = 0;
   std::wstring os( op );
   delete [] op;
   return os;
}
std::wstring UTF16FromAscii( const std::string &s )
{
   int len = UTF8ToUTF16( s.c_str(), 0, 0 );
   wchar_t *op = new wchar_t[ len + 1 ];
   ASCIIToUTF16( s.c_str(), op, len );
   op[ len ] = 0;
   std::wstring os( op );
   delete [] op;
   return os;
}

