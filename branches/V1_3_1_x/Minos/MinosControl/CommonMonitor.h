/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef CommonMonitorH
#define CommonMonitorH 
//---------------------------------------------------------------------------
class commonController;
class commonPort;
class commonLineControl;
class LineConfig;
class PortConfig;

#define TARGET_RESOLUTION 55U         // 55-millisecond target resolution
#define TIMER_INTERVAL 55U         // 55-millisecond target interval

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
      ~timerTicker();
      virtual void tickEvent() = 0;       // this will often be an interrupt routine
};

class commonController: public timerTicker
{
   public:
      commonController( );
      ~commonController();
      my_deque < commonPort *> portChain;
      commonPort *createPort( const PortConfig &port );
      commonLineControl *findLine( const std::string &name, bool lineIn );

      bool initialise( );
      void closeDown();

      virtual void tickEvent();       // this will often be an interrupt routine
      void checkControls( void );

      //void ptt(int);
      //void key(int);
      void lineChange( commonLineControl *line );
};
#endif
