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

#ifndef UTF8ConversionH
#define UTF8ConversionH 
// Utility functions for UTF-8


extern std::string AsciiToUTF8( const std::string &s );
extern std::string AsciiFromUTF8( const std::string &s );
extern std::string AsciiFromUTF16( const std::wstring &s );

extern std::string UTF16ToUTF8( const std::wstring &s );
extern std::wstring UTF16FromUTF8( const std::string &s );
extern std::wstring UTF16FromAscii( const std::string &s );
#endif
