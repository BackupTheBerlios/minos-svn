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

ScreenContact::ScreenContact() : time( false ), logSequence( 0 )
{}
ScreenContact::~ScreenContact()
{}
void ScreenContact::initialise( BaseContestLog *ct )
{
   contest = ct;
   logSequence = ( unsigned long ) - 1;
   cs = callsign();
   loc = locator();
   time = dtg( false );

   BaseContestLog * clp = ct;

   reps = "5  ";

   int ms = 0;
   if ( clp )
   {
      mode = clp->mode.getValue();
      ms = clp->maxSerial + 1;
   }
   char temp[ 10 ];
   sprintf( temp, "%03.3d", ms );
   serials = temp;
   repr = "5  ";
   serialr = std::string( SERIALLENGTH, ' ' );
   extraText = "";
   comments = "";
   contactFlags = 0;
   forcedMult = "";
   QSOValid = false;
   newCtry = false;
   newDistrict = false;
   newLoc = false ;
   op1 = "" ;
   op2 = "" ;

   districtMult = 0;
   ctryMult = 0;

   contactScore = -1;
   bearing = 0;

   multCount = 0;

}
void ScreenContact::copyFromArg( BaseContact &cct )
{
   logSequence = cct.getLogSequence();
   loc = cct.loc;
   loc.validate();
   loc.loc.clearDirty();

   extraText = cct.extraText.getValue();

   cs = cct.cs;
   cs.fullCall.clearDirty();
   cs.validate();

   time = cct.time;
   time.clearDirty();

   reps = cct.reps.getValue();
   serials = cct.serials.getValue();
   repr = cct.repr.getValue();
   serialr = cct.serialr.getValue();

   QSOValid = cct.QSOValid;

   districtMult = cct.districtMult;
   ctryMult = cct.ctryMult;
   multCount = cct.multCount;
   forcedMult = cct.forcedMult.getValue();

   op1 = cct.op1.getValue();
   op2 = cct.op2.getValue();

   newLoc = cct.newLoc;
   newDistrict = cct.newDistrict;
   newCtry = cct.newCtry;

   comments = cct.comments.getValue();

   contactFlags = cct.contactFlags.getValue();

   contactScore = cct.contactScore.getValue();
   bearing = cct.bearing;
   mode = cct.mode.getValue();
}
void ScreenContact::copyFromArg( ScreenContact &cct )
{
   //   logSequence = cct.logSequence; // don't copy between partial and screen contacts.
   loc = cct.loc;
   loc.validate();
   loc.loc.clearDirty();

   extraText = cct.extraText;

   cs = cct.cs;
   cs.fullCall.clearDirty();
   cs.validate();

   time = cct.time;
   time.clearDirty();

   reps = cct.reps;
   serials = cct.serials;
   repr = cct.repr;
   serialr = cct.serialr;

   QSOValid = cct.QSOValid;

   districtMult = cct.districtMult;
   ctryMult = cct.ctryMult;
   multCount = cct.multCount;
   forcedMult = cct.forcedMult;
   op1 = cct.op1;
   op2 = cct.op2;

   newLoc = cct.newLoc;
   newDistrict = cct.newDistrict;
   newCtry = cct.newCtry;

   comments = cct.comments;

   contactFlags = cct.contactFlags;

   contactScore = cct.contactScore;
   bearing = cct.bearing;
   mode = cct.mode;
}
void ScreenContact::check( )
{
   // check on country and district. If valid, return true,
   // having mapped any synonyms to their parents and
   // saved the pointers.

   int checkret = 0;
   BaseContestLog * clp = contest;

   if ( contactFlags & ( LOCAL_COMMENT | COMMENT_ONLY ) )
      return ;
   if ( contactFlags & NON_SCORING )
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

   if ( contactFlags & COUNTRY_FORCED )
   {
      ctryMult = MultLists::getMultLists() ->getCtryForPrefix( forcedMult );
   }
   else
   {
      ctryMult = findCtryPrefix( cs );
   }


   unsigned short cf = contactFlags;
   cf &= ~UNKNOWN_COUNTRY;
   if ( !checkret && ( clp->countryMult.getValue() || clp->districtMult.getValue() ) && !ctryMult )    // need at least a valid country
   {
      cf |= UNKNOWN_COUNTRY;
   }
   contactFlags = cf;
   if ( clp->districtMult.getValue() && ctryMult )
   {
      // if CC_mult and country "has districts" search for the "extra" in the county synonym list

      // check that the district and country agree

      // if the correct parts don't exist, not a valid contact!
      // NB that the rest of the contact has to be valid as well!

      if ( ctryMult->hasDistricts() )    // continentals dont have counties
      {
         districtMult = MultLists::getMultLists() ->searchDistrict( extraText );
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
               if ( !trim( comments ).size() )
                  checkret = ERR_21;
            }
            else
               if ( !trim( extraText ).size() )
                  checkret = ERR_21;
         }
      }

   if ( checkret )
      return ;

   QSOValid = true;        // for now

}
bool ScreenContact::isNextContact( void ) const
{
   return ( logSequence == ( unsigned long ) - 1L ) ? true : false;
}

