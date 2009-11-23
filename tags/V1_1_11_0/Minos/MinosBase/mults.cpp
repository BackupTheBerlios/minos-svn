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
#include "MultsImpl.h"

MultLists *MultLists::getMultLists()
{
   return MultListsImpl::getMultLists();
}
MultLists::MultLists()
{}
MultLists::~MultLists()
{}

