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
//---------------------------------------------------------------------------

MinosParameters *MinosParameters::mp = 0;
MinosParameters *MinosParameters::getMinosParameters()
{
   if ( !mp )
   {
      ShowMessage( "MinosPararameters not initialised" );
   }
   return mp;
}

