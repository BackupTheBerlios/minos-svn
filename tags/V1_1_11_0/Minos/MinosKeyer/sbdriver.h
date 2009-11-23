/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef sbdriverH
#define sbdriverH

enum sbControls {ePTT, eL1, eL2};

#define DOFILE_PIP -1
#define DOFILE_T1 -2
#define DOFILE_T2 -3
#define DOFILE_CW -4

//void DefineSoundCard(const ParamTable &scard);

class SoundSystem;
class sbDriver //:public timerTicker
{
   private:
      // another singleton to handle the sb card
      // it may need callbacks to interested parties!
      static sbDriver *singleton_sb;
      SoundSystem *soundSystem;
      //===============================================================
      // working values, read from control on entry, written back on exit

      DWORD CurrMasterLevel;
      DWORD CurrRecLevel;
      DWORD CurrMicLevel;

      //      VKMixer *mixer;
      //      void closeMixer();
      //===============================================================

      int oldpip;
      int oldpipVolume;
      int oldpipLength;
      double lastCWRate;

      int ihand, isave;

      //      static void interrupt(* old_intr_sb)(...);
      void unload( void );
   public:
      volatile int recording;
      bool ready;
      bool loadFailed;

      bool CW_ACTIVE;
      bool init_done;
      bool init_OK;

      int cwTone;

      int pipSamples;
      INT16 * pipptr;

      int toneSamples;
      INT16 * t1ptr;
      INT16 * t2ptr;

      int cwSamples;
      INT16 * cwptr;

      INT16 *ptr;       /* data for current file */
      int samples;   /* fsample for current file  */
      char play;  /* Play or record */

      unsigned int rate;   /* rate in Hertz -- this gets reset to nearest available value */

      VUCallBack WinVUInCallback;
      VUCallBack WinVUOutCallback;

      bool dofile( int i, int clipRecord = 0 );
      void stoprec();
      void record_file( const std::string &filename );
      long play_file( const std::string &filename, bool xmit );
      void stopall();
      void stopDMA();
      bool rdenv( std::string &errmess, std::string &in );

      void genTone( INT16 *tptr, bool add
                       , int tone, int samples, int ramptime, double vmult );
      bool createPipTone( std::string &errmess, int pipTone, int pipVolume, int pipLength );
      static void sbdvp_unload( void );
      void initTone1( int );
      void initTone2( int, int );
      void startTone1();
      void startTone2();
      void createCWBuffer( const char *message, int speed, int tone );
   public:
      bool sbdvp_init( std::string &errmess, int pipTone, int pipVolume, int pipLength );
      sbDriver();
      ~sbDriver();

      static sbDriver *getSbDriver();
};

#endif
