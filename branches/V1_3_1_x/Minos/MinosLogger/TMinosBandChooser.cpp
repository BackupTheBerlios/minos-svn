/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop

#include "TMinosBandChooser.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMinosBandChooser *MinosBandChooser;
//---------------------------------------------------------------------------
__fastcall TMinosBandChooser::TMinosBandChooser( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
