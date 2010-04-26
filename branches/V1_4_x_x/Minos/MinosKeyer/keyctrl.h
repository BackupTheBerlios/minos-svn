/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef keyctrlH
#define keyctrlH

#include <dir.h>

typedef void FAR PASCAL ( *LineCallBack ) ( bool PTT, bool keyline, bool pPTTRef, bool pL1Ref, bool pL2Ref );
typedef void FAR PASCAL ( *VUCallBack ) ( unsigned int vol );

enum KEYER_COMMAND {eKEYER_STOPRECORD,    		// kill record
                    eKEYER_RECORD,
                    eKEYER_STOPALL,    			// kill record and playback
                    eKEYER_PLAY,    				// play text message
                    eKEYER_SENDCHAR,
                    eKEYER_TONE1,
                    eKEYER_TONE2,
                    eKEYER_VOXHANG
                   };

class KeyerCtrl
{
   public:
      KeyerCtrl( KEYER_COMMAND cmd, int mno1, int mno2, int mtime, const std::string &fname, bool xm = false, bool ch = false, bool dely = false ) :
            command( cmd ), xmit( xm ), chain( ch ), delay_start( dely ), intParam1( mno1 ), intTime( mtime ), intParam2( mno2 ), filename( fname )
      {}

      KEYER_COMMAND command;				// what to do
      bool xmit;							// modifier to PLAY/BUILD - activate PTT
      bool chain;							// add to play chain, or start up if no chain
      bool delay_start;					// don't start play until a chain with delay_start off
      int intParam1;
      int intParam2;
      int intTime;
      std::string filename;			// TX file name
};
class KeyerInfo    // only passes info up
{
   public:
      std::string kname;
      double kspeed;
      bool CW;
      bool voice;

      int voxHang;
      bool tune;
      bool twoTone;
      int tone1;
      int tone2;
      int tuneTime;
      int tuneLevel;
      int CWTone;
      int CWSpeed;
};
void loadKeyers( void );
void unloadKeyers( void );

bool getKeyerStatus( std::string & );
bool getKeyerInfo( KeyerInfo * );
void setLineCallBack( LineCallBack lcallback );
void setLines( bool PTT, bool L1, bool L2 );
bool getPTT();

void getActionState( std::string & );    // returns current keyer action state string

void setVUCallBack( VUCallBack in, VUCallBack out );

void startRecordDVPFile( int fileno );
void finishRecordDVPFile( void );
void playKeyerFile( int fileno, bool ptt );

std::vector < std::string > get_keyer_list();
bool select_keyer( const std::string &kn );

void stopKeyer();
void setKeyerType( const std::string &mode );

int getAutoRepeatDelay();
void setAutoRepeatDelay( int );
bool getEnableAutoRepeat();
void setEnableAutoRepeat( bool );
bool getPipEnabled();
void setPipEnabled( bool );
int getCWSpeed();
void setCWSpeed( int );
int getCWTone();
void setCWTone( int );

bool sendCW( const char *message, int speed, int tone );

void sendTone1( void );
void sendTone2( void );
#endif
