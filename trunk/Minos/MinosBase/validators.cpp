/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#include "base_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------

#pragma package(smart_init)

ErrEntry errDefs[] =
   {
      ErrEntry( 20, "null validator failed" ),   //ERR_0
      ErrEntry( 11, "Invalid time" ),        //ERR_1
      ErrEntry( 10, "Invalid date" ),        //ERR_2
      ErrEntry( 9, "Invalid power(0.01 - 1000 W)" ),      //ERR_3
      ErrEntry( 15, "Invalid locator" ),     //ERR_4
      ErrEntry( 14, "Invalid Serial" ),      //ERR_5
      ErrEntry( 13, "Invalid RS(T)" ),       //ERR_6
      ErrEntry( 9, "Invalid power(-40 - +40 dBW)" ),      //ERR_7
      ErrEntry( 10, "Invalid district" ),      //ERR_8
      ErrEntry( 10, "Invalid band" ),        //ERR_9
      ErrEntry( 12, "!!! CS not validated !!!" ),   //ERR_10
      ErrEntry( 12, "!!! No CS present !!!" ),   //ERR_11
      ErrEntry( 1, "!!! Duplicate Callsign !!!" ),   //ERR_12
      ErrEntry( 8, "Invalid Callsign!" ),         //ERR_13
      ErrEntry( 10, "No RX Serial Number" ),      //ERR_14
      ErrEntry( 10, "" ),   //ERR_15
      ErrEntry( 10, "" ),     //ERR_16
      ErrEntry( 10, "" ),   //ERR_17
      ErrEntry( 20, "No Locator" ),             //ERR_18
      ErrEntry( 2, "!!Loc not AA00AA to RR99XX!!" ),   //ERR_19
      ErrEntry( 16, "Invalid district multiplier" ),          //ERR_20
      ErrEntry( 10, "QTH required" ),           //ERR_21
      ErrEntry( 10, "" ),         //ERR_22
      ErrEntry( 5, "Unknown Country" ),         //ERR_23
      ErrEntry( 2, "!!! Non Scoring Contact !!!" ),         //ERR_24
      ErrEntry( 1, "!!! Comment Only !!!" ),         //ERR_25
      ErrEntry( 1, "!!! DELETED !!!" ),         //ERR_26
      ErrEntry( 90, "" ),         //ERR_27
      ErrEntry( 90, "" ),         //ERR_28
      ErrEntry( 0, "" )          //ERR_29
   };
void setError( int err )   // No CS
{
   MinosLoggerEvents::SendValidateError(err);
}

Validator::Validator( validatorTypes vt ) : vt( vt ), status( false )
{}
//enum validTypes {cmNone, cmCancel, cmValid, cmCheckValid, cmValidStatus, cmReleasedFocus};
//enum validatorTypes {vtNone, vtNotEmpty, vtNumeric, vtDate, vtTime, vtCallsign,
//                      vtSN, vtRST, vtLoc, vtQTH, vtComments};
bool Validator::validate( String str )
{
   switch ( vt )
   {
      case vtNone:
         return true;

      case vtNotEmpty:
         return !allSpaces( str.c_str() );

      case vtNumeric:
         return validNumber( str.c_str(), false, true );

      case vtDate:
         {
            String field = str.Trim();
            bool res = true;
            if ( field.Length() != 8 )
               res = false;
            else
               if ( ( field[ 1 ] != '0' ) && ( field[ 1 ] != '1' ) && ( field[ 1 ] != '2' ) && ( field[ 1 ] != '3' ) )
                  res = false;
               else
                  if ( !isdigit( field[ 2 ] ) )
                     res = false;
                  else
                     if ( field[ 3 ] != '/' )
                        res = false;
                     else
                        if ( ( field[ 4 ] != '0' ) && ( field[ 4 ] != '1' ) )
                           res = false;
                        else
                           if ( !isdigit( field[ 5 ] ) )
                              res = false;
                           else
                              if ( field[ 6 ] != '/' )
                                 res = false;
                              else
                                 if ( !isdigit( field[ 7 ] ) )
                                    res = false;
                                 else
                                    if ( !isdigit( field[ 8 ] ) )
                                       res = false;
                                    else
                                       return true;   // seems to match

            if ( !res )
               setError( ERR_2 );
            return false;
         }

      case vtTime:
         {
            bool res = true;
            String field = str.Trim();
            if ( field.Length() != 5 )
               res = false;
            else
               if ( ( field[ 1 ] != '0' ) && ( field[ 1 ] != '1' ) && ( field[ 1 ] != '2' ) )
                  res = false;
               else
                  if ( !isdigit( field[ 2 ] ) )
                     res = false;
                  else
                     if ( field[ 3 ] != ':' )
                        res = false;
                     else
                        if (
                           ( field[ 4 ] != '0' ) && ( field[ 4 ] != '1' ) &&
                           ( field[ 4 ] != '2' ) && ( field[ 4 ] != '3' ) &&
                           ( field[ 4 ] != '4' ) && ( field[ 4 ] != '5' )
                        )
                           res = false;
                        else
                           if ( !isdigit( field[ 5 ] ) )
                              res = false;
                           else
                              return true;
            if ( !res )
               setError( ERR_1 );
            return false;
         }

      case vtCallsign:
         if ( allSpaces( str.c_str() ) )
         {
            setError( ERR_11 ); // No CS
            return false;
         }
         return true;

      case vtSN:
         if ( validNumber( str.c_str(), false, true ) )
         {
            return true;
         }
         else
            setError( ERR_5 );
         return false;

      case vtRST:
         if ( validateRST( str ) )
            return true;
         else
            setError( ERR_6 );
         return false;

      case vtLoc:
         {
            bool PXret = true;
            String loc = str + "        ";
            bool allowLoc8 = MinosParameters::getMinosParameters() ->getAllowLoc8();
            if ( !allowLoc8 && ( isdigit( loc[ 6 ] ) || isdigit( loc[ 7 ] ) ) )
               PXret = false;

            double longitude = 0.0;
            double latitude = 0.0;

            if ( PXret && lonlat( loc.c_str(), longitude, latitude ) != LOC_OK )
               PXret = false;

            if ( PXret == false )
               setError( allSpaces( str.c_str() ) ? ERR_18 : ERR_4 );
            return PXret;
         }
      case vtQTH:
         {
            // We need a QTH if (a) QTH required or (b) dist mult and NOT a non-G callsign
            /*
            LoggerContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
            if (ct->otherExchange.getValue() && trim(str).size() == 0)
            {
               return false;
            }
            if (ct->districtMult.getValue())
            {
             && trim(str).size() == 0)
            }
            */ 
            return true;
         }

      case vtComments:
         return true;
   }
   return false;
}
bool Validator::allSpaces( const char *str )
{
   int i;
   int n = strlen( str );
   for ( i = 0; i < n && ( str[ i ] == ' ' ); i++ )
   {}
   if ( i >= n )
      return true;

   return false;
}
bool Validator::validNumber( const char *str, bool zallowed, bool zerror )
{
   int i = 0;
   int maxi = strlen( str );
   bool num_found = false;
   bool space_found = false;
   bool NonZ = false;

   for ( i = 0; i < maxi; i++ )
   {
      if ( ( num_found ) && ( str[ i ] == ' ' ) )
      {
         space_found = true;
         continue;
      }

      if ( isdigit( str[ i ] ) )
      {
         if ( str[ i ] != '0' )
            NonZ = true;
         if ( space_found )
         {
            return false;
         }
         num_found = true;
      }
   }
   if ( num_found && ( NonZ || ( zallowed && !zerror ) ) )
      return true;
   return false;
}
/*static*/bool Validator::validateRST(  String t )
{
   int l = t.Length();
   bool picvalid = false;
   if ( l >= 1 )
   {
      if ( strchr( " 54321", t[ 1 ] ) )
      {
         if ( l >= 2 )
         {
            if ( strchr( " 987654321", t[ 1 ] ) )
            {
               if ( l >= 3 )
               {
                  if ( strchr( " 987654321A", t[ 1 ] ) )
                  {
                     picvalid = true;
                  }
               }
               else
               {
                  picvalid = true;
               }
            }
         }
      }
   }
   if ( picvalid )
      picvalid = validNumber( t.c_str(), false, true );
   if ( picvalid )
   {
      // must be more than one digit in a RST
      int rst = atoi( t.c_str() );
      if ( rst < 10 )
         picvalid = false;

      // allow strength 0 to be typed, but it is still an error!

      if ( t[ 2 ] == '0' )
         picvalid = false;
   }
   return picvalid;
}
//==============================================================================
ValidatedControl::ValidatedControl( TLabeledEdit *c, validatorTypes vt ) :
      wc( c ), tIfValid( false ), validator( vt )
{}
bool ValidatedControl::valid( validTypes cmd )
{
   if ( wc->Visible )
   {
      /*
            if ( cmd == cmValid )
               return (validator.GetStatus());
            else
      */
      if ( cmd == cmValidStatus )
      {
         return tIfValid;
      }
      else //if ( cmd != cmCancel )
      {
         if ( ! validator.validate( wc->Text ) )
         {
            tIfValid = false;
            return true; //( cmd == cmCheckValid ); // lie about it, or it stops on firstthat
         }
         tIfValid = true;  // for now, anyway!
      }
   }
   return true;
}

