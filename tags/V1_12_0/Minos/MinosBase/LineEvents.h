/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef LineEventsH
#define LineEventsH 
//---------------------------------------------------------------------------
#if !defined(BUILD_DLL)
#define DLL_EXPORT __declspec(dllimport)
#else
#define DLL_EXPORT __declspec(dllexport)
#endif
class LineFileMapper;
class DLL_EXPORT LineSet
{
   private:
      static LineSet *myLineSet;

      std::map < std::string, bool > LineMap;
      HANDLE LineProtectionMutex;
      LineFileMapper *FileMap;
      bool closing;
   protected:
      bool readSet();
      void writeSet();

   public:
      bool lineChanged;
      HANDLE LineChangedEvent;

      static void ( *lsLog ) ( const std::string & );
      static LineSet *GetLineSet();

      bool waitForChange( unsigned int ms = 10000 );

      void publish( const std::string &, bool );

      void readLines();
      bool getState( const std::string name );

      LineSet();
      virtual ~LineSet();

      bool initialise();

};

#endif
