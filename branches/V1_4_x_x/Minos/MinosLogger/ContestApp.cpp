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
#include "LoggerContest.h"
#include "MatchContact.h"
#include "MatchThread.h"
#include "MultDisp.h"
#include "StatsDisp.h"
#include "shbrowse.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define INITIAL_CONTEST_SLOTS 2
#define INITIAL_LIST_SLOTS 10

double bigClockCorr = 0;
TContestApp *TContestApp::contestApp = 0;

TContestApp *TContestApp::getContestApp()
{
   static bool firstTime = true;
   if ( !contestApp && firstTime )
   {
      firstTime = false;
      contestApp = new TContestApp;
      contestApp->initialise();
   }
   return contestApp;
}
//---------------------------------------------------------------------------

bool isOpen( ContestSlot *cs, const std::string &fn )
{
   BaseContestLog * con = cs->slot;
   if ( !bool( con ) )
      return false;
   if ( stricmp( con->cfileName, fn ) == 0 )
      return true;
   return false;
}
bool isOpen( ListSlot *cs, const std::string &fn )
{
   ContactList * con = cs->slot;
   if ( !bool( con ) )
      return false;
   if ( stricmp( con->cfileName, fn ) == 0 )
      return true;
   return false;
}
ContestSlot::ContestSlot( void ) : slotno( -1 ), slot( 0 )
{}
ContestSlot::~ContestSlot()
{
   if ( slot )
   {
      delete slot;
      slot = 0;
   }
}
ListSlot::ListSlot( void ) : slotno( -1 ), slot( 0 )
{}
ListSlot::~ListSlot()
{
   if ( slot )
   {
      delete slot;
      slot = 0;
   }
}


//---------------------------------------------------------------------------
static void initClock( void )
{
   std::ifstream strm( ".\\Configuration\\time.correction", std::ios::binary ); // should close when it goes out of scope
   if ( strm )
   {
      long big_corr;
      strm >> big_corr;
      bigClockCorr = big_corr / dtg::daySecs;
   }
}
bool TContestApp::initialise()
{

   // Under Linux/Wine (in particular) the current working directory may not
   // be where expected. So reset it to where it should be

   // Eventually, we should have an installation system so that e.g. under Vista
   // we can put things like config under the user directory

   if (!DirectoryExists(".\\Configuration"))
   {
      // try for executable directory
      char appFName[ 255 ];
      int nLen = 0;
      nLen = ::GetModuleFileName( HInstance, appFName, 255 );
      appFName[ nLen ] = '\0';

      String fpath = ExtractFilePath( appFName );

      if (DirectoryExists(fpath + "\\Configuration"))
      {
         chdir(fpath.c_str());
      }
      while (!DirectoryExists(".\\Configuration"))
      {
         std::string destDir;
         if (SimpleBrowseDirectory( destDir,
                                    LogContainer,
                                    "Set Minos Working Directory" ) )
         {
            if (strupr(destDir).find("\\CONFIGURATION") == destDir.size() - std::string("\\configuration").size())
            {
               destDir = destDir.substr(0, destDir.size() - std::string("\\configuration").size());
            }
            chdir(destDir.c_str());
         }
      }
   }
   // delay opening the trace file until we know where to put it
   enableTrace( ".\\TraceLog\\MinosLogger_" );
   // we need to open our bundles...
   // and we need to discover the defaults from the initial splash screen

   // Where do we get the initial defaults from? I think we need an "initial"
   // config file that we can use to store the others...


   //----------------------------------

   BundleFile::bundleFiles[ epLOGGERPROFILE ] = boost::shared_ptr<BundleFile>( new BundleFile( epLOGGERPROFILE ) );
   BundleFile::bundleFiles[ epPRELOADPROFILE ] = boost::shared_ptr<BundleFile>( new BundleFile( epPRELOADPROFILE ) );
   BundleFile::bundleFiles[ epDISPLAYPROFILE ] = boost::shared_ptr<BundleFile>( new BundleFile( epDISPLAYPROFILE ) );
   BundleFile::bundleFiles[ epENTRYPROFILE ] = boost::shared_ptr<BundleFile>( new BundleFile( epENTRYPROFILE ) );
   BundleFile::bundleFiles[ epQTHPROFILE ] = boost::shared_ptr<BundleFile>( new BundleFile( epQTHPROFILE ) );
   BundleFile::bundleFiles[ epSTATIONPROFILE ] = boost::shared_ptr<BundleFile>( new BundleFile( epSTATIONPROFILE ) );

   //----------------------------------
   BundleFile::bundleFiles[ epLOGGERPROFILE ] ->openProfile( ".\\Configuration\\MinosLogger.ini", "Logger Defaults" );
   loggerBundle.setProfile( BundleFile::bundleFiles[ epLOGGERPROFILE ] );
   loggerBundle.openSection( "Default" );

   //----------------------------------
   std::string preloadfile;
   loggerBundle.getStringProfile( elpPreloadFile, preloadfile );
   BundleFile::bundleFiles[ epPRELOADPROFILE ] ->openProfile( preloadfile, "Preload options" );

   preloadBundle.setProfile( BundleFile::bundleFiles[ epPRELOADPROFILE ] );

   std::string preloadsect;
   loggerBundle.getStringProfile( elpPreloadSection, preloadsect );
   preloadBundle.openSection( preloadsect );
   //----------------------------------

   std::string dispfile;
   loggerBundle.getStringProfile( elpDisplayFile, dispfile );
   BundleFile::bundleFiles[ epDISPLAYPROFILE ] ->openProfile( dispfile, "Display defaults" );

   displayBundle.setProfile( BundleFile::bundleFiles[ epDISPLAYPROFILE ] );

   std::string dispsect;
   loggerBundle.getStringProfile( elpDisplaySection, dispsect );
   displayBundle.openSection( dispsect );
   //----------------------------------

   std::string entfile;
   loggerBundle.getStringProfile( elpEntryFile, entfile );
   BundleFile::bundleFiles[ epENTRYPROFILE ] ->openProfile( entfile, "Contest Entry details" );
   //----------------------------------

   std::string qthfile;
   loggerBundle.getStringProfile( elpQTHFile, qthfile );
   BundleFile::bundleFiles[ epQTHPROFILE ] ->openProfile( qthfile, "QTH details" );
   //----------------------------------

   std::string stationfile;
   loggerBundle.getStringProfile( elpStationFile, stationfile );
   BundleFile::bundleFiles[ epSTATIONPROFILE ] ->openProfile( stationfile, "Station details" );
   //----------------------------------

   /*
   std::string temp;
   int itemp;
   displayBundle.getStringProfile( edpFontName, temp );
   if ( temp.size() )
      sysfont->Name = temp.c_str();

   displayBundle.getIntProfile( edpFontSize, itemp );
   if ( itemp != 0 )
      sysfont->Size = itemp;
   */
   initClock();
   TMatchThread::InitialiseMatchThread();

   return true;
}
bool contestAppLoadFiles( void )
{
   return MultLists::getMultLists(); // ->loadMultFiles();
}
void closeContestApp( void )
{
   if ( TContestApp::getContestApp() )
   {
      TContestApp::getContestApp() ->close();
   }
   delete TContestApp::getContestApp();
}
void TContestApp::close()
{
   writeContestList();
   TMatchThread::FinishMatchThread();
}
TContestApp::TContestApp() : MinosParameters(), magneticVariation( 0 ), period1( 5 ), period2( 20 ),
      reverseBearing( false ), dispCountry( false ), dispBearing( true ), dispScore( true ),
      currentContest( 0 )

{
   sysfont = new TFont();
   sysfont->Name = "Verdana";
   sysfont->Size = 8;

   for ( int i = 0; i < INITIAL_CONTEST_SLOTS; i++ )
   {
      ContestSlot *cs = new ContestSlot;
      cs->slotno = i + 1;
      //      contestSlotList.push_back( cs );
      contestSlotList.insert( cs );
   }
   for ( int i = 0; i < INITIAL_LIST_SLOTS; i++ )
   {
      ListSlot *cs = new ListSlot;
      cs->slotno = i + 1;
      listSlotList.insert( cs );
   }
}
TContestApp::~TContestApp()
{
   freeAll();           // closes slots - and maybe bundles

   loggerBundle.closeProfile();
   preloadBundle.closeProfile();
   displayBundle.closeProfile();
   contestApp = 0;

   delete MultLists::getMultLists();
   CsGuard::ClearDown();
}

bool TContestApp::insertContest( BaseContestLog * p, unsigned int sno )
{

   // called from LoggerContestLog::initialise
   if ( ( int ) sno != -1 )
   {
      if ( sno >= contestSlotList.size() )
      {
         for ( unsigned int i = contestSlotList.size(); i <= sno; i++ )
         {
            ContestSlot *cs = new ContestSlot;
            cs->slotno = i + 1;
            //            contestSlotList.push_back( cs );
            contestSlotList.insert( cs );
         }
      }
      ContestSlot *cs = contestSlotList[ sno ];
      if ( bool( cs->slot ) )
         return false;
      else
      {
         cs->slot = p;
         return true;
      }
   }
   for ( unsigned int i = 0; i < contestSlotList.size(); i++ )
   {
      ContestSlot *cs = contestSlotList[ i ];
      if ( !bool( cs->slot ) )
      {
         cs->slot = p;
         return true;
      }
   }
   ContestSlot *cs = new ContestSlot;
   cs->slot = p;
   cs->slotno = contestSlotList.size() + 1;
   //   contestSlotList.push_back( cs );
   contestSlotList.insert( cs );
   return true;
}
bool TContestApp::insertList( ContactList * p, unsigned int sno )
{

   // called from LoggerContestLog::initialise
   if ( ( int ) sno != -1 )
   {
      if ( sno >= listSlotList.size() )
      {
         for ( unsigned int i = listSlotList.size(); i <= sno; i++ )
         {
            ListSlot *cs = new ListSlot;
            cs->slotno = i + 1;
            //            contestSlotList.push_back( cs );
            listSlotList.insert( cs );
         }
      }
      ListSlot *cs = listSlotList[ sno ];
      if ( cs->slot )
         return false;
      else
      {
         cs->slot = p;
         return true;
      }
   }
   for ( unsigned int i = 0; i < listSlotList.size(); i++ )
   {
      ListSlot *cs = listSlotList[ i ];
      if ( !cs->slot )
      {
         cs->slot = p;
         return true;
      }
   }
   ListSlot *cs = new ListSlot;
   cs->slot = p;
   cs->slotno = listSlotList.size() + 1;
   //   contestSlotList.push_back( cs );
   listSlotList.insert( cs );
   return true;
}
int TContestApp::getOccupiedListSlotCount()
{
   int cnt = 0;
   for ( ListSlotIterator i = listSlotList.begin(); i != listSlotList.end(); i++ )
   {
      if ( ( *i ) && ( *i ) ->slot )
      {
         cnt++;
      }
   }
   return cnt;
}

bool TContestApp::isListOpen( const std::string fn )
{
   for ( ListSlotIterator i = listSlotList.begin(); i != listSlotList.end(); i++ )
   {
      if ( isOpen( ( *i ), fn ) )
      {
         mshowMessage( String( "File " ) + fn.c_str() + " is already open!" );
         return true;
      }
   }
   return false;
}
bool TContestApp::isContestOpen( const std::string fn )
{
   for ( SlotIterator i = TContestApp::getContestApp() ->contestSlotList.begin(); i != TContestApp::getContestApp() ->contestSlotList.end(); i++ )
   {
      if ( isOpen( ( *i ), fn ) )
      {
         MinosParameters::getMinosParameters() ->mshowMessage( String( "File " ) + fn.c_str() + " is already open!" );
         return true;
      }
   }
   return false;
}


BaseContestLog * TContestApp::findFirstContest()
{
   for ( unsigned int i = 0; i < contestSlotList.size(); i++ )
   {
      ContestSlot *cs = contestSlotList[ i ];
      if ( bool( cs->slot ) )
      {
         return cs->slot;
      }
   }
   return 0;
}
/*
LoggerContestLog * TContestApp::findContest( LoggerContestLog *p )
{
   for ( unsigned int i = 0; i < contestSlotList.size(); i++ )
   {
      ContestSlot *cs = contestSlotList[ i ];
      if ( cs->slot.get() == p )
      {
         return cs->slot;
      }
   }
   return LoggerContestLog *();
}
*/
int TContestApp::findContest( BaseContestLog * p )
{
   for ( unsigned int i = 0; i < contestSlotList.size(); i++ )
   {
      ContestSlot *cs = contestSlotList[ i ];
      if ( cs->slot && p == cs->slot )
      {
         return i;
      }
   }
   return -1;
}
int TContestApp::findList( ContactList * p )
{
   for ( unsigned int i = 0; i < listSlotList.size(); i++ )
   {
      ListSlot *cs = listSlotList[ i ];
      if ( cs && cs->slot && p == cs->slot )
      {
         return i;
      }
   }
   return -1;
}
void TContestApp::removeContest( BaseContestLog * p )
{
   int i = findContest( p );
   if ( i >= 0 )
   {
      if ( getCurrentContest() == p )
         setCurrentContest( 0 );
      ContestSlot * cs = contestSlotList[ i ];
      cs->slot = 0;
   }
}
void TContestApp::removeList( ContactList * p )
{
   int i = findList( p );
   if ( i >= 0 )
   {
      ListSlot * cs = listSlotList[ i ];
      cs->slot = 0;
   }
}
void TContestApp::writeContestList()
{
   preloadBundle.clearProfileSection( );
   for ( unsigned int i = 0; i < contestSlotList.size(); i++ )
   {
      ContestSlot *cs = contestSlotList[ i ];
      BaseContestLog * ct = cs->slot;
      if ( !ct )
         continue;

      std::string ent = ( boost::format( "%d" ) % cs->slotno ).str();
      preloadBundle.setStringProfile( ent.c_str(), ct->cfileName.c_str() );
      if ( currentContest == ct )
      {
         preloadBundle.setIntProfile( eppCurrent, cs->slotno );
      }
   }
   for ( unsigned int i = 0; i < listSlotList.size(); i++ )
   {
      ListSlot *cs = listSlotList[ i ];
      ContactList * ct = cs->slot;
      if ( !ct )
         continue;

      std::string ent = ( boost::format( "List%d" ) % cs->slotno ).str();
      preloadBundle.setStringProfile( ent.c_str(), ct->cfileName.c_str() );
      // no need for any concept of a "current" list
   }
   preloadBundle.flushProfile();
}
LoggerContestLog * TContestApp::openFile( const std::string &fn, bool newFile, int slotno )
{
   LoggerContestLog * contest = new LoggerContestLog();

   if ( !contest->initialise( fn, newFile, slotno ) )    // this adds it to the slot
   {
      closeFile( contest );
      return 0;
   }

   return contest;
}
ContactList * TContestApp::openListFile( const std::string &fn, int slotno )
{
   ContactList * list = new ContactList();
   if ( !list->initialise( fn, slotno ) )    // this adds it to the slot
   {
      removeList( list ); 		// must remove LoggerContestLog from its slot
      delete list;
      return 0;
   }

   return list;
}
void TContestApp::closeFile( BaseContestLog * contest )
{
   if ( contest )
   {
      removeContest( contest ); 		// must remove LoggerContestLog from its slot
      delete contest;
   }

   writeContestList();
}
// Predicate function for remove_if
bool noslot( ListSlot *ip )
{
   if ( ip == 0 )
      return true;
   else
      return false;
}
void TContestApp::closeListFile( ContactList *list )
{
   if ( list )
   {
      removeList( list ); 		// must remove LoggerContestLog from its slot
      delete list;
   }
   listSlotList.erase( std::remove_if( listSlotList.begin(), listSlotList.end(), noslot ), listSlotList.end() );
   writeContestList();
}
BaseContestLog * TContestApp::getCurrentContest()
{
   return currentContest;
}
void TContestApp::setCurrentContest( BaseContestLog * c )
{
   currentContest = c;
   int cc = findContest( c );
   preloadBundle.setIntProfile( eppCurrent, cc );
   preloadBundle.flushProfile();
}

void TContestApp::getDisplayColumnWidth( const std::string &key, int &val, int def )
{
   displayBundle.getIntProfile( key.c_str(), val, def );
}
void TContestApp::setDisplayColumnWidth( const std::string &key, int val )
{
   if ( val < 0 )
   {
      displayBundle.setStringProfile( key.c_str(), 0 );
   }
   else
   {
      displayBundle.setIntProfile( key.c_str(), val );
   }

}
void TContestApp::getBoolDisplayProfile( int enumkey, bool &value )
{
   displayBundle.getBoolProfile( enumkey, value );
}
void TContestApp::setBoolDisplayProfile( int enumkey, bool value )
{
   displayBundle.setBoolProfile( enumkey, value );
}
void TContestApp::flushDisplayProfile( void )
{
   displayBundle.flushProfile();
}
double TContestApp::getBigClockCorrection()
{
   return bigClockCorr;
}

int TContestApp::getStatsPeriod1()
{
   displayBundle.getIntProfile( edpStatisticsPeriod1, period1 );
   return period1;
}
int TContestApp::getStatsPeriod2()
{
   displayBundle.getIntProfile( edpStatisticsPeriod2, period2 );
   return period2;
}
void TContestApp::setStatsPeriod1( int s1 )
{
   period1 = s1;
   displayBundle.setIntProfile( edpStatisticsPeriod1, period1 );
   displayBundle.flushProfile();
}
void TContestApp::setStatsPeriod2( int s2 )
{
   period2 = s2;
   displayBundle.setIntProfile( edpStatisticsPeriod2, period2 );
   displayBundle.flushProfile();
}
bool TContestApp::getAllowLoc4()
{
   BaseContestLog * ct = TContestApp::getCurrentContest();
   if ( !ct )
   {
      return false;
   }
   return ct->allowLoc4.getValue();
}
bool TContestApp::getAllowLoc8()
{
   BaseContestLog * ct = TContestApp::getCurrentContest();
   if ( !ct )
   {
      return false;
   }
   return ct->allowLoc8.getValue();
}
bool TContestApp::yesNoMessage( TComponent* Owner, String mess )
{
   return mShowYesNoMessage( Owner, mess );
}
void TContestApp::mshowMessage( String mess, TComponent* Owner )
{
   mShowMessage( mess, Owner );
}

