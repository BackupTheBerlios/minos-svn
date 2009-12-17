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

//============================================================
namespace ContactBuffs
{
   char mult_chars[] = " -=####";

   char scorebuff[ 20 ];
   char brgbuff[ 6 ];
   char buff2[ 120 ];
   char qthbuff[ 120 ];
   char srbuff[ 6 ];
   char ssbuff[ 6 ];
   char buff[ 1024 ];
}
//==========================================================================
//==========================================================================
BaseContact::BaseContact( BaseContestLog * contest, bool time_now ) :
      contest( contest ), contactScore( -1 ), time( time_now ), updtime( time_now ),
      contactFlags( 0 ), multCount( 0 ),
      bearing( -1 ),
      districtMult( 0 ), ctryMult( 0 ),
      QSOValid( false ),
      newLoc( false ), newDistrict( false ), newCtry( false )
{
}
BaseContact::BaseContact( const BaseContact &ct )
      : time( false ), updtime( false )
{
   *this = ct;
}
BaseContact& BaseContact::operator =( const BaseContact &ct )
{
   contest = ct.contest;
   updtime = ct.updtime;      //CONTAIN MinosItem
   cs = ct.cs;   //CONTAIN MinosItem
   loc = ct.loc;   //CONTAIN MinosItem
   time = ct.time;      //CONTAIN MinosItem

   extraText = ct.extraText;
   mode = ct.mode;
   reps = ct.reps;
   serials = ct.serials;
   repr = ct.repr;
   serialr = ct.serialr;
   comments = ct.comments;
   contactFlags = ct.contactFlags;
   forcedMult = ct.forcedMult;

   op1 = ct.op1;         // current main op - derived from contacts
   op2 = ct.op2;         // current second op - derived from contacts

   contactScore = ct.contactScore;

   return *this;
}
//==========================================================================
//==========================================================================
void BaseContact::clearDirty()
{
   cs.fullCall.clearDirty();
   loc.loc.clearDirty();
   time.clearDirty();

   extraText.clearDirty();
   mode.clearDirty();
   reps.clearDirty();
   serials.clearDirty();
   repr.clearDirty();
   serialr.clearDirty();
   comments.clearDirty();
   contactFlags.clearDirty();
   forcedMult.clearDirty();
   op1.clearDirty();
   op2.clearDirty();
   contactScore.clearDirty();
}
void BaseContact::setDirty()
{
   cs.fullCall.setDirty();
   loc.loc.setDirty();
   time.setDirty();

   extraText.setDirty();
   mode.setDirty();
   reps.setDirty();
   serials.setDirty();
   repr.setDirty();
   serialr.setDirty();
   comments.setDirty();
   contactFlags.setDirty();
   forcedMult.setDirty();
   op1.setDirty();
   op2.setDirty();
   contactScore.setDirty();
}
//==========================================================================
CountryEntry *findCtryPrefix( const callsign &cs )
{
   CountryEntry * ctryMult;

   std::string testpart = "/";	// look for e.g. /RVI as a country suffix
   testpart += cs.suffix;	// look for e.g. /RVI as a country suffix

   CountrySynonym *csyn = 0;
   if ( cs.suffix.length() )
      csyn = MultLists::getMultLists() ->searchCountrySynonym( testpart );

   if ( !csyn )   	// look with number
   {
      /*
      		// eg for <pe/f0ctt/mm> (g0gjv/p) [F6CTT/RVI/P] ?F6CTT/RVI?
      		char prefix[BITLENGTH + 1]; // <pe> (g) [RVI] ???? country of location
      		char prefix2[BITLENGTH + 1];  // <f> (g) [F] ?F?country of issue
      		char number[NUMBITLENGTH + 1]; // <0> (0) [6] ?6?numeric part
      		char body[BITLENGTH + 1];  // <ctt> (gjv) [CTT] ?CTT?main body
      		char suffix[TRAILBITLENGTH + 1]; // <mm> (p) [P] ?RVI?trailer
      */
      if ( cs.prefix != cs.prefix2 )
      {
         // we have a leading / for a pre-pended prefix, so callsign itself is
         // not relevant
         csyn = MultLists::getMultLists() ->searchCountrySynonym( cs.prefix );
      }

      if ( !csyn )
      {
         // take the whole callsign, extra prefix, suffix, the lot and look for the
         // longest matching synonym. If the list is incomplete then this may
         // misidentify the country - e.g. if GW were missed out then this algorithm
         // would (wrongly?) allow a match on G. This might be a benefit for e.g.
         // PA or DL, as we don't need to put all the synonyms in, just the base
         // letter

         // There are really foul callsigns, such as
         // Glorioso Island:      FR-G  e.g.  FR7GL
         // Callbook implies that FR#*/G (see below) is also valid
         // Short of entering ALL these, not sure what to do

         // How about a '#' in the synonym meaning any number?
         // Also need an "any letter" - what about '?', to match
         // DOS wild cards? As well as * to mean a sequence of letters

         // This coding cannot be fully expanded into the synonym list; maybe
         // some kind of synonym decision tree needs to be built
         // This is getting VERY nasty; maybe we just say to enter the actual call
         // as a synonym (but of what... we need a placeholder for the main country)!
         testpart = cs.fullCall.getValue();
         testpart = trimr( testpart );

         int clen = testpart.length();
         int minlen;

         for ( minlen = 0; minlen < clen; minlen++ )
         {
            // go past any leading digits
            if ( !isdigit( testpart[ minlen ] ) )
               break;
         }
         for ( ; minlen < clen; minlen++ )
         {
            // and then to end of leading alpha part
            if ( !isalpha( testpart[ minlen ] ) )
               break;
         }
         while ( ( clen >= minlen ) && ( !csyn ) )
         {
            // we need to stop when we get to the basic prefix...
            // otherwise RVI6ABC ends up matching R, which is UA
            testpart[ clen ] = 0;
            clen--;
            csyn = MultLists::getMultLists() ->searchCountrySynonym( testpart );
         }
         if ( !csyn )
         {
            csyn = MultLists::getMultLists() ->searchCountrySynonym( cs.prefix );
         }
      }
   }
   ctryMult = ( csyn ) ? ( csyn->country ) : 0;
   return ctryMult;
}
void BaseContact::getText( std::string &dest, const BaseContestLog * const curcon ) const
{
   ContactBuffs::brgbuff[ 0 ] = 0;
   ContactBuffs::scorebuff[ 0 ] = 0;
   ContactBuffs::qthbuff[ 0 ] = 0;

   if ( contactFlags.getValue() & ( LOCAL_COMMENT | COMMENT_ONLY | DONT_PRINT ) )
   {
      sprintf( ContactBuffs::buff, "%-5.5s %s %-60.60s", time.getTime( DTGDISP ).c_str(),
               ( contactFlags.getValue() & DONT_PRINT ) ? "DELETED" : ( contactFlags.getValue() & LOCAL_COMMENT ) ? "LOCAL COMMENT" : "COMMENT FOR ADJUDICATOR",
               comments.getValue().c_str() );
   }
   else
   {
      // if contest requires a serial
      makestrings( curcon ->serialField.getValue() );

      mystrncpy( ContactBuffs::qthbuff, extraText.getValue().c_str(), 100 );

      if ( contactFlags.getValue() & MANUAL_SCORE )
         strcpy( ContactBuffs::brgbuff, "MAN" );

      if ( contactFlags.getValue() & DONT_PRINT )
         strcpy( ContactBuffs::scorebuff, "DEL" );
      else
         if ( contactFlags.getValue() & NON_SCORING )
            strcpy( ContactBuffs::scorebuff, "N/S" );
         else
         {
            // look at the contest dup
            if ( ( cs.valRes == ERR_DUPCS ) && ( curcon == contest ) )
               strcpy( ContactBuffs::scorebuff, "DUP" );
         }
   }

   int nbuff = 4;

   if ( contactFlags.getValue() & VALID_DUPLICATE )
      strcpy( ContactBuffs::buff2, "BP " );
   else
      if ( contactFlags.getValue() & XBAND )
         strcpy( ContactBuffs::buff2, "XB " );
      else
      {
         nbuff = 0;
         ContactBuffs::buff2[ 0 ] = 0;
      }
   strcpysp( ContactBuffs::buff, comments.getValue(), 42 );
   if ( ContactBuffs::buff[ 0 ] )
   {
      strcpysp( &ContactBuffs::buff2[ nbuff ], ContactBuffs::qthbuff, 20 );
      strcat( ContactBuffs::buff2, " | " );
      strcat( ContactBuffs::buff2, ContactBuffs::buff );
   }
   else
      strcpysp( &ContactBuffs::buff2[ nbuff ], ContactBuffs::qthbuff, 42 );

   memset( ContactBuffs::buff, ' ', 255 );
   int next = 0;
   next = placestr( ContactBuffs::buff, time.getDate( DTGDISP ), next, 8 );
   next += 1;
   next = placestr( ContactBuffs::buff, time.getTime( DTGDISP ), next, 5 );

   next += 1;
   next = placestr( ContactBuffs::buff, cs.fullCall.getValue(), next, 11 );

   if ( curcon ->RSTField.getValue() )
      next = placestr( ContactBuffs::buff, reps.getValue(), next, 3 );
   next = placestr( ContactBuffs::buff, ContactBuffs::ssbuff, next, -4 );
   if ( curcon ->RSTField.getValue() )
      next = placestr( ContactBuffs::buff, repr.getValue(), next + 1, 3 );
   next = placestr( ContactBuffs::buff, ContactBuffs::srbuff, next, -4 );

   next = placestr( ContactBuffs::buff, loc.loc.getValue(), next + 1, ( curcon ->allowLoc8.getValue() ) ? 8 : 6 );

   next = placestr( ContactBuffs::buff, ContactBuffs::brgbuff, next + 1, 4 );
   next = placestr( ContactBuffs::buff, ContactBuffs::scorebuff, next, -5 );

   next = placestr( ContactBuffs::buff, op1.getValue(), next, -8 );
   next += 1;
   next = placestr( ContactBuffs::buff, op2.getValue(), next, -8 );

   next = placestr( ContactBuffs::buff, ContactBuffs::buff2, next + 1, 90 );

   dest = trimr( ContactBuffs::buff );
}

void BaseContact::makestrings( bool sf ) const
{
   ContactBuffs::qthbuff[ 0 ] = 0;

   int sr = atoi( serialr.getValue().c_str() );
   int ss = atoi( serials.getValue().c_str() );
   if ( ss )
      sprintf( ContactBuffs::ssbuff, "%03.3d", ss );
   else
      ContactBuffs::ssbuff[ 0 ] = 0;

   if ( sr && sf )
      sprintf( ContactBuffs::srbuff, "%03.3d", sr );
   else
      ContactBuffs::srbuff[ 0 ] = 0;
}

