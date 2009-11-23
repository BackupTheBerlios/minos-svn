/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "XMPP_pch.h"
#pragma hdrstop

#include "TinyUtils.h"

bool checkElementName( TiXmlElement *e, const std::string &expected )
{
   if ( !stricmp( e->Value(), expected.c_str() ) )
      return true;
   return false;
}

std::string getAttribute( TiXmlElement *tix, const char *attname )
{
   std::string attval;
   const char * att = tix->Attribute( attname );
   if ( att )
   {
      attval = att;
   }
   return attval;
}

