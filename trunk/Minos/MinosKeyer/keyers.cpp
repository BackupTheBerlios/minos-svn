/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "MinosVK_pch.h"
#pragma hdrstop

#include <process.h>
#include "keyers.h"
#include "portcon.h"
#include "vkmixer.h"
#include "sbdriver.h"

//==============================================================================

std::map <char, std::string> MORSECODE;    // . is 0x40, - is 0x80
std::map <int, MORSEMSG> MORSEMSGS;

CRITICAL_SECTION disableInterrupts::intCriticalSection;
bool disableInterrupts::terminated = true;
//==============================================================================

// some of these might not belong here, but they were in sbdvp or tlkeyer
// and I still need them somewhere
bool sblog = false;
bool sbintrlog = false;
unsigned long MORSEINTCOUNT = 0;

int tuneTime = 10;
double tuneLevel = 80.0;

int CWTone = 1000;
int CWSpeed = 12;

unsigned long currTick = 0;
unsigned long basetick = 0;
my_deque < KeyerAction *> KeyerAction::currentAction;
//=============================================================================

commonKeyer *currentKeyer = 0;

//=============================================================================
bool keyer_docommand( const KeyerCtrl &keyer_ctrl )
{
   if ( sblog )
   {
      trace( "docommand(" + makeStr( keyer_ctrl.command ) + ")" );
   }

   if ( currentKeyer )
      return currentKeyer->docommand( keyer_ctrl );
   return false;
}
//==========================================================================
void startRecordDVPFile( int fileno )
{
   char buff[ 20 ];
   sprintf( buff, "CQF%d.WAV", fileno );
   keyer_docommand( KeyerCtrl( eKEYER_RECORD, fileno, 0, 0, buff ) );
}
void finishRecordDVPFile( void )
{
   keyer_docommand( KeyerCtrl( eKEYER_STOPRECORD, -1, 0, 0, "" ) );
}
void playKeyerFile( int fileno, bool ptt )
{
   char buff[ 20 ];
   sprintf( buff, "CQF%d.WAV", fileno );
   keyer_docommand( KeyerCtrl( eKEYER_PLAY, fileno, 0, 0, buff, ptt ) );
}
void sendTone1( void )
{
   KeyerInfo ki;
   if ( getKeyerInfo( &ki ) )
   {
      keyer_docommand( KeyerCtrl( eKEYER_TONE1, ki.tone1, 0, ki.tuneTime, "" ) );
   }
}
void sendTone2( void )
{
   KeyerInfo ki;
   if ( getKeyerInfo( &ki ) )
   {
      keyer_docommand( KeyerCtrl( eKEYER_TONE2, ki.tone1, ki.tone2, ki.tuneTime, "" ) );
   }
}
void stopKeyer()
{
   keyer_docommand( KeyerCtrl( eKEYER_STOPALL, -1, 0, 0, "" ) );
}
//==========================================================================
bool keyer_config( std::string & /*errmess*/ )
{
   configureKeyers();   // which goes off to XML, which drives the keyer configuration
   return true;
}
bool keyer_init( std::string &errmess )
{
   disableInterrupts::initialise();
   if ( sblog )
   {
      trace( "newkeyer_init called" );   // make sure file is open
   }

   currTick = 0;
   basetick = GetTickCount();

   // Use the mixer as it is - don't attempt to save/reset
   // If needed, use the mixer utility program for that!

   if ( !keyer_config( errmess ) )
      return false;
   return true;
}
bool select_keyer( const std::string &kn )
{
   if ( sblog )
   {
      trace( "Select keyer " + kn + " called." );
   }
   keyer_docommand( KeyerCtrl( eKEYER_STOPALL, -1, 0, 0, "" ) );
   int i = 0;

   for ( std::deque <commonPort *>::iterator icp = portChain.begin(); icp != portChain.end(); icp++ )
   {
      commonPort *cp = *icp;
      for ( std::deque <lineMonitor *>::iterator ilm = cp->monitors.begin(); ilm != cp->monitors.end(); ilm++ )
      {
         lineMonitor *lm = ( *ilm );
         if ( stricmp( kn.c_str(), lm->pName.c_str() ) == 0 )
         {
            if ( currentKeyer )
               currentKeyer->select( false );
            currentKeyer = ( commonKeyer * ) lm;
            if ( sblog )
            {
               trace( "Keyer " + kn + " made current." );
            }
            currentKeyer->select( true );
            return true;
         }
         i++;
      }
   }

   return false;
}
void loadKeyers()
{
   std::string buff;
   bool KeyerLoaded = keyer_init( buff );	// params are argc, argv fo DVP control strings
   if ( KeyerLoaded )
   {
      std::vector < std::string > kl = get_keyer_list();
      if ( kl.size() )
      {
         select_keyer( kl[ 0 ] );
      }
   }
   else
      ShowMessage( buff.c_str() );
}
void unloadKeyers()
{
   if ( sblog )
   {
      trace( "newkeyer_unload" );
   }

   disableInterrupts guard;
   KeyerAction::currentAction.freeAll();
   // stop each linemonitor and ticker
   baseTimer::killTimer();

   currentKeyer = 0;

   // close down each port
   for ( std::deque <commonPort *>::iterator icp = portChain.begin(); icp != portChain.end(); icp++ )
   {
      if ( *icp )
         ( *icp ) ->closePort();
   }

   // close down the SB card
   sbDriver::sbdvp_unload();
   // unload everything - destructors SHOULD sort the mess out!
   portChain.freeAll();

   // Use the mixer utility if we need to save/reset mixer settings
   disableInterrupts::terminate();
}
bool getPTT()
{
   if ( currentKeyer )
      return currentKeyer->getPTT();
   return false;
}
void getActionState( std::string &s )        // returns currnnt keyer action state string
{
   if ( currentKeyer )
      currentKeyer->getActionState( s );
   else
      s = "No current keyer";
}
bool getKeyerStatus( std::string &s )
{
   if ( currentKeyer )
      return currentKeyer->getStatus( s );
   s = "";
   return false;
}
unsigned long keyer_getintcount()
{
   return MORSEINTCOUNT;
}
std::vector < std::string > get_keyer_list()
{
   std::vector < std::string > keylist;
   // go through portChain, and look down each namechain of linemonitors
   keylist.clear();
   for ( std::deque <commonPort *>::iterator icp = portChain.begin(); icp != portChain.end(); icp++ )
   {
      commonPort *cp = *icp;
      for ( std::deque <lineMonitor *>::iterator ilm = cp->monitors.begin(); ilm != cp->monitors.end(); ilm++ )
      {
         lineMonitor *lm = ( *ilm );
         if ( dynamic_cast<commonKeyer *>( lm ) )
         {
            keylist.push_back( lm->pName );
         }
      }
   }
   return keylist;
}
bool getKeyerInfo( KeyerInfo *ki )
{
   // pass back the name and speed of the current keyer
   if ( currentKeyer )
   {
      return currentKeyer->getInfo( ki );
   }
   return false;
}
void setLines( bool PTT, bool L1, bool L2 )
{
   WindowsMonitorPort::PTTInState = PTT;
   WindowsMonitorPort::L1State = L1;
   WindowsMonitorPort::L2State = L2;
}
void setLineCallBack( LineCallBack lcallback )
{
   WindowsMonitorPort::WinLineCallback = lcallback;
   WinMonitor::WinLineCallback = lcallback;
   LineEventsPort::WinLineCallback = lcallback;
}
void setVUCallBack( VUCallBack in, VUCallBack out )
{
   sbDriver::getSbDriver() ->WinVUInCallback = in;
   sbDriver::getSbDriver() ->WinVUOutCallback = out;
}
int getAutoRepeatDelay()
{
   if ( currentKeyer )
   {
      return currentKeyer->kconf.autoRepeatDelay;
   }
   return 0;
}
void setAutoRepeatDelay( int d )
{
   if ( currentKeyer )
   {
      currentKeyer->kconf.autoRepeatDelay = d;
   }
}
bool getEnableAutoRepeat()
{
   if ( currentKeyer )
   {
      return currentKeyer->kconf.enableAutoRepeat;
   }
   return false;
}
void setEnableAutoRepeat( bool b )
{
   if ( currentKeyer )
   {
      currentKeyer->kconf.enableAutoRepeat = b;
   }
}

bool getPipEnabled()
{
   if ( currentKeyer )
   {
      return currentKeyer->kconf.enablePip;
   }
   return false;
}
void setPipEnabled( bool val )
{
   if ( currentKeyer )
   {
      currentKeyer->kconf.enablePip = val;
   }
}

int getCWSpeed()
{
   return CWSpeed;
}
void setCWSpeed( int s )
{
   CWSpeed = s;
}
int getCWTone()
{
   return CWTone;
}
void setCWTone( int t )
{
   CWTone = t;
}

bool sendCW( const char *message, int speed, int tone )
{
   if ( currentKeyer )
   {
      return currentKeyer->sendCW( message, speed, tone );
   }
   return false;
}
lineMonitor::lineMonitor( const std::string pname )
      : pName( pname )
{}
lineMonitor::lineMonitor( const KeyerConfig &keyer, const PortConfig &port )
      : pName( keyer.name ), kconf( keyer ), pconf( port )
{
   cp = createPort( port );
}
lineMonitor::~lineMonitor()
{
   if ( sblog )
   {
      trace( "lineMonitor::~lineMonitor" );
   }
}
bool lineMonitor::initialise( const KeyerConfig &/*keyer*/, const PortConfig &/*port*/ )
{
   if ( cp )
   {
      cp->registerMonitor( this );
   }
   return true;
}

void lineMonitor::checkControls( void )
{
   if ( cp )
      cp->checkControls();
}

void lineMonitor::ptt( int state )
{
   if ( sblog )
   {
      trace( std::string( "PTT set to " ) + makeStr( state ) );
   }
   if ( cp )
      cp->ptt( state );
}
void lineMonitor::key( int state )
{
   if ( cp )
      cp->key( state );
}

//==============================================================================

// static members
baseTimer *baseTimer::tt = 0;
int baseTimer::ttcount = 0;
HANDLE baseTimer::hTimerThread = 0;
HANDLE baseTimer::hTimerThreadCloseEvent = 0;

void baseTimer::timerThread()
{
   GJV_thread::setThreadName( "hires timer" );
   currTick = 0;

   basetick = GetTickCount();


   TIMECAPS tc;

   if ( timeGetDevCaps( &tc, sizeof( TIMECAPS ) ) != TIMERR_NOERROR )
   {
      // Error; application can't continue.
      MessageBox( 0, "Timer", "GetDevCaps", MB_ICONERROR | MB_OK );
   }

   UINT wTimerRes = std::min( std::max( tc.wPeriodMin, TARGET_RESOLUTION ), tc.wPeriodMax );
   timeBeginPeriod( wTimerRes );
   MMRESULT timeEventId = timeSetEvent( TIMER_INTERVAL, 0, intrTick, 0, TIME_PERIODIC );
   if ( sblog )
   {
      trace( "baseTimer::wTimerRes " + makeStr( wTimerRes ) );
   }

   hTimerThreadCloseEvent = CreateEvent(
                               NULL,     // no security attributes
                               TRUE,     // manual reset event
                               FALSE,      // not-signalled
                               NULL );  // no name

   while ( 1 )
   {
      // Wait for request to close or for a read thread to terminate.
      DWORD dwWait = WaitForSingleObject( hTimerThreadCloseEvent, 1000 );

      if ( dwWait == WAIT_TIMEOUT )
         continue;

      CloseHandle( hTimerThreadCloseEvent );
      if ( timeEventId )
      {
         timeKillEvent( timeEventId );
         timeEventId = 0;
      }

      if ( wTimerRes )
      {
         timeEndPeriod( wTimerRes );
         wTimerRes = 0;
      }
      return ;
   }

}
/*static */unsigned __stdcall baseTimer::timerThread(    LPVOID lpThreadParameter )
{
   baseTimer * b = ( baseTimer * ) lpThreadParameter;
   b->timerThread();

   return 0;
}


void baseTimer::tickEvent()
{
   // need to go through the keyer chain
   for ( std::deque <timerTicker *>::iterator tt = tickers.begin(); tt != tickers.end(); tt++ )
   {
      if ( ( *tt ) ->ready )
         ( *tt ) ->tickEvent();
   }
}
void baseTimer::registerTicker( timerTicker *k )
{
   tickers.push_back( k );
}

void CALLBACK baseTimer::intrTick( UINT /*wTimerID*/, UINT /*msg*/,
                                   DWORD /*dwUser*/, DWORD /*dw1*/, DWORD /*dw2*/ )
{
   // NB we may need to have a fine timer around for CW - at a variable rate
   currTick++;
   if ( tt )
      tt->tickEvent();
}
/*static*/baseTimer *baseTimer::initialiseTimer()
{
   if ( !tt )
   {
      tt = new baseTimer();
   }
   ttcount++;
   return tt;
}
/*static*/void baseTimer::killTimer()
{
   if ( sblog )
   {
      trace( "baseTimer::killTimer" );
   }
   if ( tt )
      tt->tickers.clear();

   SetEvent( hTimerThreadCloseEvent );
   WaitForSingleObject( hTimerThread, INFINITE );

}
/*static*/void baseTimer::closeTimer()
{
   if ( --ttcount <= 0 )
   {
      killTimer();
      delete tt;
      tt = 0;
   }
}
// take the clock interrupt, use it to tick
baseTimer::baseTimer()
{

   unsigned int dwTimerThreadId;
   // NB that ABOVE_NORMAL is a Win2K invention
   hTimerThread = ( HANDLE ) _beginthreadex( NULL,                    // security
                  0,                        // stacksize
                  timerThread,            // start
                  this,                   // arg
                  //                  HIGH_PRIORITY_CLASS,     // createflags
                  NORMAL_PRIORITY_CLASS,     // createflags
                  &dwTimerThreadId           // threadid
                                           );
}
baseTimer::~baseTimer()
{
   if ( sblog )
   {
      trace( "baseTimer::~baseTimer" );
   }
   killTimer();
}

//==============================================================================
timerTicker::timerTicker() : ready( false )
{
   b = baseTimer::initialiseTimer();
   b->registerTicker( this );
}
timerTicker::~timerTicker()
{
   if ( sblog )
   {
      trace( "timerTicker::~timerTicker" );
   }
   ready = false;
   b->closeTimer();
}

//==============================================================================

commonKeyer::commonKeyer( const KeyerConfig &keyer, const PortConfig &port )
      : lineMonitor( keyer, port ), started( false ), startcount( 20 ),
      boxRecPending( false ),
      recPending( false ),
      L1State( false ), L2State( false ),
      pttState( false ),
      cwRate( 0.0 ), tickcount( -1 ), lastIntCount( 0 ),
      tone1( 650 ), tone2( 1250 ), inTone( false )
{}
commonKeyer::~commonKeyer()
{
   if ( sblog )
   {
      trace( "commonKeyer::~commonKeyer" );
   }
}
void commonKeyer::tickEvent()
{
   // we have a timer tick; do what is needed
   if ( !started )
   {
      started = ( --startcount < 0 );
   }
}
bool commonKeyer::getPTT( void )
{
   return started & pttState;
}
void commonKeyer::getActionState( std::string &s )
{
   disableInterrupts guard;
   KeyerAction * sba = KeyerAction::getCurrentAction();
   if ( sba )
      sba->getActionState( s );
   else
      s = "No current actions";
}

bool commonKeyer::getStatus( std::string &buff )
{
   disableInterrupts guard;
   buff = "[";
   buff += ( pttState ? "PTT " : "" );
   buff += ( L1State ? "1 " : "" );
   buff += ( L2State ? "2 " : "" );
   for ( std::deque < KeyerAction *>::iterator isba = KeyerAction::currentAction.begin(); isba != KeyerAction::currentAction.end(); isba++ )
   {
      buff += ( *isba ) ->statusLetter();
   }
   buff += "]";

   return ( KeyerAction::getCurrentAction() ) ? true : false;
}
bool commonKeyer::initialise( const KeyerConfig &keyer, const PortConfig &port )
{
   if ( kconf.pipTone >= 0 )
   {
      if ( kconf.pipTone < 500 )
         kconf.pipTone = 500;
      if ( kconf.pipTone > 1500 )
         kconf.pipTone = 1500;
   }
   bool ret = lineMonitor::initialise( keyer, port );
   timerTicker::ready = true;
   return ret;
}
void commonKeyer::select( bool /*sel*/ )
{}
bool commonKeyer::L1Changed( int state )
{
   if ( sblog )
   {
      trace( "L1Changed(" + makeStr( state ) + ")" );
   }
   L1State = state;
   return true;
}
bool commonKeyer::L2Changed( int state )
{
   if ( sblog )
   {
      trace( "L2Changed(" + makeStr( state ) + ")" );
   }
   L2State = state;
   return true;
}
bool commonKeyer::pttChanged( int state )
{
   if ( sblog )
   {
      trace( "pttChanged(" + makeStr( state ) + ")" );
   }
   pttState = state;
   return true;
}
void commonKeyer::queueFinished()
{}
//==============================================================================

voiceKeyer::voiceKeyer( const KeyerConfig &keyer, const PortConfig &port )
      : commonKeyer( keyer, port ), sbKeyer()
{}
voiceKeyer::~voiceKeyer()
{
   if ( sblog )
   {
      trace( "voiceKeyer::~voiceKeyer" );
   }
}

bool voiceKeyer::docommand( const KeyerCtrl &dvp_ctrl )
{
   if ( sblog )
   {
      trace( "docommand(" + makeStr( dvp_ctrl.command ) + ")" );
   }
   disableInterrupts guard;
   switch ( dvp_ctrl.command )
   {
      case eKEYER_STOPALL:      /* kill audio */
         {
            sbDriver::getSbDriver() ->stopall();
            KeyerAction::currentAction.freeAll();
            SetCurrentMixerSet( emsPassThroughNoPTT );
            break;
         }

      case eKEYER_PLAY:      /* transmit file */
         {
            KeyerAction::currentAction.clear_after( KeyerAction::getCurrentAction() );
            new PlayAction( dvp_ctrl.filename, !dvp_ctrl.xmit, kconf.startDelay, kconf.autoRepeatDelay, true, false );
         }
         break;

      case eKEYER_RECORD:      /* record file */
         {
            KeyerAction::currentAction.freeAll();
            new RecordAction( dvp_ctrl.filename );
            break;
         }

      case eKEYER_STOPRECORD:      /* stop recording file */
         sbDriver::getSbDriver() ->stoprec();
         KeyerAction::currentAction.freeAll();
         break;

      case eKEYER_TONE1:
         {
            KeyerAction *sba = KeyerAction::getCurrentAction();
            sbDriver::getSbDriver() ->stopall();
            KeyerAction::currentAction.freeAll();
            if ( !sba )
            {
               tone1 = dvp_ctrl.intParam1;
               tuneTime = dvp_ctrl.intTime;
               initTone1( tone1 );
               new ToneAction( 1, kconf.startDelay );
            }
         }
         break;

      case eKEYER_TONE2:
         {
            KeyerAction *sba = KeyerAction::getCurrentAction();
            sbDriver::getSbDriver() ->stopall();
            KeyerAction::currentAction.freeAll();
            if ( !sba )
            {
               tone1 = dvp_ctrl.intParam1;
               tuneTime = dvp_ctrl.intTime;
               tone2 = dvp_ctrl.intParam2;
               initTone2( tone1, tone2 );
               new ToneAction( 2, kconf.startDelay );
            }
         }
         break;

      case eKEYER_VOXHANG:
         kconf.voxHangTime = dvp_ctrl.intParam1;
         break;

      default:
         break;
   }
   return true;
}
bool voiceKeyer::getInfo( KeyerInfo *ki )
{
   ki->kname = pName;
   ki->kspeed = 0;
   ki->CW = false;
   ki->voice = true;
   ki->voxHang = kconf.voxHangTime;
   ki->tune = true;
   ki->twoTone = true;
   ki->tone1 = tone1;
   ki->tuneTime = tuneTime;
   ki->tuneLevel = tuneLevel;
   ki->tone2 = tone2;
   ki->CWTone = CWTone;
   ki->CWSpeed = CWSpeed;
   return true;
}
bool voiceKeyer::pttChanged( int state )
{
   commonKeyer::pttChanged( state );
   if ( started && currentKeyer == this )
   {
      // with current wiring this is PTT from microphone
      // NB called within the serial interrupt routine

      // we need to also take note of PTT being used as a record trigger

      disableInterrupts guard;
      KeyerAction * sba = KeyerAction::getCurrentAction();
      if ( sba )
         sba->pttChanged( state );
      else
         if ( state )
         {
            // no current action...
            new InitialPTTAction();
            KeyerAction::getCurrentAction() ->timeOut();
         }
   }
   return true;
}
bool voiceKeyer::L1Changed( int state )
{
   // This should be part of the keyer...
   commonKeyer::L1Changed( state );
   if ( started && currentKeyer == this )
   {
      if ( !state && !recPending && !boxRecPending )
      {
         // will be CQ1 or dit paddle	 RELEASED
         // NB called within the serial interrupt routine

         disableInterrupts guard;
         KeyerAction * ca = KeyerAction::getCurrentAction();
         if ( !ca || !ca->playingFile( "CQF1.WAV" ) )
         {
            KeyerAction::currentAction.clear_after( ca );
            new PlayAction( "CQF1.WAV", false, kconf.startDelay, kconf.enableAutoRepeat ? kconf.autoRepeatDelay : 0, true, false );
            KeyerAction::getCurrentAction() ->LxChanged( eL1, state );
         }
      }
      else
         if ( state )
         {
            if ( L2State && L1State )
            {
               // use this as reset, as well as start rec sequence

               sbDriver::getSbDriver() ->stopall();                       // make sure nothing is happening
               KeyerAction::currentAction.freeAll();	// clear all the chains

               new BoxRecordAction();
            }
            else
               if ( boxRecPending )
                  KeyerAction::getCurrentAction() ->LxChanged( eL1, state );
         }
   }
   return true;
}
bool voiceKeyer::L2Changed( int state )
{
   commonKeyer::L2Changed( state );
   if ( started && currentKeyer == this )
   {
      if ( !state && !recPending && !boxRecPending )
      {
         // will be CQ2 or dah paddle
         // NB called within the serial interrupt routine

         disableInterrupts guard;
         KeyerAction * ca = KeyerAction::getCurrentAction();
         if ( !ca || !ca->playingFile( "CQF2.WAV" ) )
         {
            KeyerAction::currentAction.clear_after( ca );
            new PlayAction( "CQF2.WAV", false, kconf.startDelay, kconf.enableAutoRepeat ? kconf.autoRepeatDelay : 0, true, false );
            KeyerAction::getCurrentAction() ->LxChanged( eL2, state );
         }
      }
      else
         if ( state )
         {
            if ( L2State && L1State )
            {
               // use this as reset, as well as start rec sequence

               sbDriver::getSbDriver() ->stopall();                       // make sure nothing is happening
               KeyerAction::currentAction.freeAll();	// clear all the chains

               new BoxRecordAction();
            }
            else
               if ( boxRecPending )
                  KeyerAction::getCurrentAction() ->LxChanged( eL2, state );
         }
   }
   return true;
}

void voiceKeyer::tickEvent()
{
   if ( currentKeyer == this )
   {
      commonKeyer::tickEvent();
      sbTickEvent();
   }
}
bool voiceKeyer::initialise( const KeyerConfig &keyer, const PortConfig &port )
{
   if ( commonKeyer::initialise( keyer, port ) )
   {
      if ( sblog )
      {
         trace( "commonkeyer initialised" );
      }
      return sbInitialise( kconf.pipTone, kconf.pipVolume, kconf.pipLength );
   }
   return false;
}
void voiceKeyer::select( bool /*sel*/ )
{}
void voiceKeyer::initTone1( int t1 )
{
   sbInitTone1( t1 );
}
void voiceKeyer::initTone2( int t1, int t2 )
{
   sbInitTone2( t1, t2 );
}
void voiceKeyer::startTone1()
{
   sbStartTone1();
}
void voiceKeyer::startTone2()
{
   sbStartTone2();
}
bool voiceKeyer::sendCW( const char *message, int speed, int tone )
{
   disableInterrupts guard;
   KeyerAction * ca = KeyerAction::getCurrentAction();
   if ( !ca && !recPending && !boxRecPending && started && currentKeyer == this )
   {
      sbDriver::getSbDriver() ->stopall();    // make sure nothing is happening
      KeyerAction::currentAction.freeAll();	// clear all the chains

      sbDriver::getSbDriver() ->createCWBuffer( message, speed, tone );
      new PlayAction( "AudioCWFile", false, kconf.startDelay, kconf.enableAutoRepeat ? kconf.autoRepeatDelay : 0, true, true );
      return true;
   }
   return false;
}
bool voiceKeyer::startMicPassThrough()
{
   return sbDriver::getSbDriver() ->startMicPassThrough();
}
bool voiceKeyer::stopMicPassThrough()
{
   return sbDriver::getSbDriver() ->stopMicPassThrough();
}
//==============================================================================
//==============================================================================
sbKeyer::sbKeyer()
{}
sbKeyer::~sbKeyer()
{
   if ( sblog )
   {
      trace( "sbKeyer::~sbKeyer" );
   }
}
void sbKeyer::sbTickEvent()           // this will often be an interrupt routine
{
   // most of this WAS TXInt, but it should only get called as a tick
   // and it should all be in the keyer, not here - sb shouldn't get
   // time interrupts.
   if ( currentKeyer && currentKeyer->started )
   {
      disableInterrupts guard;
      KeyerAction * sba = KeyerAction::getCurrentAction();
      if ( sba )
      {
         if ( sba->deleteAtTick )
         {
            KeyerAction * sbn = sba->getNextAction();
            if ( sbn )
            {
               sbn->actionTime = 1;
            }
            // need to remove the action from the queue
            KeyerAction::currentAction.free_element( sba );
         }

         sba = KeyerAction::getCurrentAction();
         if ( sba )
         {
            sba->checkTimer();
         }
      }
      currentKeyer->checkControls();   // which we are a base class of...
   }
}
bool sbKeyer::sbInitialise( int pipTone, int pipVolume, int pipLength )
{
   std::string errmess;
   if ( !sbDriver::getSbDriver() ->sbdvp_init( errmess, pipTone, pipVolume, pipLength ) )
   {
      trace( "sbdvp_init failed! " + errmess );
      trace( errmess.c_str() );
      return false;
   }
   return true;
}
void sbKeyer::sbInitTone1( int t1 )
{
   sbDriver::getSbDriver() ->initTone1( t1 );
}
void sbKeyer::sbInitTone2( int t1, int t2 )
{
   sbDriver::getSbDriver() ->initTone2( t1, t2 );
}

void sbKeyer::sbStartTone1()
{
   sbDriver::getSbDriver() ->startTone1();
}
void sbKeyer::sbStartTone2()
{
   sbDriver::getSbDriver() ->startTone2();
}

//==============================================================================
// Common action stuff

KeyerAction::KeyerAction() : actionTime( -1 ), deleteAtTick( false ),
      pipStartDelaySamples( 0 ), tailWithPip( false )
{
   disableInterrupts guard;
   KeyerAction::currentAction.push_back( this );
   startTick = currTick;
   lastTick = currTick;
}
KeyerAction::~KeyerAction()
{
   deleteAtTick = false;
   actionTime = -1;
}

/*static*/ KeyerAction *KeyerAction::getCurrentAction()
{
   disableInterrupts guard;
   if ( currentAction.begin() == currentAction.end() )
      return 0;
   return *currentAction.begin();
}

KeyerAction *KeyerAction::getNextAction()
{
   disableInterrupts guard;
   return KeyerAction::currentAction.next_element( this );
}

void KeyerAction::checkTimer()
{
   long thisTick = currTick;
   if ( actionTime > 0 )
   {
      if ( thisTick <= lastTick )
      {
         lastTick = thisTick;
         return ;
      }

      //      long intime = actionTime;
      actionTime -= ( thisTick - lastTick );
      lastTick = thisTick;
      if ( actionTime <= 0 )
      {
         timeOut();
      }
   }
}
bool KeyerAction::playingFile( const std::string & )
{
   return false;
}
void KeyerAction::activateVox( void )
{}
void KeyerAction::queueFinished( void )
{}
void KeyerAction::stopTransmit()
{}
//=============================================================================
VoiceAction::VoiceAction()
{}
VoiceAction::~VoiceAction()
{}
void VoiceAction::interruptOK( void )
{
   // this should stop timeout if no interrupts,
   actionTime = 1000 / TIMER_INTERVAL;   // ticks
}
//=============================================================================
//=============================================================================
void ToneAction::getActionState( std::string &s )
{
   char buff[ 80 ];
   sprintf( buff, "Tuning %d tone", nTone );
   s = buff;
}
void ToneAction::LxChanged( int /*line*/, bool state )
{
   if ( sblog )
   {
      trace( "ToneAction::LxChanged(" + makeStr( state ) + ")" );
   }
   disableInterrupts guard;
   KeyerAction::currentAction.clear_after( KeyerAction::getCurrentAction() );

   sbDriver::getSbDriver() ->stopDMA();
   sbDriver::getSbDriver() ->CW_ACTIVE = false;
   if ( currentKeyer )
      currentKeyer->ptt( 0 );
   SetCurrentMixerSet( emsPassThroughNoPTT );
   deleteAtTick = true;
}
void ToneAction::pttChanged( bool state )
{
   if ( sblog )
   {
      trace( "ToneAction::pttChanged(" + makeStr( state ) + ")" );
   }
   // PTT pressed, so kill tone
   disableInterrupts guard;
   KeyerAction::currentAction.clear_after( KeyerAction::getCurrentAction() );

   sbDriver::getSbDriver() ->stopDMA();
   sbDriver::getSbDriver() ->CW_ACTIVE = false;
   if ( currentKeyer )
      currentKeyer->ptt( 0 );
   SetCurrentMixerSet( emsPassThroughNoPTT );
   deleteAtTick = true;
}
void ToneAction::queueFinished()
{
   actionTime = 1;
   //   timeOut();
}
void ToneAction::timeOut()
{
   if ( sblog )
   {
      trace( "ToneAction::timeOut(" + makeStr( actionState ) + ")" );
   }
   actionTime = -1;
   switch ( actionState )
   {
      case etasInitial:
         actionTime = 1;
         actionState = etasPTTDelay;
         break;

      case etasPTTDelay:
         currentKeyer->ptt( 1 );
         actionTime = delayTicks;
         actionState = etasStartTone;
         break;

      case etasStartTone:
         actionTime = 1000 / TIMER_INTERVAL;
         actionState = etasStopTone;
         if ( nTone == 1 )
         {
            SetCurrentMixerSet( emsReplayT1 );
            currentKeyer->startTone1();
         }
         else
         {
            SetCurrentMixerSet( emsReplayT2 );
            currentKeyer->startTone2();
         }
         break;

      case etasStopTone:
         sbDriver::getSbDriver() ->stopDMA();
         sbDriver::getSbDriver() ->CW_ACTIVE = false;
         if ( currentKeyer )
            currentKeyer->ptt( 0 );
         SetCurrentMixerSet( emsPassThroughNoPTT );
         deleteAtTick = true;
         ;
         break;

   }
}
void ToneAction::interruptOK( void )
{
   // this should stop timeout if no interrupts,

   actionTime = 1000 / TIMER_INTERVAL;
}
ToneAction::ToneAction( int n, long pdelayStart ) : actionState( etasInitial ), nTone( n )
{
   if ( sblog )
   {
      trace( "new ToneAction" );
   }
   delayTicks = ( pdelayStart / TIMER_INTERVAL );	// 55ms/tick, 18.4ticks/sec
   if ( delayTicks <= 0 )
      delayTicks = 1;	// so it starts on next tick
   actionTime = 1;
}
ToneAction::~ToneAction()
{
   if ( sblog )
   {
      trace( "ToneAction deleted" );
   }
}
//=============================================================================
InitialPTTAction::InitialPTTAction() : actionState( einitPTTInitial )
{
   if ( sblog )
   {
      trace( "new InitialPTTAction" );
   }
   actionTime = 1;
}
InitialPTTAction::~InitialPTTAction()
{
   if ( sblog )
   {
      trace( "InitialPTTAction deleted" );
   }
}
void InitialPTTAction::getActionState( std::string &s )
{
   s = "Initial PTT";
}
void InitialPTTAction::LxChanged( int /*line*/, bool /*state*/ )
{}
void InitialPTTAction::pttChanged( bool state )
{
   if ( sblog )
   {
      trace( "InitialPTTAction::pttChanged(" + makeStr( state ) + ")" );
   }
   if ( state )
   {
      // PTT pressed - again? - ignore it!
   }
   else
   {
      // PTT released, start pip - after debounce if needed
      disableInterrupts guard;
      KeyerAction * sba = KeyerAction::getCurrentAction();
      if ( sba )
         sba->timeOut();
   }
}
void InitialPTTAction::queueFinished()
{
   // shouldn't happen...
}
void InitialPTTAction::timeOut()
{
   // enum initPTTActionStates {einitPTTInitial = -1, einitPTTStart, einitPTTEnd, einitPTTFlickRelease, einitPTTRelease} actionState;
   if ( sblog )
   {
      trace( "InitialPTTAction::timeOut(" + makeStr( actionState ) + ")" );
   }
   actionTime = -1;
   switch ( actionState )
   {
      case einitPTTInitial:
         if ( currentKeyer )
         {
            currentKeyer->ptt( 1 );
            SetCurrentMixerSet( emsPassThroughPTT );
            currentKeyer->startMicPassThrough();
         }
         actionState = einitPTTStart;
         // This is where a real PTT initiated voice from microphone to rig commences
         // It won't time out.
         break;

      case einitPTTStart:
         {
            if ( currTick - lastTick <= 1 )
            {
               actionState = einitPTTFlickRelease;
               actionTime = 1;
               break;				// debounce, or "flick" PTT
            }
            actionState = einitPTTRelease;
            timeOut();
            break;
         }

      case einitPTTFlickRelease:
         {
            if ( !currentKeyer->pttState )
            {
               actionState = einitPTTRelease;	// PTT still off, go to pip
               timeOut();
               break;
            }
            actionState = einitPTTStart;   // PTT pressed again, so go back
         }
         break;

      case einitPTTRelease:
         if ( currentKeyer->kconf.enablePip )
         {
            if ( !getNextAction() )
               new PipAction();
            deleteAtTick = true;
         }
         actionState = einitPTTEnd;
         actionTime = 1;
         break;

      case einitPTTEnd:
         currentKeyer->ptt( 0 );	// if we got here then no "next" so kill ptt
         SetCurrentMixerSet( emsPassThroughNoPTT );
         deleteAtTick = true;
         break;

      default:
         break;
   }
}
//=============================================================================
InterruptingPTTAction::InterruptingPTTAction() : actionState( einterPTTInitial )
{
   if ( sblog )
   {
      trace( "new InterruptingPTTAction" );
   }
   actionState = einterPTTWaitDelay;
   actionTime = 1;
}
InterruptingPTTAction::~InterruptingPTTAction()
{
   if ( sblog )
   {
      trace( "InterruptingPTTAction deleted" );
   }
}
void InterruptingPTTAction::getActionState( std::string &s )
{
   s = "Interrupting PTT";
}
void InterruptingPTTAction::LxChanged( int /*line*/, bool /*state*/ )
{}
void InterruptingPTTAction::pttChanged( bool state )
{
   if ( sblog )
   {
      trace( "InterruptingPTTAction::pttChanged(" + makeStr( state ) + ")" );
   }
   if ( state )
   {
      // PTT pressed - again? - ignore it!
   }
   else
   {
      // PTT released, start pip
      disableInterrupts guard;
      KeyerAction * sba = KeyerAction::getCurrentAction();
      if ( sba )
         sba->timeOut();
   }
}
void InterruptingPTTAction::queueFinished()
{
   // Shouldn't happen...
}
void InterruptingPTTAction::timeOut()
{
   if ( sblog )
   {
      trace( "InterruptingPTTAction::timeOut(" + makeStr( actionState ) + ")" );
   }
   actionTime = -1;
   switch ( actionState )
   {
      case einterPTTWaitDelay:
         {
            actionState = einterPTTWaitDelayFinish;
            actionTime = ( currentKeyer->kconf.playPTTDelay / TIMER_INTERVAL );	// 55ms/tick, 18.4ticks/sec
            if ( actionTime < 1 )
               actionTime = 1;
            break;
         }

      case einterPTTWaitDelayFinish:
         {
            // PTT releasing play, don't start for a bit!
            if ( currentKeyer->pttState )
            {
               currentKeyer->ptt( 1 );
               SetCurrentMixerSet( emsPassThroughPTT );
               // This is where we tail end a file play with a normal transmission
               currentKeyer->startMicPassThrough();

               actionState = einterPTTDoPip;   	// do a pip
            }
            else
            {
               deleteAtTick = true; // short blip - don't pip
               currentKeyer->ptt( 0 );	// if we got here then no "next" so kill ptt
               actionState = einterPTTQuickRelease;
            }
            break;
         }

      case einterPTTDoPip:
         if ( currentKeyer->kconf.enablePip )
         {
            if ( !getNextAction() )
               new PipAction();
            deleteAtTick = true;
         }
         actionState = einterPTTQuickRelease;
         actionTime = 1;
         break;

      case einterPTTQuickRelease:
         currentKeyer->ptt( 0 );	// if we got here then no "next" so kill ptt
         SetCurrentMixerSet( emsPassThroughNoPTT );
         deleteAtTick = true;
         break;

      default:
         break;
   }
}
//=============================================================================
PlayAction::PlayAction( const std::string &pfileName, bool noPTT, long pdelayStart,
                        long prepeatDelay, bool firstTime, bool CW )
      : testMode( noPTT ), actionState( epasInitial ), CW( CW )
{
   if ( sblog )
   {
      trace( "new PlayAction" );
   }
   fileName = pfileName;

   delayTicks = ( pdelayStart / TIMER_INTERVAL );	// 55ms/tick, 18.4ticks/sec
   if ( delayTicks <= 0 )
      delayTicks = 1;	// so it starts on next tick

   repeatTicks = ( long ) ( prepeatDelay * 1000 / TIMER_INTERVAL );	// 18.4 ticks/sec
   actionTime = 1;

   if ( testMode )
   {
      actionState = epasPlayFile;	// no PTT, no linear, no repeat
      repeatTicks = -1;
      delayTicks = -1;
   }
   else
   {
      if ( firstTime )
         actionState = epasInitial;
      else
         actionState = epasWaitRepeat;
   }
}
PlayAction::~PlayAction()
{
   if ( sblog )
   {
      trace( "PlayAction deleted" );
   }
}
void PlayAction::getActionState( std::string &s )
{
   s = "Play " + fileName + " : " + ActionStateString;
}
void PlayAction::LxChanged( int line, bool state )
{
   if ( sblog )
   {
      trace( "PlayAction::LxChanged(" + makeStr( line ) + ", " + makeStr( state ) + ")" );
   }
   disableInterrupts guard;
   if ( KeyerAction::currentAction.next_element( this ) && ( actionState != epasEndPlayFile ) )    	// something waiting, but not actually playing
   {
      // start the next action instantly
      deleteAtTick = true;
   }
}
void PlayAction::pttChanged( bool state )
{
   if ( sblog )
   {
      trace( "PlayAction::pttChanged(" + makeStr( state ) + ")" );
   }
   if ( state )
   {
      // PTT pressed, so kill playback and chain PTT action
      disableInterrupts guard;
      KeyerAction::currentAction.clear_after( KeyerAction::getCurrentAction() );

      sbDriver::getSbDriver() ->stopDMA();
      sbDriver::getSbDriver() ->CW_ACTIVE = false;
      //      currentKeyer->ptt( 0 );
      // start a PTT action, but don't pip unless it is "long"
      InterruptingPTTAction *ptta = new InterruptingPTTAction();
      ptta->actionTime = 1;
      deleteAtTick = true;
   }
   else
   {
      // how did we release PTT? Anyway, ignore it
   }
}
void PlayAction::queueFinished()
{
   actionTime = 1;
   timeOut();   // pick it up as quickly as possible
}
void PlayAction::timeOut()
{
   if ( sblog )
   {
      trace( "PlayAction::timeOut(" + makeStr( actionState ) + ")" );
   }
   actionTime = -1;
   switch ( actionState )
   {
      case epasInitial:
         // state -1 startup; next state is 1 (don't wait for repeat delay)
         actionTime = 1;
         actionState = epasWaitLinear;
         break;

      case epasWaitRepeat:
         // state 0 wait for repeat delay
         actionTime = repeatTicks;
         ActionStateString = "Auto Repeat gap";
         actionState = epasWaitLinear;
         break;

      case epasWaitLinear:
         // state 1 PTT, wait for linear delay
         if ( !testMode )
         {
            currentKeyer->ptt( 1 );
            actionTime = delayTicks;
            ActionStateString = "Starting";
            actionState = epasPlayFile;
         }
         break;

      case epasPlayFile:
         {
            // state 2 play file
            SetCurrentMixerSet( emsReplay );
            actionState = epasEndPlayFile;
            ActionStateString = "Play";

            pipStartDelaySamples = ( currentKeyer->kconf.pipStartDelay * sbDriver::getSbDriver() ->rate ) / 1000;
            tailWithPip = currentKeyer->kconf.enablePip;

            if ( sbDriver::getSbDriver() ->play_file( fileName, !testMode ) < 0 )
            {
               actionTime = 1;
               deleteAtTick = true;
            }
            else
            {
               actionTime = 1000 / TIMER_INTERVAL;	// 1 sec to first interrupt ought to be enough
            }
            break;
         }

      case epasEndPlayFile:
         // state 3 chain PIP and chain next repeat; start in state 1
         ActionStateString = "Ending";
         KeyerAction *sbn = getNextAction();
         /*
         if ( currentKeyer->kconf.enablePip )
         {
            if ( !sbn && !CW )
               new PipAction();
            // leave mixer alone until next action sets it
         }
         else
         */
         {
            sbDriver::getSbDriver() ->stopDMA();
            sbDriver::getSbDriver() ->CW_ACTIVE = false;
            if ( currentKeyer )
               currentKeyer->ptt( 0 );
            SetCurrentMixerSet( emsPassThroughNoPTT );
         }
         if ( !testMode && currentKeyer->kconf.enableAutoRepeat && currentKeyer->kconf.autoRepeatDelay && !sbn && !CW )
         {
            new PlayAction( fileName, false, currentKeyer->kconf.startDelay, currentKeyer->kconf.autoRepeatDelay, false, false );
         }

         deleteAtTick = true;
         break;
   }
}
bool PlayAction::playingFile( const std::string &f )
{
   bool samefile = ( stricmp( f.c_str(), fileName.c_str() ) == 0 );

   if ( samefile && ( actionState == epasPlayFile || actionState == epasEndPlayFile ) )
      return true;	// same file is actually playing (or waiting for the linear)

   return false;
}
//=============================================================================
PipAction::PipAction() : actionState( epipasInitial )
{
   if ( sblog )
   {
      trace( "new PipAction" );
   }
}
PipAction::~PipAction()
{
   if ( sblog )
   {
      trace( "PipAction deleted" );
   }
}
void PipAction::LxChanged( int /*line*/, bool /*state*/ )
{}
void PipAction::getActionState( std::string &s )
{
   s = "Pip";
}
void PipAction::pttChanged( bool state )
{
   if ( sblog )
   {
      trace( "pttChanged::pttChanged(" + makeStr( state ) + ")" );
   }
   if ( state )
   {
      // PTT pressed, so kill playback and chain PTT action
      disableInterrupts guard;
      KeyerAction::currentAction.clear_after( KeyerAction::getCurrentAction() );
      new InterruptingPTTAction();
      deleteAtTick = true;
   }
   else
   {
      // how did we release PTT? Anyway, ignore it
   }
}
void PipAction::queueFinished()
{
   actionTime = 1;
   //   timeOut();
}
void PipAction::timeOut()
{
   if ( sblog )
   {
      trace( "PipAction::timeOut(" + makeStr( actionState ) + ")" );
   }
   actionTime = -1;
   switch ( actionState )
   {
      case epipasInitial:
         // start up the pip tone
         actionTime = ( currentKeyer->kconf.pipStartDelay / TIMER_INTERVAL );	// 55ms/tick, 18.4ticks/sec
         if ( actionTime < 1 )
            actionTime = 1;
         actionState = epipasPip;
         break;

      case epipasPip:
         SetCurrentMixerSet( emsReplayPip );
         sbDriver::getSbDriver() ->play = true;
         sbDriver::getSbDriver() ->recording = false;
         sbDriver::getSbDriver() ->dofile( DOFILE_PIP );
         actionState = epipasEndPip;
         actionTime = 1000 / TIMER_INTERVAL;	// safety net! 1 sec to first interrupt
         break;

      case epipasEndPip:
         deleteAtTick = true;
         sbDriver::getSbDriver() ->stopDMA();
         sbDriver::getSbDriver() ->CW_ACTIVE = false;
         if ( currentKeyer )
            currentKeyer->ptt( 0 );
         SetCurrentMixerSet( emsPassThroughNoPTT );
         break;

   }
}
//=============================================================================

RecordAction::RecordAction( const std::string &pfileName )
      : actionState( erasInitial )
{
   if ( sblog )
   {
      trace( "new RecordAction" );
   }
   currentKeyer->recPending = true;
   actionTime = 1;
   fileName = pfileName;
}
RecordAction::~RecordAction()
{
   if ( sblog )
   {
      trace( "RecordAction deleted" );
   }
   currentKeyer->recPending = false;
}
void RecordAction::getActionState( std::string &s )
{
   s = "Record " + fileName + ( ( actionState == erasRecWaitPTT ) ? " Waiting PTT" : "" );
}
void RecordAction::LxChanged( int /*line*/, bool /*state*/ )
{}
void RecordAction::pttChanged( bool state )
{
   if ( sblog )
   {
      trace( "RecordAction::pttChanged(" + makeStr( state ) + ")" );
   }
   if ( state )
   {
      // PTT pressed, so start recording

      if ( actionState == erasRecWaitPTT )
      {
         // wait for PTT/timeout state, so go as PTT
         actionState = erasStartRec;
         timeOut();
      }
      else
      {
         sbDriver::getSbDriver() ->stopall();
         deleteAtTick = true;
      }

      // NB blip the PTT will stop recording
   }
   else
   {
      if ( actionState == erasStopRec )
      {
         // PTT released, finish recording
         timeOut();
      }
      else
      {
         sbDriver::getSbDriver() ->stopall();
         deleteAtTick = true;
      }
   }
}
void RecordAction::queueFinished()
{
   actionTime = 1;
   //   timeOut();
}
void RecordAction::timeOut()
{
   if ( sblog )
   {
      trace( "RecordAction::timeOut(" + makeStr( actionState ) + ")" );
   }
   actionTime = -1;
   switch ( actionState )
   {
      case erasInitial:
         // wait for record start on PTT
         actionState = erasRecWaitPTT;
         actionTime = 5 * 1000 / TIMER_INTERVAL;	// about 5 seconds
         break;

      case erasRecWaitPTT:
         deleteAtTick = true;
         break;

      case erasStartRec:
         // start recording
         SetCurrentMixerSet( emsVoiceRecord );
         actionState = erasStopRec;
         sbDriver::getSbDriver() ->record_file( fileName );
         actionTime = 1000 / TIMER_INTERVAL;			// safety net to first interrupt
         break;

      case erasStopRec:
         // stop recording
         actionState = erasRecFinished;
         actionTime = 1000 / TIMER_INTERVAL;			// safety net to finish buffer return
         sbDriver::getSbDriver() ->stoprec();
         currentKeyer->recPending = false;
         break;

      case erasRecFinished:                            // queue finished should bring us here
         SetCurrentMixerSet( emsPassThroughNoPTT );
         deleteAtTick = true;
         break;
   }
}
//=============================================================================

BoxRecordAction::BoxRecordAction() : actionState( ebrasInitial )
{
   if ( sblog )
   {
      trace( "new BoxRecordAction" );
   }
   actionTime = 5 * 1000 / TIMER_INTERVAL;	// about 5 seconds
   currentKeyer->boxRecPending = true;
}
BoxRecordAction::~BoxRecordAction()
{
   if ( sblog )
   {
      trace( "BoxRecordAction deleted" );
   }
   currentKeyer->boxRecPending = false;
}
void BoxRecordAction::getActionState( std::string &s )
{
   s = "Record; wait for line";
}
void BoxRecordAction::LxChanged( int line, bool state )
{
   if ( sblog )
   {
      trace( "BoxRecordAction::LxChanged(" + makeStr( line ) + ", " + makeStr( state ) + ")" );
   }
   if ( line == eL1 && state == true )
   {
      new RecordAction( "CQF1.WAV" );
      currentKeyer->boxRecPending = false;
      deleteAtTick = true;
   }
   else
      if ( line == eL2 && state == true )
      {
         new RecordAction( "CQF2.WAV" );
         currentKeyer->boxRecPending = false;
         deleteAtTick = true;
      }
}
void BoxRecordAction::pttChanged( bool state )
{
   if ( sblog )
   {
      trace( "BoxRecordAction::pttChanged(" + makeStr( state ) + ")" );
   }
   deleteAtTick = true;
}
void BoxRecordAction::queueFinished()
{}
void BoxRecordAction::timeOut()
{
   if ( sblog )
   {
      trace( "BoxRecordAction::timeOut(" + makeStr( actionState ) + ")" );
   }
   // wait for L1 or L2 to be PRESSED, start a Record Action
   // Then wait on PTT to trigger a Record
   // NB that state doesn't matter
   currentKeyer->boxRecPending = false;
   deleteAtTick = true;
}
//==============================================================================
//==============================================================================

