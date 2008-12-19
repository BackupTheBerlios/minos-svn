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

//==========================================================================
DisplayContestContact::DisplayContestContact( BaseContestLog * ct, bool time_now )
      : BaseContact( ct, time_now ),
      logSequence( 0 ),
      modificationCount( 0 )
{
   BaseContestLog * clp = ct;
   if ( clp )
   {
      mode.setInitialValue( clp->mode.getValue() );
   }
   else
      ShowMessage( "Contact constructor with no contest" );

   int ms = 0;

   if ( clp )
   {
      ms = clp->maxSerial + 1;
   }

   serialr.setInitialValue( std::string( SERIALLENGTH, ' ' ) );
   repr.setInitialValue( "5  " );
   reps.setInitialValue( "5  " );
   clearDirty();         // is this correct? Or should we clear the serial as well
   char temp[ 10 ];
   sprintf( temp, "%03.3d", ms );
   serials.setValue( temp );
}
DisplayContestContact::~DisplayContestContact()
{}

void DisplayContestContact::copyFromArg( ScreenContact &cct )
{
   //   logSequence = cct.logSequence; // addContact or whatever will already have it correct
   loc.loc.setValue( cct.loc.loc );
   loc.validate();
   extraText.setValue( cct.extraText );

   cs.fullCall.setValue( cct.cs.fullCall );
   cs.validate();

   time.setValue( cct.time );

   reps.setValue( cct.reps );
   serials.setValue( cct.serials );
   repr.setValue( cct.repr );
   serialr.setValue( cct.serialr );

   QSOValid = cct.QSOValid;

   districtMult = cct.districtMult;
   ctryMult = cct.ctryMult;
   multCount = cct.multCount;
   forcedMult.setValue( cct.forcedMult );

   op1.setValue( cct.op1 );
   op2.setValue( cct.op2 );

   newLoc = cct.newLoc;
   newDistrict = cct.newDistrict;
   newCtry = cct.newCtry;

   comments.setValue( cct.comments );

   contactFlags.setValue( cct.contactFlags );

   contactScore.setValue( cct.contactScore );
   bearing = cct.bearing;
   mode.setValue( cct.mode );

}
// used to test if anything has changed

bool DisplayContestContact::ne( const ScreenContact &mct ) const
{
   std::string ne_temp_date = mct.time.getDate( DTGDISP );
   std::string ne_temp_time = mct.time.getTime( DTGDISP );

   if ( strcmpsp( ne_temp_date, time.getDate( DTGDISP ) ) )
      return true; // i.e. not equal

   if ( strcmpsp( ne_temp_time, time.getTime( DTGDISP ) ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.cs.fullCall.getValue(), cs.fullCall.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.reps, reps.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.serials, serials.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.repr, repr.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.serialr, serialr.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.reps, reps.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.loc.loc.getValue(), loc.loc.getValue() ) )
      return true; // i.e. not equal

   if ( stricmpsp( mct.extraText, extraText.getValue() ) )       // we force exchange upper case if dist code
      return true; // i.e. not equal

   if ( strcmpsp( mct.comments, comments.getValue() ) )
      return true; // i.e. not equal

   if ( mct.contactFlags != contactFlags.getValue() )
      return true; // i.e. not equal

   if ( strcmpsp( mct.mode, mode.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.forcedMult, forcedMult.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.op1.getValue(), op1.getValue() ) )
      return true; // i.e. not equal

   if ( strcmpsp( mct.op2.getValue(), op2.getValue() ) )
      return true; // i.e. not equal


   if ( ( mct.contactScore != contactScore.getValue() ) && ( contactScore.getValue() != -1 ) )
      return true;	// not equal

   return false;  // i.e. equal
}

void DisplayContestContact::check( )
{
   // check on country and district. If valid, return true,
   // having mapped any synonyms to their parents and
   // saved the pointers.

   int checkret = 0;
   double dist = 0.0;
   BaseContestLog * clp = contest;

   if ( contactFlags.getValue() & ( LOCAL_COMMENT | COMMENT_ONLY ) )
      return ;
   if ( contactFlags.getValue() & NON_SCORING )
      return ;

   QSOValid = false;             // initially, anyway
   int csret = cs.validate( );
   if ( csret != CS_OK )
      checkret = ERR_13;

   int index;
   if ( !checkret )
   {
      BaseContact * valp = 0;
      valp = clp->validationPoint;
      if ( clp->DupSheet.checkCurDup( this, valp, false ) )
      {
         cs.valRes = ERR_DUPCS;
         checkret = ERR_12;
      }
   }

   // search for prefix in country synonym list. Have to allow for e.g. HB0 as a mult

   if ( contactFlags.getValue() & COUNTRY_FORCED )
   {
      ctryMult = MultLists::getMultLists() ->getCtryForPrefix( forcedMult.getValue() );
   }
   else
      if ( !checkret )
      {
         ctryMult = findCtryPrefix( cs );
      }

   //   extraText = trimr( extraText );   // needs to be done already

   unsigned short cf = contactFlags.getValue();
   cf &= ~UNKNOWN_COUNTRY;
   if ( !checkret && ( clp->countryMult.getValue() || clp->districtMult.getValue() ) && !ctryMult )    // need at least a valid country
   {
      cf |= UNKNOWN_COUNTRY;
   }
   contactFlags.setValue( cf );
   if ( clp->districtMult.getValue() && ctryMult )
   {
      // if CC_mult and country "has districts" search for the "extra" in the county synonym list

      // check that the district and country agree

      // if the correct parts don't exist, not a valid contact!
      // NB that the rest of the contact has to be valid as well!

      if ( ctryMult->hasDistricts() )    // continentals dont have counties
      {
         districtMult = MultLists::getMultLists() ->searchDistrict( extraText.getValue() );
         if ( !districtMult && !( cf & VALID_DISTRICT ) )
         {
            checkret = ERR_8;
         }

         if (
            !checkret &&       						// no errors
            !( cf & VALID_DISTRICT ) &&      // ? district forced OK
            districtMult &&
            ( districtMult->country1 != ctryMult ) &&     // check district in country
            ( districtMult->country2 != ctryMult )
         )
         {
            checkret = ERR_8;
         }
      }
      // so all seems OK, or checkret is set to the first error
   }
   else
      if ( !checkret )
      {
         districtMult = 0;						// just in case we have changed the type...
         if ( clp->otherExchange.getValue() )
         {
            if ( clp->districtMult.getValue() )
            {
               if ( !trim( comments.getValue() ).size() )
                  checkret = ERR_21;
            }
            else
               if ( !trim( extraText.getValue() ).size() )
                  checkret = ERR_21;
         }
      }

   // calc the bearing and score anyway; otherwise dups get a bearing of -1

   if ( bearing < 0 )
   {
      double lon = 0.0;
      double lat = 0.0;
      int brg;

      if ( lonlat( loc.loc.getValue(), lon, lat ) == LOC_OK )
      {
         clp->disbear( lon, lat, dist, brg );
         bearing = brg;
      }
   }

   if ( checkret )
      return ;

   QSOValid = true;        // for now

   // same as ScreenContact::Check up to here

   if ( districtMult )
   {
      if ( districtMult->country1 &&
           ( ( clp->districtWorked[ districtMult->listOffset ] ) ++ < districtMult->country1->districtLimit() ) )
      {
         clp->ndistrict++;
         if ( clp->districtMult.getValue() )
         {
            multCount++;
            newDistrict = true;
         }
      }
   }

   if ( ctryMult )
   {
      if ( ( clp->countryWorked[ ctryMult->listOffset ] ) ++ == 0 )
      {
         clp->nctry++;
         if ( clp->countryMult.getValue() )
         {
            multCount++;
            newCtry = true;
         }
      }
   }
   clp->DupSheet.checkCurDup( this, 0, true ); // add to duplicates list

   if ( !( contactFlags.getValue() & ( MANUAL_SCORE | NON_SCORING | LOCAL_COMMENT | COMMENT_ONLY | DONT_PRINT ) ) )
   {
      if ( loc.loc.getValue().size() == 4 && clp->allowLoc4.getValue() )
      {
         dist = clp->CalcNearest( loc.loc.getValue() ); // deal with 4 char locs
      }
      contactScore.setValue( dist );
   }

   if ( !clp->locatorField.getValue() || contactScore.getValue() >= 0 )   		// don't add -1 scores in, but DO add zero km
      // as it is 1 point.
   {
      int cscore = contactScore.getValue();
      switch ( clp->scoreMode.getValue() )
      {
         case PPKM:
            {
               if ( contactFlags.getValue() & XBAND )
               {
                  cscore = ( cscore + 1 ) / 2;
               }
               clp->contestScore += cscore;
            }
            break;

         case PPQSO:
            if ( cscore > 0 )
               clp->contestScore++;
            break;

      }
   }

   // now look at the locator list
   TEMPBUFF( letters, 3 );
   TEMPBUFF( numbers, 3 );

   std::string sloc = loc.loc.getValue();
   letters[ 0 ] = sloc[ 0 ];
   letters[ 1 ] = sloc[ 1 ];
   letters[ 2 ] = 0;

   numbers[ 0 ] = sloc[ 2 ];
   numbers[ 1 ] = sloc[ 3 ];
   numbers[ 2 ] = 0;

   int lsi;

   LocSquare *ls = 0;

   for ( LocSquareIterator i = clp->locs.llist.begin(); i != clp->locs.llist.end(); i++ )
   {
      if ( strncmpi( ( *i ) ->loc, letters, 2 ) == 0 )
      {
         ls = ( *i );
         break;
      }
   }

   if ( !ls )
   {
      if ( isalpha( letters[ 0 ] ) && isalpha( letters[ 1 ] ) )
      {
         ls = new LocSquare( letters );
         clp->locs.llist.insert( ls );
      }
   }

   if ( ls )
   {
      unsigned char * npt = ls->map( numbers );
      if ( npt )
      {
         if ( QSOValid && ( *npt < 255 ) )
         {
            if ( ( *npt ) ++ == 0 )
            {
               clp->nlocs++;
               if ( clp->locMult.getValue() )
               {
                  multCount++;
                  newLoc = true;
               }
            }
         }
      }
   }
}

std::string DisplayContestContact::getField( int ACol, const BaseContestLog *const curcon ) const
{
   // only used to get fields for main log display
   std::string res;
   if ( !curcon )
      return res;

   BaseContestLog * clp = contest;

   unsigned short cf = contactFlags.getValue();
   if ( cf & ( LOCAL_COMMENT | COMMENT_ONLY | DONT_PRINT ) )
   {
      switch ( ACol )
      {
         case egTime:
            res = time.getTime( DTGDISP );
            break;
         case egCall:
            res = ( cf & DONT_PRINT ) ? "DELETED" : ( cf & LOCAL_COMMENT ) ? "LOCAL COMMENT" : "COMMENT FOR ADJUDICATOR";
            break;
         case egRSTTx:
            res = comments.getValue();
            break;
      }
   }
   else
   {
      switch ( ACol )
      {
         case egTime:
            res = time.getTime( DTGDISP );
            break;
         case egCall:
            res = cs.fullCall.getValue();
            break;
         case egRSTTx:
            res = reps.getValue();
            break;
         case egSNTx:
            {
               int ss = atoi( serials.getValue().c_str() );
			   AnsiString ssbuff;
               if ( ss )
                  ssbuff.printf( "%03.3d", ss );
               res = ssbuff.c_str();
            }
            break;
         case egRSTRx:
            res = repr.getValue();
            break;
         case egSNRx:
            {
               int sr = atoi( serialr.getValue().c_str() );
			   AnsiString srbuff;
               if ( sr )
				  srbuff.printf( "%03.3d", sr );
               res = srbuff.c_str();
            }
            break;
         case egLoc:
            res = loc.loc.getValue();
            break;
         case egBrg:
            {
			   AnsiString brgbuff;

               if ( contactFlags.getValue() & MANUAL_SCORE )
                  brgbuff = "MAN";
               else
               {
                  if ( loc.valRes == LOC_OK )
                  {
                     if ( contest == curcon )
                     {
                        // use the existing data
						brgbuff.printf( "%03.3d\xB0", varBrg( bearing ) );
                     }
                     else
                     {
                        // rework to come from prime contest loc
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

                           if ( lonlat( loc.loc.getValue(), lon, lat ) == LOC_OK )
                           {
                              curcon->disbear( lon, lat, dist, brg );
                           }
                           brgbuff.printf( "%03.3d\xB0", varBrg( brg ) );
                        }
                     }
                  }
			   }
			   res = brgbuff.c_str();
			}
			break;

		 case egScore:
			{
			   AnsiString scorebuff;

			   if ( contactFlags.getValue() & DONT_PRINT )
				  scorebuff = "DEL";
			   else
				  if ( contactFlags.getValue() & NON_SCORING )
					 scorebuff = "N/S";
				  else
				  {
					 if ( contest == curcon )
					 {
						// use the existing data
						if ( ( cs.valRes == ERR_DUPCS ) && ( curcon == clp ) )
						   scorebuff = "DUP";
						else
						{
						   int temp = contactScore.getValue();
						   if ( temp <= 0 )
							  temp = 0;

                           switch ( contest->scoreMode.getValue() )
                           {
                              case PPKM: 	// needs a valid LOC
                                 if ( !curcon->locatorField.getValue() )
                                 {
                                    if ( temp )
                                       scorebuff = "1";
                                    else
                                       scorebuff = "0";
                                 }
                                 else
                                 {
                                    if ( contactFlags.getValue() & XBAND )
                                    {
									   scorebuff.printf( "%4dXB", temp );
                                    }
                                    else
                                       scorebuff.printf( "%4d", temp );
                                 }
                                 break;

                              case PPQSO:
                                 // actually we would often want the distance here
                                 scorebuff.printf( "%4d", temp );
                                 break;

                           }
                        }
                     }
                     else
                     {
                        // we don't have it worked out already...
                        double lon = 0.0;
                        double lat = 0.0;
                        int brg;
                        double dist = 0.0;

                        if ( lonlat( loc.loc.getValue(), lon, lat ) == LOC_OK )
                        {
                           curcon->disbear( lon, lat, dist, brg );
                        }
                        scorebuff.printf( "%d", ( int ) dist );

                     }
                  }
               res = scorebuff.c_str();
            }
            break;
         case egExchange:
            if ( districtMult )
            {
               res = districtMult->districtCode + "(" + extraText.getValue() + ")";
            }
            else
            {
               res = extraText.getValue();
            }
            break;
         case egComments:
            {
               res = comments.getValue();
            }
            break;
      }
   }
   return res;
}
void DisplayContestContact::processMinosStanza( const std::string &methodName, MinosTestImport * const mt )
{
   std::string updtg;

   int itemp;
   if ( mt->getStructArgMemberValue( "lseq", itemp ) )
      logSequence = ( unsigned long ) itemp;
   mt->getStructArgMemberValueDTG( "uDTG", updtg );

   if ( methodName == "MinosLogComment" )
   {
      std::string ctime;
      mt->getStructArgMemberValueDTG( "logTime", ctime );
      time.setIsoDTG( ctime );

      bool btemp = false;
      unsigned short cf = contactFlags.getValue();
      if ( mt->getStructArgMemberValue( "LocalComment", btemp ) )
         mt->setBit( cf, LOCAL_COMMENT, btemp );
      if ( mt->getStructArgMemberValue( "DontPrint", btemp ) )
         mt->setBit( cf, DONT_PRINT, btemp );
      contactFlags.setInitialValue( cf );
      mt->getStructArgMemberValue( "comment", comments );

   }
   else
      if ( methodName == "MinosLogQSO" )
      {
         modificationCount++;

         updtime.setIsoDTG( updtg );

         std::string ctime;
         if ( mt->getStructArgMemberValueDTG( "logTime", ctime ) )
            time.setIsoDTG( ctime );

         unsigned short cf = contactFlags.getValue();
         bool btemp = false;
         if ( mt->getStructArgMemberValue( "validDistrict", btemp ) )
            mt->setBit( cf, VALID_DISTRICT, btemp );
         mt->setBit( cf, VALID_DISTRICT, btemp );
         if ( mt->getStructArgMemberValue( "countryForced", btemp ) )
            mt->setBit( cf, COUNTRY_FORCED, btemp );
         if ( mt->getStructArgMemberValue( "unknownCountry", btemp ) )
            mt->setBit( cf, UNKNOWN_COUNTRY, btemp );
         if ( mt->getStructArgMemberValue( "nonScoring", btemp ) )
            mt->setBit( cf, NON_SCORING, btemp );
         if ( mt->getStructArgMemberValue( "manualScore", btemp ) )
            mt->setBit( cf, MANUAL_SCORE, btemp );
         if ( mt->getStructArgMemberValue( "DontPrint", btemp ) )
            mt->setBit( cf, DONT_PRINT, btemp );
         if ( mt->getStructArgMemberValue( "validDuplicate", btemp ) )
            mt->setBit( cf, VALID_DUPLICATE, btemp );
         if ( mt->getStructArgMemberValue( "toBeEntered", btemp ) )
            mt->setBit( cf, TO_BE_ENTERED, btemp );
         if ( mt->getStructArgMemberValue( "xBand", btemp ) )
            mt->setBit( cf, XBAND, btemp );
         if ( mt->getStructArgMemberValue( "Forced", btemp ) )
            mt->setBit( cf, FORCE_LOG, btemp );

         contactFlags.setInitialValue( cf );

         mt->getStructArgMemberValue( "modeTx", mode );
         mt->getStructArgMemberValue( "rstTx", reps );
         mt->getStructArgMemberValue( "serialTx", serials );
         mt->getStructArgMemberValue( "exchangeTx", contest->location );
         mt->getStructArgMemberValue( "modeRx", mode );
         mt->getStructArgMemberValue( "callRx", cs.fullCall );
         mt->getStructArgMemberValue( "rstRx", repr );
         mt->getStructArgMemberValue( "serialRx", serialr );
         mt->getStructArgMemberValue( "exchangeRx", extraText );
         if ( mt->getStructArgMemberValue( "locRx", loc.loc ) )
            loc.validate();
         mt->getStructArgMemberValue( "commentsTx", comments );
         mt->getStructArgMemberValue( "commentsRx", comments );

         mt->getStructArgMemberValue( "power", contest->power );
         mt->getStructArgMemberValue( "band", contest->band );
         mt->getStructArgMemberValue( "claimedScore", contactScore );
         mt->getStructArgMemberValue( "forcedMult", forcedMult );

         mt->getStructArgMemberValue( "op1", op1 );
         mt->getStructArgMemberValue( "op2", op2 );

         int maxct = atoi( serials.getValue().c_str() );
         if ( maxct > contest->maxSerial )
            contest->maxSerial = maxct;

         contest->validationPoint = this;
         check();
         BaseContact bc( *this );   // this should get it now??
         getHistory().push_back( bc );
      }

}

