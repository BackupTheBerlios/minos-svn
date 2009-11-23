/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef keyersH
#define keyersH
#include <dir.h>        // just for MAXPATH
#include "keyconf.h"
#include "keyctrl.h"
#include "keyerlog.h"

class sbDriver;

class commonPort;
class commonKeyer;

class lineMonitor
{
   private:
      commonPort *cp;
   public:
      PortConfig pconf;
      KeyerConfig kconf;
      std::string pName;
      lineMonitor( const std::string pname );
      lineMonitor( const KeyerConfig &keyer, const PortConfig &port );
      virtual ~lineMonitor();

      virtual void ptt( int state );
      virtual void key( int state );

      virtual bool pttChanged( int state ) = 0;
      virtual bool L1Changed( int state ) = 0;
      virtual bool L2Changed( int state ) = 0;

      virtual bool initialise( const KeyerConfig &keyer, const PortConfig &port ) = 0;

      virtual void checkControls( void );
};

class timerTicker;
// timerTicker is used for the state machine sequencing
class baseTimer
{
      static HANDLE hTimerThread;
      static HANDLE hTimerThreadCloseEvent;
      static unsigned __stdcall timerThread( LPVOID lpThreadParameter );
      void timerThread();
      // baseTimer is a singleton to handle the ticker interrupt
      my_deque < timerTicker *> tickers;

      static void CALLBACK intrTick( UINT wTimerID, UINT msg,
                                     DWORD dwUser, DWORD dw1, DWORD dw2 );
      static baseTimer *tt;

      static int ttcount;
   public:
      static baseTimer *initialiseTimer();
      static void killTimer();
      static void closeTimer();

      baseTimer();
      ~baseTimer();

      void registerTicker( timerTicker * );
      void tickEvent();       // this will often be an interrupt routine
};
class timerTicker
{
      baseTimer *b;
   public:
      bool ready;
      timerTicker();
      virtual ~timerTicker();
      virtual void tickEvent() = 0;       // this will often be an interrupt routine
};

// all keyers want timer ticks to sequence the state machine
class commonKeyer: public lineMonitor, public timerTicker
{
   public:
      commonKeyer( const KeyerConfig &keyer, const PortConfig &port );
      ~commonKeyer();

      virtual bool pttChanged( int state );
      virtual bool L1Changed( int state );
      virtual bool L2Changed( int state );

      virtual void tickEvent();       // this will often be an interrupt routine
      virtual bool getInfo( KeyerInfo * ) = 0;

      virtual bool initialise( const KeyerConfig &keyer, const PortConfig &port ) = 0;
      virtual void select( bool ) = 0;

      virtual bool docommand( const KeyerCtrl &dvp_ctrl ) = 0;

      virtual bool getPTT( void );
      virtual void getActionState( std::string & );
      virtual bool getStatus( std::string &a );
      virtual bool sendCW( const char *message, int speed, int tone ) = 0;

      virtual void initTone1( int ) = 0;
      virtual void initTone2( int, int ) = 0;
      virtual void startTone1() = 0;
      virtual void startTone2() = 0;
      virtual void queueFinished();

      bool started;
      int startcount;

      bool L1State;
      bool L2State;
      bool pttState;
      virtual void enableQueue( bool /*b*/ )
      {}
      bool boxRecPending;
      bool recPending;

      /*      // all now in kconf
      int autoRepeat;
      int startDelay;
      int voxHang;

      int pipstartDelay;
      int playPTTDelay;
      int pipTone;
      int pipVolume;
      bool pipEnabled;
      */
      double cwRate;
      int tickcount;
      unsigned long lastIntCount;

      int tone1;
      int tone2;

      bool inTone;
};

class sbKeyer
{
   public:
      sbKeyer();
      virtual ~sbKeyer();
      void sbTickEvent();       // this will often be an interrupt routine
      bool sbInitialise( int pipTone, int pipVolume, int pipLength );
      void sbInitTone1( int );
      void sbInitTone2( int, int );
      void sbStartTone1();
      void sbStartTone2();
};

class voiceKeyer: public commonKeyer, public sbKeyer
{
   public:

      voiceKeyer( const KeyerConfig &keyer, const PortConfig &port );
      ~voiceKeyer();
      bool docommand( const KeyerCtrl &dvp_ctrl );

      virtual bool getInfo( KeyerInfo * );
      virtual bool pttChanged( int state );
      virtual bool L1Changed( int state );
      virtual bool L2Changed( int state );
      virtual bool initialise( const KeyerConfig &keyer, const PortConfig &port );
      virtual void select( bool );
      virtual bool sendCW( const char *message, int speed, int tone );
      void tickEvent();       // this will often be an interrupt routine
      virtual void initTone1( int );
      virtual void initTone2( int, int );
      virtual void startTone1();
      virtual void startTone2();
};

//=============================================================================

extern int tuneTime;
extern double tuneLevel;
extern int CWTone;
extern int CWSpeed;

extern commonKeyer *currentKeyer;

extern std::map <char, std::string> MORSECODE;    // . is 0x40, - is 0x80
class MORSEMSG
{
   public:
      MORSEMSG();
      ~MORSEMSG();
      void setMessage( const std::string &msg );
      std::string msg;
      bool repeat;
};
extern std::map <int, MORSEMSG> MORSEMSGS;


//=============================================================================
class KeyerAction
{
   protected:
      long startTick;
      long lastTick;
   public:
      int pipStartDelaySamples;
      bool tailWithPip;

      long actionTime;
      bool deleteAtTick;
      void checkTimer();
      virtual void getActionState( std::string &s ) = 0;

      virtual void LxChanged( int line, bool state ) = 0;
      virtual void pttChanged( bool state ) = 0;
      virtual void queueFinished() = 0;
      virtual void timeOut() = 0;
      virtual void stopTransmit();

      KeyerAction();
      virtual ~KeyerAction();
      static my_deque < KeyerAction *> currentAction;
      static KeyerAction *getCurrentAction();
      KeyerAction *getNextAction();
      virtual char statusLetter() = 0;
      virtual bool playingFile( const std::string & );
      virtual void activateVox( void );
      virtual void interruptOK( void ) = 0;

      long getActionTime()
      {
         return actionTime;
      }
};
//=============================================================================
class ToneAction: public KeyerAction
{
      enum ToneActionStates {etasInitial = -1, etasPTTDelay, etasStartTone, etasStopTone} actionState;
      int nTone;
   public:
      long delayTicks;

      virtual void getActionState( std::string &s );
      virtual void LxChanged( int line, bool state );
      virtual void pttChanged( bool state );
      virtual void queueFinished();
      virtual void timeOut();
      virtual void interruptOK( void );
      char statusLetter()
      {
         return 't';
      }
      ToneAction( int tones, long pdelayStart );
      virtual ~ToneAction();
};
//=============================================================================
class VoiceAction: public KeyerAction
{
   public:
      virtual void getActionState( std::string &s ) = 0;
      virtual void LxChanged( int line, bool state ) = 0;
      virtual void pttChanged( bool state ) = 0;
      virtual void queueFinished() = 0;
      virtual void timeOut() = 0;
      virtual void interruptOK( void );
      VoiceAction();
      virtual ~VoiceAction();
};
//=============================================================================
class InitialPTTAction: public VoiceAction
{
      enum initPTTActionStates {einitPTTInitial = -1, einitPTTStart, einitPTTEnd, einitPTTFlickRelease, einitPTTRelease} actionState;
   public:
      virtual void getActionState( std::string &s );
      virtual void LxChanged( int line, bool state );
      virtual void pttChanged( bool state );
      virtual void queueFinished();
      virtual void timeOut();
      InitialPTTAction();
      virtual ~InitialPTTAction();
      char statusLetter()
      {
         return 'T';
      }
};
//=============================================================================
class InterruptingPTTAction: public VoiceAction
{
      enum InterruptPTTActionStates {einterPTTInitial = -1, einterPTTWaitDelay,
                                     einterPTTWaitDelayFinish, einterPTTDoPip, einterPTTQuickRelease} actionState;
   public:
      virtual void getActionState( std::string &s );
      virtual void LxChanged( int line, bool state );
      virtual void pttChanged( bool state );
      virtual void queueFinished();
      virtual void timeOut();
      InterruptingPTTAction();
      virtual ~InterruptingPTTAction();
      char statusLetter()
      {
         return 'T';
      }
};
//=============================================================================
class PlayAction: public VoiceAction
{
      std::string fileName;
      long delayTicks;
      long repeatTicks;
      bool testMode;
      bool CW;
      enum PlayActionStates {epasInitial = -1, epasWaitRepeat, epasWaitLinear, epasPlayFile,
                             epasEndPlayFile} actionState;
      std::string ActionStateString;
   public:
      virtual void getActionState( std::string &s );
      virtual void LxChanged( int line, bool state );
      virtual void pttChanged( bool state );
      virtual void queueFinished();
      virtual void timeOut();
      PlayAction( const std::string &fileName, bool noPTT, long delayStart, long repeatDelay, bool firstTime, bool CW );
      virtual ~PlayAction();
      char statusLetter()
      {
         return 'S';
      }
      virtual bool playingFile( const std::string & );
};
//=============================================================================
class PipAction: public VoiceAction
{
      enum PipActionStates {epipasInitial = -1, epipasPip, epipasEndPip} actionState;
   public:
      virtual void getActionState( std::string &s );
      virtual void LxChanged( int line, bool state );
      virtual void pttChanged( bool state );
      virtual void queueFinished();
      virtual void timeOut();
      PipAction();
      virtual ~PipAction();
      char statusLetter()
      {
         return 'P';
      }
};
//=============================================================================
class RecordAction: public VoiceAction
{
      std::string fileName;
      enum RecordActionStates {erasInitial = -1, erasStartRec, erasStopRec, erasRecFinished, erasRecWaitPTT} actionState;
   public:
      virtual void getActionState( std::string &s );
      virtual void LxChanged( int line, bool state );
      virtual void pttChanged( bool state );
      virtual void queueFinished();
      virtual void timeOut();
      RecordAction( const std::string &fileName );
      virtual ~RecordAction();
      char statusLetter()
      {
         return 'R';
      }
};
//=============================================================================
class BoxRecordAction: public VoiceAction
{
      enum BoxRecordActionStates {ebrasInitial = -1} actionState;
   public:
      virtual void getActionState( std::string &s );
      virtual void LxChanged( int line, bool state );
      virtual void pttChanged( bool state );
      virtual void queueFinished();
      virtual void timeOut();
      BoxRecordAction();
      virtual ~BoxRecordAction();
      char statusLetter()
      {
         return 'B';
      }
};
//=============================================================================


/*
bool keyer_docommand( const KeyerCtrl & );
bool keyer_init( std::string & errmess );
bool keyer_select( const std::string & );
bool keyer_config( std::string & errmess );
void keyer_unload( void );
bool keyer_getPTT( void );
void keyer_getActionState( std::string & );
bool keyer_getStatus( std::string & );
int keyer_getlist( std::vector < std::string > & );
bool keyer_getinfo( KeyerInfo * );
unsigned long keyer_getintcount( void );
void keyer_setlines( bool PTT, bool L1, bool L2 );
void keyer_setlinecallback( LineCallBack lcb );
void keyer_setVUcallback( VUCallBack in, VUCallBack out );
*/
#endif
