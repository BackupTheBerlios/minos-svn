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

#include "gridhint.h"
#include "MatchContact.h"
#include "MatchThread.h"
#include "LogFrame.h"
#include "LogMain.h"
#pragma package(smart_init) 
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMatchThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
TMatchThread *TMatchThread::matchThread = 0;

__fastcall TMatchThread::TMatchThread()
      : TThread(  /*CreateSuspended*/ false ), myMatches( 0 ), myListMatches( 0 ), mct(0),
      EL_ScreenContactChanged ( EN_ScreenContactChanged, & ScreenContactChanged_Event ),
      EL_CountrySelect ( EN_CountrySelect, & CountrySelect_Event ),
      EL_DistrictSelect ( EN_DistrictSelect, & DistrictSelect_Event ),
      EL_LocatorSelect ( EN_LocatorSelect, & LocatorSelect_Event )
{
   Priority = tpLower;
   logMatch = new LogMatcher();
   listMatch = new ListMatcher();
}
void TMatchThread::FinishMatchThread()
{
   if ( matchThread )
   {
      matchThread->Terminate();
      matchThread->WaitFor();
   }
}
void TMatchThread::InitialiseMatchThread()
{
   if ( !matchThread )
      matchThread = new TMatchThread();
}
void TMatchThread::ScreenContactChanged_Event ( MinosEventBase & Event )
{
   ActionEvent2<ScreenContact *, BaseContestLog *, EN_ScreenContactChanged> & S = dynamic_cast<ActionEvent2<ScreenContact *, BaseContestLog *, EN_ScreenContactChanged> &> ( Event );
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   if (S.getContext() == ct)
   {
      mct = S.getData();
      if (mct)
      {
         // we want to initialise the search from the screen contact - break what couplings we can
         // we need to take care over thread safety as well!
         startMatch();
      }
   }
}
//---------------------------------------------------------------------------
void TMatchThread::CountrySelect_Event ( MinosEventBase & Event )
{
   ActionEvent2<std::string,  BaseContestLog *, EN_CountrySelect> & S = dynamic_cast<ActionEvent2<std::string, BaseContestLog *, EN_CountrySelect> &> ( Event );
   std::string sel = S.getData();
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   if (S.getContext() == ct)
   {
      mct = 0;
      CountryEntry *ce = MultLists::getMultLists() ->getCtryForPrefix( sel );
      startMatch(ce);
   }
}

//---------------------------------------------------------------------------
void TMatchThread::DistrictSelect_Event ( MinosEventBase & Event )
{
   ActionEvent2<std::string,  BaseContestLog *, EN_DistrictSelect> & S = dynamic_cast<ActionEvent2<std::string, BaseContestLog *, EN_DistrictSelect> &> ( Event );
   std::string sel = S.getData();
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   if (S.getContext() == ct)
   {

   }
}

//---------------------------------------------------------------------------
void TMatchThread::LocatorSelect_Event ( MinosEventBase & Event )
{
   ActionEvent2<std::string,  BaseContestLog *, EN_LocatorSelect> & S = dynamic_cast<ActionEvent2<std::string, BaseContestLog *, EN_LocatorSelect> &> ( Event );
   std::string sel = S.getData();
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   if (S.getContext() == ct)
   {

   }
}

//---------------------------------------------------------------------------
// limit the total  number of hits
const int MATCH_LIM = 20;
void __fastcall TMatchThread::Execute()
{
   //---- Place thread code here ----
   GJV_thread::setThreadName( "QSO Match" );
   try
   {
      while ( !Terminated )
      {
         logMatch->initMatch();  // does nothing unless matchRequired is true
         listMatch->initMatch();

         // so it only does a max of 20+20 contacts before switching
         // to "other" of log/list
         if ( !logMatch->idleMatch( 20 ) && !listMatch->idleMatch( 20 ) )
            Sleep( 100 );  // this only happens when no more to do
      }
      delete logMatch;
      logMatch = 0;
      delete listMatch;
      listMatch = 0;
   }
   catch ( ... )
   {
      // exception caught - in wrong thread, so we can't do much
      trace( "Exception in match thread" );
   }
   trace( "Exiting match thread" );
}
//---------------------------------------------------------------------------
/*static*/ void TMatchThread::startMatch(   CountryEntry *ce )
{
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendMatchStarting(ct);

   matchThread->logMatch->startMatch( ce );
   matchThread->listMatch->startMatch( ce );

}
//---------------------------------------------------------------------------
void __fastcall TMatchThread::doReplaceContestList( void )
{
   try
   {
      BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
      MinosLoggerEvents::SendReplaceLogList(myMatches, ct);
   }
   catch ( ... )
   {}
}
void __fastcall TMatchThread::replaceContestList( TMatchCollection *matchCollection )
{
   myMatches = matchCollection;
   Synchronize( &doReplaceContestList );
}
//---------------------------------------------------------------------------
void __fastcall TMatchThread::doReplaceListList( void )
{
   try
   {
      BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
      MinosLoggerEvents::SendReplaceListList(myListMatches, ct);
   }
   catch ( ... )
   {}
}
void __fastcall TMatchThread::replaceListList( TMatchCollection *matchCollection )
{
   myListMatches = matchCollection;
   Synchronize( &doReplaceListList );
}
//---------------------------------------------------------------------------
void __fastcall TMatchThread::ShowMatchStatus( std::string mess )
{
   matchStatus = mess;
}
std::string TMatchThread::getMatchStatus( )
{
   if ( matchThread )
   {
      return matchThread->matchStatus;
   }
   return "";
}

//=============================================================================
void __fastcall TMatchThread::doMatchCountry( void )
{
   try
   {
      BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
      MinosLoggerEvents::SendScrollToCountry(ctrymatch, ct);
   }
   catch ( ... )
   {}
}
void __fastcall TMatchThread::matchCountry( std::string cs )
{
   ctrymatch = cs;
   Synchronize( &doMatchCountry );
}
//=============================================================================
void __fastcall TMatchThread::doMatchDistrict( void )
{
   try
   {
      BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
      MinosLoggerEvents::SendScrollToDistrict(distmatch, ct);
   }
   catch ( ... )
   {}
}
void __fastcall TMatchThread::matchDistrict( std::string dist )
{
   distmatch = dist;
   Synchronize( &doMatchDistrict );
}

//=============================================================================
TMatchCollection::TMatchCollection( void )
{}
TMatchCollection::~TMatchCollection( void )
{
   freeAll();
}
int TMatchCollection::getContactCount( void )
{
   return matchList.size();
}
MatchContact *TMatchCollection::pcontactAt( int i )
{
   return matchList.at( i );
}
//=============================================================================
matchElement::matchElement( void ) : match( false ), empty( true )
{
   mstr[ 0 ] = 0;
   rawstr[ 0 ] = 0;
}
static std::string match_temp;
unsigned char matchElement::checkGreater( const std::string &s )
{
   unsigned char res = 0;
   match_temp[ 0 ] = 0;
   if ( !s.length() )
   {
      return res;
   }

   strcpysp( match_temp, s, EXTRALENGTH );
   strupr( match_temp );

   int oldlen = rawstr.length();
   int newlen = match_temp.length();

   if ( newlen == oldlen )
   {
      if ( stricmp( match_temp, rawstr ) == 0 )
         return res;		// no change
      res = SET_CHANGED | SET_NOT_GREATER;
   }
   else
   {
      res = SET_CHANGED | SET_NOT_GREATER;
      if ( ( oldlen == 0 ) || ( ( newlen > oldlen ) && ( strnicmp( rawstr, match_temp, oldlen ) == 0 ) ) )
         res &= ~SET_NOT_GREATER;
   }

   return res;
}
unsigned char matchElement::set
   ( const std::string &s )
{
   unsigned char res = 0;
   match_temp[ 0 ] = 0;
   match = false;
   empty = true;

   if ( !s.length() )
   {
      // Null argument used to clear it out
      mstr[ 0 ] = 0;
      rawstr[ 0 ] = 0;
      return res;
   }

   res = checkGreater( s );

   if ( match_temp[ 0 ] )
   {
      empty = false;
      match = true;
   }

   mstr = match_temp;
   rawstr = match_temp;
   return res;
}
bool matchElement::checkMatch( const std::string &s )
{
   bool ismatch = true;
   if ( match && !empty )
   {
      if ( !wildComp( s, mstr ) )
         ismatch = false;
   }

   return ismatch;
}

//=============================================================================

Matcher::Matcher() : contestIndex( -1 ), contactIndex( -1 ), mp( Exact ),
      matchStarted( false ), firstMatch( Starting ), tickct( 0 ), ce( 0 ),
      matchCollection( 0 ), matchRequired( false )
{
   matchCollection = new TMatchCollection;
}
Matcher::~Matcher()
{
   clearmatchall();
   delete matchCollection;
   matchCollection = 0;
}

void Matcher::clearmatchall( )
{
   contestIndex = -1;
   contactIndex = -1;
   matchStarted = false;
   matchRequired = false;

   matchCollection->freeAll();

   // don't want to keep the old, so clear down the memory of selection terms
   matchcs.set( "" );
   matchloc.set( "" );
   matchqth.set( "" );
   ce = 0;
   thisContestMatched = 0;

   TMatchThread::getMatchThread() ->ShowMatchStatus( "" );
}
// mark things that we want to start a match
void Matcher::startMatch( CountryEntry *c )
{
   matchRequired = true;
   matchStarted = false;				// if it started we need to stop it...
   tickct = 0;
   ce = c;
   thisContestMatched = 0;
}

// on timer, start it off NB this NOT in the main thread
void Matcher::initMatch( void )
{
   try
   {
      if ( matchRequired )
      {
         tickct++;
         if ( tickct < 2 )
            return ;
      }
      else
      {
         return ;
      }

      if ( !TContestApp::getContestApp() ->getCurrentContest() )
      {
         clearmatchall();
         return ;
      }

      if ( !ce )
      {
         clearmatchall();
         // we need to have been sent this... in a start match action?
         ScreenContact * mct = TMatchThread::getMatchThread()->mct;
         if ( !mct )
            return ;

         unsigned char qth_changed = matchqth.set( mct->extraText );
         if ( qth_changed & SET_CHANGED )
         {
            matchDistrict( mct->extraText );
         }
         unsigned char changed = matchcs.set( mct->cs.fullCall.getValue() );	// we rely on set to set up the search terms
         if ( changed & SET_CHANGED )
         {
            matchCountry( mct->cs.fullCall.getValue() );   // scroll to
         }
         changed |= matchloc.set( mct->loc.loc.getValue() );			// so don't do this all on one line
         changed |= qth_changed;

         if ( matchcs.empty && matchloc.empty && matchqth.empty )
         {
            clearmatchall();
            replaceList( matchCollection );
            return ;
         }

         if ( !changed & SET_CHANGED )
            return ;

         if ( changed & SET_NOT_GREATER )
         {
            replaceList( matchCollection );
         }
         // we have something matchable, so go to it!

         mp = Exact;		// set phase
      }
      else
      {
         CountryEntry *c = ce;	// preserve over cleardown
         clearmatchall();
         mp = Country;
         ce = c;
      }

      contestIndex = 0;
      contactIndex = 0;
      matchStarted = true;
      firstMatch = Starting;
   }
   catch ( ... )
   {
      trace( "Exception in init match" );
   }
}
//==============================================================================
LogMatcher::LogMatcher()
{}
LogMatcher::~LogMatcher()
{}
void LogMatcher::matchDistrict( const std::string &extraText )
{
   TMatchThread::getMatchThread() ->matchDistrict( extraText );   // scroll to
}
void LogMatcher::matchCountry( const std::string &cs )
{
   TMatchThread::getMatchThread() ->matchCountry( cs );   // scroll to
}
void LogMatcher::addMatch( BaseContact *cct, BaseContestLog * ccon )
{
   if ( !cct )
      return ;
   int i;

   MatchContact *mct = new MatchLogContact( ccon, cct );

   bool exists = std::binary_search( matchCollection->matchList.begin(), matchCollection->matchList.end(), mct );
   if ( !exists )
   {
      matchCollection->matchList.insert( mct );
   }
   else
   {
      delete mct;
   }
   if ( ccon == TContestApp::getContestApp() ->getCurrentContest() )
   {
      thisContestMatched = matchCollection->matchList.size();
   }
}
bool LogMatcher::idleMatch( int limit )
{
   try
   {
      // this is called repeatedly off the timer to execute the match
      if ( !matchStarted || ( contestIndex < 0 ) )
         return false;				// nothing to do (yet)

      int cnt = matchCollection->matchList.size();
      if ( ( firstMatch == Starting ) && ( ( mp == Exact ) || ( mp == Country ) ) )
      {
         contestIndex = 0;
         contactIndex = 0;
         firstMatch = MainContest;
      }
      else
         if ( ( contestIndex >= TContestApp::getContestApp() ->getContestSlotCount() ) || ( cnt > MATCH_LIM ) )
         {
            bool EndScan = true;

            if ( cnt == 0 )
            {
               switch ( mp )
               {
                  case Exact:
                     {
                        if ( matchcs.match )
                        {
                           bool dropthrough = false;
                           const char *c = matchcs.mstr.c_str();
                           // need to trim out any leading and trailing
                           while ( *c && *c != '/' )
                              c++;

                           const char *c2 = c;
                           if ( *c2 )
                              c2++;	// skip the first /
                           while ( *c2 && *c2 != '/' )
                              c2++;

                           if ( *c && *c2 )
                           {
                              matchcs.mstr = std::string( c ).substr( 0, c2 - c );	// copy back over ourselves
                           }
                           else
                              if ( *c && ( c - matchcs.mstr.c_str() < 3 ) && ( strlen( c ) > 2 ) )
                              {
                                 // prefix less than 3 chars and suffix more than 1 character
                                 matchcs.mstr = std::string( c );	// copy back over ourselves
                              }
                              else
                                 if ( *c == '/' )
                                 {
                                    matchcs.mstr = matchcs.mstr.substr( 0, c - matchcs.mstr.c_str() );	// copy back over ourselves
                                    //                                 *c = 0;				// force a stop on the /
                                 }
                                 else
                                 {
                                    // want to drop through
                                    dropthrough = true;
                                 }

                           if ( !dropthrough )
                           {
                              TMatchThread::getMatchThread() ->ShowMatchStatus( " - No exact match" );
                              mp = NoSuffix;
                              contestIndex = 0;
                              contactIndex = 0;
                              firstMatch = MainContest;
                              EndScan = false;
                              break;
                           }
                        }
                     }
                     // or no /P anyway, so drop through

                  case NoSuffix:
                     {
                        if ( matchcs.match && ( matchloc.match || matchqth.match ) )
                        {
                           matchloc.set( "" );
                           matchqth.set( "" );
                           mp = NoLoc;
                           contestIndex = 0;
                           contactIndex = 0;
                           firstMatch = MainContest;
                           EndScan = false;
                           TMatchThread::getMatchThread() ->ShowMatchStatus( " - No match No Suffix" );
                           break;
                        }
                     }
                     // or no loc anyway, so drop through

                  case NoLoc:
                     {
                        if ( matchcs.match )
                        {
                           // We know that cs is not empty
                           // strip temp cs of its leading country and number
                           const char * mstrStart = matchcs.mstr.c_str();
                           const char * c = &mstrStart[ matchcs.mstr.length() ];

                           while ( c > mstrStart )
                           {
                              if ( !isdigit( *( c - 1 ) ) )
                                 c--;
                              else
                                 break;
                           }

                           if ( c > mstrStart )   	// i.e. we havent got back to the start
                           {
                              if ( *c )   			// we would be left with something
                              {
                                 matchcs.mstr = c;	// copy back over
                                 mp = Body;
                                 contestIndex = 0;
                                 contactIndex = 0;
                                 firstMatch = MainContest;
                                 EndScan = false;
                                 TMatchThread::getMatchThread() ->ShowMatchStatus( " - No match No LOC" );
                                 break;
                              }
                           }
                        }
                     }
                     // else we have already done what we can, so drop through

                  case Country:
                  case District:
                  case Locator:
                  case Body:
                     {
                        // we have finished.
                        break;
                     }
               }
            }

            if ( EndScan )
            {
               matchStarted = false;
               setMatchRequired( false );

               BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
               if ( bool( ct ) && ( mp != Country )  && ( mp != District )  && ( mp != Locator ) )
                  addMatch( ct->DupSheet.getCurDup(), ct );	// in case it isn't already

               std::string buff;
               // now make it display
               replaceList( matchCollection );

               if ( thisContestMatched )
               {
                  // conteste focused the top line of matches here
                  // want to manage plurals, and local/other contests
                  std::string matchSuffix = "Possible";
                  if (mp == Country)
                  {
                     matchSuffix = "Country";
                  }
                  else if (mp == District)
                  {
                     matchSuffix = "District";
                  }
                  else if (mp == Locator)
                  {
                     matchSuffix = "Locator";
                  }
                  buff = ( boost::format( " - %s%d %s matches" ) % ( ( cnt > MATCH_LIM ) ? ">" : "" ) % thisContestMatched
                           % ( ( mp == Exact ) ? "" : matchSuffix.c_str() ) ).str();
               }
               else
               {
                  buff = " - No match";
               }
               TMatchThread::getMatchThread() ->ShowMatchStatus( buff );
               contestIndex = -1;
               return false;
            }
         }

      BaseContestLog * ccon;

      if ( firstMatch == MainContest )
      {
         ccon = TContestApp::getContestApp() ->getCurrentContest();		// we always go through current FIRST! to make sure we see it
      }
      else
      {
         ContestSlot *cs = TContestApp::getContestApp() ->contestSlotList.at( contestIndex );
         ccon = cs->slot;
         if ( ccon == TContestApp::getContestApp() ->getCurrentContest() )
            contactIndex = ccon->getContactCount();	// force to go on
      }

      while (  matchStarted && limit > 0 )
      {
//         limit -= 1;
         if ( !ccon ||
              ( contactIndex >= ccon->getContactCount() )
            )
         {
            // we need to move on

            bool next_needed = true;
            contactIndex = 0;

            if ( firstMatch == MainContest )
            {
               // we have done the current ContestLog, go to the rest
               firstMatch = Rest;
               contestIndex = -1;	// as it gets ++ed lower down
            }

            if ( next_needed )
            {
               // go to the next ContestLog
               contestIndex++;
               if ( contestIndex < TContestApp::getContestApp() ->getContestSlotCount() )
               {
                  // ContestLog is valid
                  ContestSlot * cs = TContestApp::getContestApp() ->contestSlotList.at( contestIndex );
                  ccon = cs->slot;
               }
            }
            return true;
         }
         BaseContact *cct = ccon->pcontactAt( contactIndex++ );
         if ( !cct )
            return true;

         // now do the match

         if ( mp != Country && mp != District && mp != Locator )
         {
            bool csmatch = false;
            bool locmatch = false;
            bool qthmatch = false;

            csmatch = matchcs.checkMatch( ( ( mp == Body ) ? ( cct->cs.body ) : ( cct->cs.fullCall.getValue() ) ) );

            if ( csmatch )
            {
               locmatch = matchloc.checkMatch( cct->loc.loc.getValue() );
            }

            if ( csmatch && locmatch )
            {
               if ( !matchqth.match || matchqth.empty )
               {
                  qthmatch = true;
               }
               else
               {
                  TEMPBUFF( uprqth, EXTRALENGTH + 1 );
                  strcpysp( uprqth, cct->extraText.getValue(), EXTRALENGTH );
                  strupr( uprqth );
                  qthmatch = matchqth.checkMatch( uprqth );

                  if ( bool( ccon ) && !qthmatch )
                  {
                     if ( ccon->districtMult.getValue() )
                     {
                        // attempt match against district code
                        if ( cct->districtMult && wildComp( cct->districtMult->districtCode, matchqth.mstr ) )
                           qthmatch = true;
                     }
                  }
               }
            }

            if ( csmatch && locmatch && qthmatch )
            {
               addMatch( cct, ccon );
               if ( matchCollection->matchList.size() > MATCH_LIM )
                  break;
            }
         }
         else if (mp == Country)
         {
            if ( cct->ctryMult && ( cct->ctryMult == ce ) )
            {
               addMatch( cct, ccon );
               if ( matchCollection->matchList.size() > MATCH_LIM )
                  break;
            }

         }
         else if (mp == District)
         {
/*            if ( cct->ctryMult && ( cct->ctryMult == ce ) )
            {
               addMatch( cct, ccon );
               if ( matchCollection->matchList.size() > MATCH_LIM )
                  break;
            }
*/
         }
         else if (mp == Locator)
         {
/*            if ( cct->ctryMult && ( cct->ctryMult == ce ) )
            {
               addMatch( cct, ccon );
               if ( matchCollection->matchList.size() > MATCH_LIM )
                  break;
            }
*/
         }
      }
   }
   catch ( ... )
   {
      trace( "Exception in log idle match" );
   }
   return true;
}
void LogMatcher::replaceList( TMatchCollection *matchCollection )
{
   TMatchThread::getMatchThread() ->replaceContestList( matchCollection );
}
//==============================================================================
ListMatcher::ListMatcher()
{}
ListMatcher::~ListMatcher()
{}
void ListMatcher::matchDistrict( const std::string &/*extraText*/ )
{}
void ListMatcher::matchCountry( const std::string &/*cs*/ )
{}
void ListMatcher::addMatch( ListContact *cct, ContactList * ccon )
{
   if ( !cct )
      return ;
   int i;

   MatchListContact *mct = new MatchListContact( ccon, cct );

   bool exists = std::binary_search( matchCollection->matchList.begin(), matchCollection->matchList.end(), mct );
   if ( !exists )
   {
      matchCollection->matchList.insert( mct );
   }
   else
   {
      delete mct;
   }
}
bool ListMatcher::idleMatch( int limit )
{
   try
   {
      // this is called repeatedly off the timer to execute the match
      if ( !matchStarted || ( contestIndex < 0 ) )
         return false;				// nothing to do (yet)
      if ( TContestApp::getContestApp() ->getListSlotCount() <= 0 )
         return false;

      int cnt = matchCollection->matchList.size();
      if ( ( firstMatch == Starting ) && ( ( mp == Exact ) || ( mp == Country ) || ( mp == District ) || ( mp == Locator ) ) )
      {
         contestIndex = 0;
         contactIndex = 0;
         firstMatch = MainContest;
      }
      else
         if ( ( contestIndex >= TContestApp::getContestApp() ->getListSlotCount() ) || ( cnt > MATCH_LIM ) )
         {
            bool EndScan = true;

            if ( cnt == 0 )
            {
               switch ( mp )
               {
                  case Exact:
                     {
                        if ( matchcs.match )
                        {
                           bool dropthrough = false;
                           const char *c = matchcs.mstr.c_str();
                           // need to trim out any leading and trailing
                           while ( *c && *c != '/' )
                              c++;

                           const char *c2 = c;
                           if ( *c2 )
                              c2++;	// skip the first /
                           while ( *c2 && *c2 != '/' )
                              c2++;

                           if ( *c && *c2 )
                           {
                              matchcs.mstr = std::string( c ).substr( 0, c2 - c );	// copy back over ourselves
                           }
                           else
                              if ( *c && ( c - matchcs.mstr.c_str() < 3 ) && ( strlen( c ) > 2 ) )
                              {
                                 // prefix less than 3 chars and suffix more than 1 character
                                 matchcs.mstr = std::string( c );	// copy back over ourselves
                              }
                              else
                                 if ( *c == '/' )
                                 {
                                    matchcs.mstr = matchcs.mstr.substr( 0, c - matchcs.mstr.c_str() );	// copy back over ourselves
                                    //                                 *c = 0;				// force a stop on the /
                                 }
                                 else
                                 {
                                    // want to drop through
                                    dropthrough = true;
                                 }

                           if ( !dropthrough )
                           {
                              //TMatchThread::getMatchThread()->ShowMatchStatus( " - No exact match" );
                              mp = NoSuffix;
                              contestIndex = 0;
                              contactIndex = 0;
                              firstMatch = MainContest;
                              EndScan = false;
                              break;
                           }
                        }
                     }
                     // or no /P anyway, so drop through

                  case NoSuffix:
                     {
                        if ( matchcs.match && ( matchloc.match || matchqth.match ) )
                        {
                           matchloc.set( "" );
                           matchqth.set( "" );
                           mp = NoLoc;
                           contestIndex = 0;
                           contactIndex = 0;
                           firstMatch = MainContest;
                           EndScan = false;
                           //TMatchThread::getMatchThread()->ShowMatchStatus( " - No match No Suffix" );
                           break;
                        }
                     }
                     // or no loc anyway, so drop through

                  case NoLoc:
                     {
                        if ( matchcs.match )
                        {
                           // We know that cs is not empty
                           // strip temp cs of its leading country and number
                           const char * mstrStart = matchcs.mstr.c_str();
                           const char * c = &mstrStart[ matchcs.mstr.length() ];

                           while ( c > mstrStart )
                           {
                              if ( !isdigit( *( c - 1 ) ) )
                                 c--;
                              else
                                 break;
                           }

                           if ( c > mstrStart )   	// i.e. we havent got back to the start
                           {
                              if ( *c )   			// we would be left with something
                              {
                                 matchcs.mstr = c;	// copy back over
                                 mp = Body;
                                 contestIndex = 0;
                                 contactIndex = 0;
                                 firstMatch = MainContest;
                                 EndScan = false;
                                 //TMatchThread::getMatchThread()->ShowMatchStatus( " - No match No LOC" );
                                 break;
                              }
                           }
                        }
                     }
                     // else we have already done what we can, so drop through

                  case Country:
                  case District:
                  case Locator:
                  case Body:
                     {
                        // we have finished.
                        break;
                     }
               }
            }

            if ( EndScan )
            {
               matchStarted = false;
               setMatchRequired( false );

               std::string buff;
               // now make it display
               replaceList( matchCollection );

               contestIndex = -1;
               return false;
            }
         }

      ContactList * ccon;

      ListSlot *cs = TContestApp::getContestApp() ->listSlotList.at( contestIndex );
      ccon = cs->slot;

      while ( limit > 0 )
      {
//         limit -= 1;
         if ( !ccon ||
              ( contactIndex >= ccon->getContactCount() )
            )
         {
            // we need to move on

            bool next_needed = true;
            contactIndex = 0;

            if ( firstMatch == MainContest )
            {
               // we have done the current ContestLog, go to the rest
               firstMatch = Rest;
               contestIndex = -1;	// as it gets ++ed lower down
            }

            if ( next_needed )
            {
               // go to the next ContestLog
               contestIndex++;
               if ( contestIndex < TContestApp::getContestApp() ->getListSlotCount() )
               {
                  // ContestLog is valid, if it is a list set first part
                  ListSlot * cs = TContestApp::getContestApp() ->listSlotList.at( contestIndex );
                  ccon = cs->slot;
               }
            }
            return true;
         }
         ListContact *cct = ccon->pcontactAt( contactIndex++ );
         if ( !cct )
            return true;

         // now do the match
#warning need country, district, locator matches
// And why aren't we doing a QTH match mere?
         bool csmatch = false;
         bool locmatch = false;

         csmatch = matchcs.checkMatch( ( ( mp == Body ) ? ( cct->cs.body ) : ( cct->cs.fullCall.getValue() ) ) );

         if ( csmatch )
         {
            locmatch = matchloc.checkMatch( cct->loc.loc.getValue() );
         }


         if ( csmatch && locmatch )
         {
            addMatch( cct, ccon );
            if ( matchCollection->matchList.size() > MATCH_LIM )
               break;
         }
      }
   }
   catch ( ... )
   {
      trace( "Exception in list idle match" );
   }
   return true;
}
void ListMatcher::replaceList( TMatchCollection *matchCollection )
{
   TMatchThread::getMatchThread() ->replaceListList( matchCollection );
}

