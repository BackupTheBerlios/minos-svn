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

#include "..\MinosDisplay\TreeUtils.h" 
//---------------------------------------------------------------------------

#pragma package(smart_init)

std::string ListContact::getField( int ACol, const BaseContestLog *const curcon ) const
{
   // only used to get fields for main log display
   std::string res;
   if ( !curcon )
      return res;

   switch ( ACol )
   {
      case egCall:
         res = cs.fullCall.getValue();
         break;
      case egLoc:
         res = loc.loc.getValue();
         break;
      case egBrg:
         {
			AnsiString brgbuff;
            double lon = 0.0;
            double lat = 0.0;
            int brg;
            double dist;
            if ( lonlat( loc.loc.getValue(), lon, lat ) == LOC_OK )
            {
               // we don't have it worked out already...
               double lon = 0.0;
               double lat = 0.0;
               int brg;
               double dist;



               int lres = lonlat( loc.loc.getValue(), lon, lat );
               if ( lres == LOC_OK )
               {
                  curcon->disbear( lon, lat, dist, brg );
               }
               else
                  if ( lres == LOC_SHORT )
                  {
#warning  what here?

                  }
               brgbuff.printf( "%03.3d\xB0", varBrg( brg ) );
            }
            res = brgbuff.c_str();
         }
         break;

      case egScore:
         {
            AnsiString scorebuff;

            // we don't have it worked out already...
            double lon = 0.0;
            double lat = 0.0;
            int brg;
            double dist;

            if ( lonlat( loc.loc.getValue(), lon, lat ) == LOC_OK )
            {
               curcon->disbear( lon, lat, dist, brg );
            }
			scorebuff.printf( "%d", ( int ) dist );
            res = scorebuff.c_str();
         }
         break;

      case egExchange:
         {
            res = extraText;
         }
         break;

      case egComments:
         {
            res = comments;
         }
         break;
   }
   return res;
}

