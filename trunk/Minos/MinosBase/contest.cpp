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
/*
#include "latlong.h"
 
#include "MatchContact.h"
#include "MinosTestImport.h"
*/
bool LtLogSeq::operator() ( const BaseContact* s1, const BaseContact* s2 ) const
{
   return s1->getLogSequence() < s2->getLogSequence();
}

BaseContestLog::BaseContestLog( void ) :
      readOnly( false ),
      postEntry( false ),
      nextBlock( 1 ),
      stanzaCount( 0 ), slotno( -1 ), locValid( false ),
      ode( 0.0 ), odn( 0.0 ), sinodn( 0.0 ), cosodn( 0.0 ),
      allowLoc8( false ), allowLoc4 ( false ),
      RSTField( true ), serialField( true ), locatorField( true ), QTHField( true ),
      otherExchange( false ), countryMult( false ),
      districtMult( false ), locMult( false ), scoreMode( PPKM ),
      powerWatts( true ), maxSerial( 0 ),
      contestScore( 0 ), ndistrict( 0 ), nctry( 0 ), nlocs( 0 ),
      validationPoint( 0 ),
      nextScan( -2 ),
      countryWorked( 0 ), districtWorked( 0 ),
      mycall( "" ),
      QSO1( 0 ), QSO2( 0 ), QSO1p( 0 ), QSO2p( 0 ),
      kms1( 0 ), kms2( 0 ), kms1p( 0 ), kms2p( 0 ),
      mults1( 0 ), mults2( 0 ), mults1p( 0 ), mults2p( 0 )
{
   mode.setValue( "J3E" );
   int i;

   int nc = MultLists::getMultLists() ->getCtryListSize();
   countryWorked = new int[ nc ];
   for ( i = 0; i < nc; i++ )
      countryWorked[ i ] = 0;

   nc = MultLists::getMultLists() ->getDistListSize();
   districtWorked = new int[ nc ];
   for ( i = 0; i < nc; i++ )
      districtWorked[ i ] = 0;

}
BaseContestLog::~BaseContestLog()
{
   delete [] districtWorked;
   delete [] countryWorked;
   districtWorked = 0;
   countryWorked = 0;

   locs.freeAll();
   freeAll();
   closeFile();
}
void BaseContestLog::freeAll()
{
   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
      delete ( *i );
   ctList.clear();
}
unsigned int BaseContestLog::indexOf( BaseContact * item )
{
   LogIterator f = std::lower_bound( ctList.begin(), ctList.end(), item, LtLogSeq() );
   if ( f == ctList.end() || ( *f ) != item )
   {
      return ( ctList.end() - ctList.begin() );
   }
   unsigned int diff = f - ctList.begin();
   return diff;
}
int BaseContestLog::getContactCount( void )
{
   return ctList.size();
}

BaseContact *BaseContestLog::pcontactAt( unsigned int i )
{
   if ( i < ctList.size() )
   {
      return ctList.at( i );
   }
   return 0;
}

BaseContact *BaseContestLog::pcontactAtSeq( unsigned long logSequence )
{
   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      if ( ( *i ) ->getLogSequence() == logSequence )
         return ( *i );
   }
   return 0;
}
void BaseContestLog::clearDirty()
{
   postEntry.clearDirty();
   readOnly.clearDirty();
   mycall.fullCall.clearDirty();

   name.clearDirty();
   location.clearDirty();
   myloc.loc.clearDirty();
   allowLoc4.clearDirty();
   allowLoc8.clearDirty();
   RSTField.clearDirty();
   serialField.clearDirty();
   locatorField.clearDirty();
   QTHField.clearDirty();
   power.clearDirty();
   mode.clearDirty();
   band.clearDirty();
   otherExchange.clearDirty();
   countryMult.clearDirty();
   locMult.clearDirty();
   districtMult.clearDirty();
   powerWatts.clearDirty();
   scoreMode.clearDirty();
   DTGStart.clearDirty();
   DTGEnd.clearDirty();
   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      ( *i ) ->clearDirty();
   }
}
void BaseContestLog::setDirty()
{
   postEntry.setDirty();
   readOnly.setDirty();
   mycall.fullCall.setDirty();
   name.setDirty();
   location.setDirty();
   myloc.loc.setDirty();
   allowLoc4.setDirty();
   allowLoc8.setDirty();
   RSTField.setDirty();
   serialField.setDirty();
   locatorField.setDirty();
   QTHField.setDirty();
   power.setDirty();
   mode.setDirty();
   band.setDirty();
   otherExchange.setDirty();
   countryMult.setDirty();
   locMult.setDirty();
   districtMult.setDirty();
   powerWatts.setDirty();
   scoreMode.setDirty();
   DTGStart.setDirty();
   DTGEnd.setDirty();
   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      ( *i ) ->setDirty();
   }
}
void BaseContestLog::makeContact( bool timeNow, BaseContact *&lct )
{
   lct = new BaseContact( this, timeNow );
}
void BaseContestLog::validateLoc( void )
{
   if ( myloc.validate( ode, odn ) == LOC_OK )
   {
      cosodn = cos( odn );
      sinodn = sin( odn );
      locValid = true;
   }
   else
      locValid = false;

}
/*********************************************************************/
/* calculate distance and bearing of contact from our station        */
/*                                                                   */
/* uses global double variables odn, ode, sinodn, and cosodn, and pi */
/*                                                                   */
/* pi must be set to value of pi before calling                      */
/* our station longtitude and latitude must be set up in ode and odn */
/* before invoking this routine.   Also, cos(odn) must be set up in  */
/* cosodn and sin(odn) in sindodn.                                   */
/*                                                                   */
/* Latitude is NS, Longtitude is EW                                  */
/*                                                                   */
/*********************************************************************/
void BaseContestLog::disbear( double lon, double lat, double &dist, int &brg ) const
//double lon ;                    /* other stations longtitude */
//double lat ;                    /* other stations latitude */
//int *dist ;                     /* resulting distance */
//int *brg ;                      /* resulting bearing */

{
   if ( myloc.valRes != LOC_OK )
   {
      dist = 1;
      brg = 0;
      return ;
   }
   if ( lon == ode && lat == odn )        /* same square ! */ /* testing doubles for equality! */
   {
      // same square - commenced Kilometre, so always 1
      dist = 1;
      brg = 0 ;                       /* or bearing */
      return ;                         /* just exit */
   }

   double coslat = cos( lat );                    /* pre-calculate values */
   double sinlat = sin( lat );
   double coscos = cosodn * coslat ;

   // first distance into dx

   double co = cos( ode - lon ) * coscos + sinodn * sinlat ;
   double ca = atan( fabs( sqrt( ( double ) 1.0 - co * co ) / co ) );
   if ( co < ( double ) 0.0 )
      ca = pi - ca ;
   double dx = ( double ) 6371.291 * ca ;       /* 6371.291 is approved radius of earth */

   // and then the bearing

   double si = sin( lon - ode ) * coscos ;
   co = sinlat - sinodn * cos( ca );
   double az = atan( fabs( si / co ) );
   if ( co < ( double ) 0.0 )
      az = pi - az ;
   if ( si < ( double ) 0.0 )
      az = -az ;
   if ( az < ( double ) 0.0 )
      az = az + ( double ) 2.0 * pi ;

   az = az / dr ;                      /* convert to degrees */
   az += ( double ) 0.5 ;                /* correct angle */
   dx = ceil( dx );                      // adjust for commenced kilometer
   dx += ( double ) 0.5 ;              // make sure double truncates properly back to int
   dist = dx ;			                  /* return result */
   if ( ( int ) az == 0 )                  /* due north */
      az = 360.00 ;                 	/* so show valid */
   brg = ( int ) az ;                   /* and give it back as integer */
}
//---------------------------------------------------------------------------
bool BaseContestLog::getsdist( const char *loc, char *minloc, double &mindist )
{
   int brg;
   double dist = 0.0;
   double lon = 0.0;
   double lat = 0.0;

   int lres = lonlat( loc, lon, lat );
   if ( lres == LOC_OK || lres == LOC_SHORT )
   {
      disbear( lon, lat, dist, brg );
      if ( dist < mindist )
      {
         mindist = dist;
         strcpy( minloc, loc );
      }
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
int BaseContestLog::CalcNearest( const std::string &scalcloc )
{
   const char * calcloc = scalcloc.c_str();

   if ( scalcloc.size() != 4 )
      return 0;	// only valid 4 fig locs

   // calculate the nearest point of loc2 from loc1

   double mindist = 1000000.0;

   char minloc[ LOCLENGTH + 1 ] = {0};
   char temploc[ LOCLENGTH + 1 ];

   strncpy( temploc, calcloc, LOCLENGTH );
   temploc[ LOCLENGTH + 1 ] = 0;

   for ( char i = 'A'; i <= 'X'; i++ )
   {
      temploc[ 4 ] = 'A';
      temploc[ 5 ] = i;
      temploc[ 6 ] = 0;
      if ( !getsdist( temploc, minloc, mindist ) )
      {
         return -1;
      }

      temploc[ 4 ] = 'X';

      if ( !getsdist( temploc, minloc, mindist ) )
      {
         return -1;
      }

      temploc[ 4 ] = i;
      temploc[ 5 ] = 'X';
      if ( !getsdist( temploc, minloc, mindist ) )
      {
         return -1;
      }

      temploc[ 5 ] = 'A';
      if ( !getsdist( temploc, minloc, mindist ) )
      {
         return -1;
      }
   }
   return mindist;
}
void BaseContestLog::getMatchText( BaseContact *pct, std::string &disp, const BaseContestLog *const ct ) const
{
   if ( DupSheet.isCurDup( pct ) )
   {
      disp = "DUPLICATE OF ";
   }
   std::string temp;
   pct->getText( temp, ct );
   disp += temp;

   disp = trim( disp );
}
void BaseContestLog::getMatchField( BaseContact *pct, int col, std::string &disp, const BaseContestLog *const ct ) const
{
   if ( pct && DupSheet.isCurDup( pct ) && col == 0 )
   {
      disp = "DUP OF";
      return ;
   }
   std::string temp;
   if ( pct )
      temp = pct->getField( col, ct );

   disp = trim( temp );
}
bool BaseContestLog::updateStat( BaseContact *cct )
{
   // need to check if a valid DTG
   bool acted = false;
   dtg tnow( true );

   time_t nowTime;
   time_t cttime;

   if ( ( cct->contactScore.getValue() <= 0 ) || !cct->time.getDtg( cttime ) || ! tnow.getDtg( nowTime) )
      return true;

   time_t t = nowTime + MinosParameters::getMinosParameters() ->getBigClockCorrection() * dtg::daySecs;

   time_t tdiff = t - cttime;
   if ( tdiff < 0 )
      return true;

   long thisscore = cct->contactScore.getValue();
   switch ( scoreMode.getValue() )
   {
      case PPKM:
         break;

      case PPQSO:
         if ( thisscore > 0 )
            thisscore = 1;
         else
            thisscore = 0;
         break;

   }

   if ( tdiff < MinosParameters::getMinosParameters() ->getStatsPeriod1() * 60L )
   {
      // need a common routine
      // This period
      QSO1++;
      kms1 += thisscore;
      mults1 += cct->multCount;
      acted = true;
   }
   else
      if ( tdiff < MinosParameters::getMinosParameters() ->getStatsPeriod1() * 60L * 2 )
      {
         // need a common routine
         // previous period
         QSO1p++;
         kms1p += thisscore;
         mults1p += cct->multCount;
         acted = true;
      }

   if ( tdiff < MinosParameters::getMinosParameters() ->getStatsPeriod2() * 60L )
   {
      // need a common routine
      // this period
      QSO2++;
      kms2 += thisscore;
      mults2 += cct->multCount;
      acted = true;
   }
   else
      if ( tdiff < MinosParameters::getMinosParameters() ->getStatsPeriod2() * 60L * 2 )
      {
         // need a common routine
         // previous period
         QSO2p++;
         kms2p += thisscore;
         mults2p += cct->multCount;
         acted = true;
      }

   return acted;
}
void BaseContestLog::updateStats( void )
{
   QSO1 = 0;
   kms1 = 0;
   mults1 = 0;
   QSO2 = 0;
   kms2 = 0;
   mults2 = 0;
   QSO1p = 0;
   kms1p = 0;
   mults1p = 0;
   QSO2p = 0;
   kms2p = 0;
   mults2p = 0;
   for ( int i = getContactCount() - 1; i >= 0; i-- )
   {
      if ( !updateStat( ctList[ i ] ) )
         break;
   }
}

static void isBestDX( BaseContact *cct, BaseContact **bestDX )
{

   if ( cct->contactFlags.getValue() & ( NON_SCORING | COMMENT_ONLY | LOCAL_COMMENT | DONT_PRINT ) )
      return ;

   if ( cct->cs.valRes != CS_OK )
      return ;

   if ( ( !*bestDX ) || ( ( cct->contactScore.getValue() > ( *bestDX ) ->contactScore.getValue() ) ) )
      * bestDX = cct;
}
BaseContact *BaseContestLog::getBestDX( void )
{
   BaseContact * bestDX = 0;
   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
      isBestDX( ( *i ), &bestDX );
   return bestDX;
}
std::string BaseContestLog::dateRange( DTG dstyle )
{
   std::string date1 = "999999";
   std::string date2 = "000000";
   LogIterator low = ctList.end();
   LogIterator high = ctList.end();
   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      if ( ( *i ) ->contactScore.getValue() > 0 )
      {
         std::string qsodate = ( *i ) ->time.getDate( DTGLOG );
         if ( qsodate < date1 )
         {
            low = i;
            date1 = ( *i ) ->time.getDate( DTGLOG );
         }
         if ( qsodate > date2 )
         {
            high = i;
            date2 = ( *i ) ->time.getDate( DTGLOG );
         }
      }
   }
   if ( low == ctList.end() || high == ctList.end() )
   {
      return "";
   }
   return ( *low ) ->time.getDate( dstyle ) + ";" + ( *high ) ->time.getDate( dstyle );
}

void BaseContestLog::setScore( std::string &buff )
{
   // do this first, as the same buff gets used for summary prints...

   char brcc1 = '(';
   char brcc2 = ')';
   char brcc3 = '(';
   char brcc4 = ')';
   char brloc1 = '(';
   char brloc2 = ')';

   int mults = 0;
   if ( countryMult.getValue() )
   {
      brcc1 = brcc2 = ' ';
      mults += nctry;
   }
   if ( districtMult.getValue() )
   {
      brcc3 = brcc4 = ' ';
      mults += ndistrict;
   }
   if ( locMult.getValue() )
   {
      brloc1 = brloc2 = ' ';
      mults += nlocs;
   }
   mults = std::max(mults, 1);

   long totalScore = contestScore*mults;

   buff = ( boost::format( "Score: %ld pts :%c%d countries%c:%c%d districts%c:%c%d locators%c = %ld" )
            % contestScore % brcc1 % nctry % brcc2 % brcc3 % ndistrict %
            brcc4 % brloc1 % nlocs % brloc2 % totalScore ).str();

}
void BaseContestLog::startScan( void )
{
   DupSheet.clear();

   locs.freeAll();
   delete [] districtWorked;
   delete [] countryWorked;
   districtWorked = 0;
   countryWorked = 0;

   int nc = MultLists::getMultLists() ->getDistListSize();
   districtWorked = new int[ nc ];
   memset( districtWorked, 0, nc * sizeof( int ) );

   nc = MultLists::getMultLists() ->getCtryListSize();
   countryWorked = new int[ nc ];
   memset( countryWorked, 0, nc * sizeof( int ) );

   // set up for the idle loop scan
   // NB we may need to clear e.g. the accumulated score

   // we will need to clear the multiplier work counts


   contestScore = 0;
   ndistrict = 0;
   nctry = 0;
   nlocs = 0;

   nextScan = -1;

   rescan();

}
void BaseContestLog::rescan( void )
{
   std::string curop1;
   std::string curop2;
   oplist.clear();
   while ( nextScan >= -1 )
   {
      // get the next contact in sequence and do any required scan checks
      nextScan++;
      if ( nextScan >= getContactCount() )
      {
         // end of scan

         nextScan = -2;

         if ( isReadOnly() )
         {
            DupSheet.clear();
         }

         break;
      }
      BaseContact *nct = ctList[ nextScan ];
      if ( !nct )
         break ;

      curop1 = nct->op1.getValue();
      oplist.insert( curop1 );
      curop2 = nct->op2.getValue();
      oplist.insert( curop2 );

      if ( nct->contactFlags.getValue() & ( NON_SCORING | DONT_PRINT | LOCAL_COMMENT | COMMENT_ONLY ) )
      {
         nct->contactScore.setValue( -1 );		// force it!
         continue;
      }

      validationPoint = nct;

      // check for duplicates; accumulate the current points score

      nct->cs.valRes = CS_NOT_VALIDATED;
      nct->cs.validate( );

      nct->bearing = -1;		// force a recalc
      nct->loc.validate();

      int index;

      if ( DupSheet.checkCurDup( nct, 0, true ) )    // check for dup, insert it if required
         nct->cs.valRes = ERR_DUPCS;


      nct->multCount = 0;
      nct->newDistrict = false;
      nct->newCtry = false;
      nct->newLoc = false;
      nct->check( );
   }
   op1.setInitialValue( curop1 );
   op2.setInitialValue( curop2 );
}
//============================================================
DupContact::DupContact( BaseContact *c ) : dct( c ), sct( 0 )
{}
DupContact::DupContact( ScreenContact *c ) : dct( 0 ), sct( c )
{}
DupContact::DupContact() : dct( 0 ), sct( 0 )
{}
DupContact::~DupContact()
{}
bool DupContact::operator<( const DupContact& rhs ) const
{
   callsign * c1 = 0;    // search item
   callsign *c2 = 0;    // collection item

   if ( dct )
   {
      c1 = &dct->cs;
   }
   else
      if ( sct )
      {
         c1 = &sct->cs;
      }
   if ( rhs.dct )
   {
      c2 = &rhs.dct->cs;
   }
   else
      if ( rhs.sct )
      {
         c2 = &rhs.sct->cs;
      }

   int ret = stricmp( c1->prefix2, c2->prefix2 );
   if ( ret == 0 )
   {
      ret = stricmp( c1->number, c2->number );
      if ( ret == 0 )
      {
         ret = stricmp( c1->body, c2->body );
         if ( ret == 0 )
            return false;
      }
   }
   return ret < 0;
}
bool DupContact::operator==( const DupContact& rhs ) const
{
   callsign * c1 = 0;    // search item
   callsign *c2 = 0;    // collection item

   if ( dct )
   {
      c1 = &dct->cs;
   }
   else
      if ( sct )
      {
         c1 = &sct->cs;
      }
   if ( rhs.dct )
   {
      c2 = &rhs.dct->cs;
   }
   else
      if ( rhs.sct )
      {
         c2 = &rhs.sct->cs;
      }

   int ret = stricmp( c1->prefix2, c2->prefix2 );
   if ( ret != 0 )
      return false;

   ret = stricmp( c1->number, c2->number );
   if ( ret != 0 )
      return false;

   ret = stricmp( c1->body, c2->body );
   if ( ret != 0 )
      return false;

   return true;
}
bool DupContact::operator!=( const DupContact& rhs ) const
{
   return !( *this == rhs );
}

bool LtDup::operator() ( const DupContact* s1, const DupContact* s2 ) const
{
   bool res = ( *s1 < *s2 );
   return res;
}

dupsheet::dupsheet()
{}
dupsheet::~dupsheet()
{
   clear();
}
bool dupsheet::checkCurDup( ScreenContact *nct, BaseContact *valp, bool insert )
{
   curdup = 0;
   if ( nct->cs.valRes == CS_OK )
   {
      DupContact test( nct );
      bool exists = std::binary_search( ctList.begin(), ctList.end(), &test, LtDup() );
      if ( exists )
      {
         if ( !( nct->contactFlags & VALID_DUPLICATE ) )
         {
            DupIterator c = std::lower_bound( ctList.begin(), ctList.end(), &test, LtDup() );

            if ( valp && valp->getLogSequence() <= ( *c ) ->dct->getLogSequence() )
            {
               return false; // as val point earlier than current list item
            }

            if ( c != ctList.end() )
               curdup = *c;

            return true;
         }
      }
      else
         if ( insert )
         {
            DupContact * ins = new DupContact( nct );
            ctList.insert( ins );
            return false;
         }
   }
   return false;
}
bool dupsheet::checkCurDup( BaseContact *nct, BaseContact *valp, bool insert )
{
   curdup = 0;
   if ( nct->cs.valRes == CS_OK )
   {
      DupContact test( nct );
      bool exists = std::binary_search( ctList.begin(), ctList.end(), &test, LtDup() );
      if ( exists )
      {
         if ( !( nct->contactFlags.getValue() & VALID_DUPLICATE ) )
         {
            DupIterator c = std::lower_bound( ctList.begin(), ctList.end(), &test, LtDup() );

            if ( valp && valp->getLogSequence() <= ( *c ) ->dct->getLogSequence() )
            {
               return false; // as val point earlier than current list item
            }

            if ( c != ctList.end() )
               curdup = *c;

            return true;
         }
      }
      else
         if ( insert )
         {
            DupContact * ins = new DupContact( nct );
            ctList.insert( ins );
            return false;
         }
   }
   return false;
}

bool dupsheet::isCurDup( ScreenContact *nct ) const
{
   const DupContact test( nct );
   bool cd = ( curdup && ( *curdup == test ) ) ;

   if ( cd && !nct->isNextContact() )
   {
      if ( nct->getLogSequence() > curdup->dct->getLogSequence() )
      {
         return false;
      }
   }
   return cd;
}
bool dupsheet::isCurDup( BaseContact *nct ) const
{
   const DupContact test( nct );
   bool cd = curdup && ( *curdup == test ) ;

   if ( cd )
   {
      if ( nct->getLogSequence() > curdup->dct->getLogSequence() )
      {
         return false;
      }
   }
   return cd;
}
void dupsheet::clearCurDup()
{
   curdup = 0;
}
BaseContact *dupsheet::getCurDup()
{
   if ( curdup )
      return curdup->dct;
   return 0;
}
void dupsheet::clear()
{
   curdup = 0;
   for ( DupIterator i = ctList.begin(); i != ctList.end(); i++ )
      delete ( *i );
   ctList.clear();
}
//============================================================
void BaseContestLog::processMinosStanza( const std::string &methodName, MinosTestImport * const mt )
{
   unsigned long logSequence = ( unsigned long ) - 1;

   int itemp;
   if ( mt->getStructArgMemberValue( "lseq", itemp ) )
      logSequence = ( unsigned long ) itemp;

   if ( methodName == "MinosLogContest" )
   {
      mt->getStructArgMemberValue( "name", name );
      mt->getStructArgMemberValue( "band", band );

      bool btemp;
      if ( mt->getStructArgMemberValue( "scoreKms", btemp ) )
         scoreMode.setInitialValue( btemp ? PPKM : PPQSO );

      mt->getStructArgMemberValue( "startTime", DTGStart );
      mt->getStructArgMemberValue( "endTime", DTGEnd );

      mt->getStructArgMemberValue( "districtMult", districtMult );
      mt->getStructArgMemberValue( "DXCCMult", countryMult );
      mt->getStructArgMemberValue( "locMult", locMult );
      mt->getStructArgMemberValue( "QTHReq", otherExchange );
      mt->getStructArgMemberValue( "AllowLoc4", allowLoc4 );
      mt->getStructArgMemberValue( "AllowLoc8", allowLoc8 );
   }
   else
      if ( methodName == "MinosLogMode" )
      {
         mt->getStructArgMemberValue( "readOnly", readOnly );
         mt->getStructArgMemberValue( "postEntry", postEntry );
      }
   if ( methodName == "MinosLogQTH" )
   {
      if ( mt->getStructArgMemberValue( "locator", myloc.loc ) )
         validateLoc();
      mt->getStructArgMemberValue( "district", location );
      mt->getStructArgMemberValue( "location", location ); // doubled up...
   }
   else
      if ( methodName == "MinosLogEntry" )
      {
         mt->getStructArgMemberValue( "call", mycall.fullCall );
      }
      else
         if ( methodName == "MinosLogStation" )
         {
            mt->getStructArgMemberValue( "power", power );
         }
         else
            if ( methodName == "MinosLogOperators" )
            {}
            else
               if ( methodName == "MinosLogCurrent" )
               {}
               else
                  if ( methodName == "MinosLogBundles" )
                  {}
                  else
                     if ( methodName == "MinosLogComment" )
                     {
                        BaseContact * rct = pcontactAtSeq( logSequence );
                        if ( !rct )
                        {
                           makeContact( false, rct );
                           rct->setLogSequence( logSequence );
                           ctList.insert( rct );
                           nextBlock++;
                        }
                        rct->processMinosStanza( methodName, mt );

                     }
                     else
                        if ( methodName == "MinosLogQSO" )
                        {
                           BaseContact * rct = pcontactAtSeq( logSequence );
                           if ( !rct )
                           {
                              makeContact( false, rct );
                              rct->setLogSequence( logSequence );
                              ctList.insert( rct );
                              nextBlock++;
                           }
                           rct->processMinosStanza( methodName, mt );

                        }
}

//====================================================================
void BaseContestLog::setStanza( unsigned int /*stanza*/, int /*stanzaStart*/ )
{
   // not used in base log
   // Used in Logger to give file position of stanza
}
//====================================================================
bool BaseContestLog::getStanza( unsigned int /*stanza*/, std::string & /*stanzaData*/ )
{
   // not used in base log
   // used in Logger to get stanza direct from a file
   return false;
}
//====================================================================
BaseContact * BaseContestLog::findNextUnfilledContact()
{
   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      if ( ( *i ) ->contactFlags.getValue() & TO_BE_ENTERED )
      {
         return ( *i );
      }
   }
   return 0;
}
//====================================================================
bool BaseContestLog::checkTime(const dtg &t)
{
   try
   {
      std::string dtgstr = t.getDate(DTGFULL) + t.getTime(DTGLOG);

      TDateTime check = CanonicalToTDT( dtgstr.c_str() );
      TDateTime start = CanonicalToTDT( DTGStart.getValue().c_str() );
      TDateTime end = CanonicalToTDT( DTGEnd.getValue().c_str() );

      if (check < start)
      {
         return false;
      }
      if (check >= end)
      {
         return false;
      }
      return true;
   }
   catch (EConvertError & /*e*/)
   {
      return false;
   }
}
//====================================================================

