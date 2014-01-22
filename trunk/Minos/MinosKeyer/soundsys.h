/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef soundsysH
#define soundsysH

#ifndef MAX_BLOCK_LENGTH
#define MAX_BLOCK_LENGTH 1024
#endif
#ifndef CW_BLOCK_LENGTH
#define CW_BLOCK_LENGTH 256
#endif 
/*
* some good values for block size and count
*/
#define BLOCK_COUNT 20

class WriterThread;
class SoundSystem
{
   protected:
      int cfgrate;

   public:
      volatile int done;
      volatile long samplesremaining;
      volatile long now;
      volatile int sbactive;

      INT16 *dataptr;
      long samples;

      int mset;

      static SoundSystem *createSoundSystem();
      SoundSystem();
      virtual ~SoundSystem();
      virtual bool initialise( std::string &errmess ) = 0;
      virtual void terminate() = 0;
      virtual int setRate() = 0;
      virtual bool startMicPassThrough() = 0;
      virtual bool stopMicPassThrough() = 0;

      virtual bool startDMA( bool play, const std::string &fname ) = 0;
      virtual void stopDMA() = 0;

};


class WindowsSoundSystem: public SoundSystem
{
   private:
      // internal values
      WriterThread *wt;
      int sampleRate;
      //====================================================
      HWAVEOUT hWaveOut; /* device handle */
      bool waveOutActive;
      WAVEHDR* waveOutBlocks;
      int waveOutCurrentBlock;

      HANDLE hOutputThread;
      HANDLE hOutputThreadCloseEvent;

      volatile long samplesOutput;

      int writePassthrough( WAVEHDR * inhdr );
      int writeAudio( int deadSamples = 0 );
      static unsigned __stdcall OutputThread( LPVOID lpThreadParameter );
      void OutputThread();

      //====================================================
      HWAVEIN hWaveIn; /* device handle */
      bool waveInActive;
      WAVEHDR* waveInBlocks;
      int waveInCurrentBlock;

      HANDLE hInputThread;
      HANDLE hInputThreadCloseEvent;

      volatile long samplesInput;

      void addNextInBuffer();
      void readAudio();
      static unsigned __stdcall InputThread( LPVOID lpThreadParameter );
      void InputThread();

      //====================================================
      bool passthroughin;
      bool passthroughout;

      void allocateBlocks( WAVEHDR **blocks, int size, int count );
      void freeBlocks();

   public:
      volatile int waveInEventCount;
      volatile int waveInFreeBlockCount;
      HANDLE hWaveInEvent;    // used to signal from wave oiut/in
      //      int AudioRecordCallback(unsigned char *input, unsigned long len);

      volatile int waveOutFreeBlockCount;
      HANDLE hWaveOutEvent;
      //      int AudioPlayCallback(unsigned char *output, unsigned long len);

      WindowsSoundSystem();
      ~WindowsSoundSystem();

      virtual bool initialise( std::string &errmess );

      bool startInput( std::string fname );
      bool startOutput();
      virtual void terminate();
      virtual int setRate();

      virtual bool startMicPassThrough();
      virtual bool stopMicPassThrough();

      virtual bool startDMA( bool play, const std::string &fname );
      virtual void stopDMA();
};
#endif
