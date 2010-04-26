/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "display_pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)

GridColumn QSOTreeColumns[ LOGTREECOLS ] =
   {
      GridColumn( egTime, "XXXXXX", "UTC", taLeftJustify ),               // time
      GridColumn( egCall, "MMMMMMMMMMM", "Callsign", taLeftJustify ),         // call
      GridColumn( egRSTTx, "599XXX", "RepTx", taLeftJustify ),                 // RST
      GridColumn( egSNTx, "1234X", "SnTx", taLeftJustify /*taRightJustify*/ ),   // serial
      GridColumn( egRSTRx, "599XXX", "RepRx", taLeftJustify ),                 // RST
      GridColumn( egSNRx, "1234X", "SnRx", taLeftJustify /*taRightJustify*/ ),   // Serial
      GridColumn( egLoc, "MM00MM00X", "Loc", taLeftJustify ),            // LOC
      GridColumn( egBrg, "3601X", "brg", taLeftJustify ),                // bearing
      GridColumn( egScore, "12345XX", "dist", taLeftJustify /*taRightJustify*/ ),  // score
      GridColumn( egExchange, "XXXXXXXXXXXXXXXX", "Exchange", taLeftJustify ),    // QTH
      GridColumn( egComments, "XXXXXXXXXXXXXXXX", "Comments", taLeftJustify )     // comments
   };

