/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "MinosEventManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
MinosEventManager & MinosEventManager::GetMinosEventManager ( void )
{
	static MinosEventManager EM;
	return EM;
}
//---------------------------------------------------------------------------

