/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef LoggerContestH
#define LoggerContestH 
//----------------------------------------------------------------------------
class DisplayContestContact;
struct StanzaPos
{
   unsigned int stanza;
   long stanzaStart;
};
class LoggerContestLog : public BaseContestLog
{
      friend class MinosTestImport;
      // a LoggerContestLog is a sorted collection of ContestContact records, sorted by
      // the logSequence field
   private:

      // These are Logger specific, as they relate to storage
      bool minosFile;
      bool GJVFile;
      int logCount;  // Only used loading GJV files
      bool logFile;
      bool adifFile;
      bool ediFile;
      bool needExport;
      HANDLE minosContestFile;
      HANDLE GJVcontestFile;
      HANDLE logContestFile;
      HANDLE adifContestFile;
      HANDLE ediContestFile;
      bool importLOG( HANDLE );
      bool importAdif( HANDLE );
      bool importReg1Test( HANDLE adifContestFile );
      std::vector<StanzaPos> stanzaLocations;

   protected:
   public:
      bool isMinosFile()
      {
         return minosFile;
      }
      bool isGJVFile()
      {
         return GJVFile;
      }
      bool isLogFile()
      {
         return logFile;
      }
      bool needsExport()
      {
         return needExport;
      }

      virtual void processMinosStanza( const std::string &methodName, MinosTestImport *mt );
      void setStanza( unsigned int stanza, int stanzaStart );
      virtual bool getStanza( unsigned int Stanza, std::string &stanzaData );

      // The contest details

      // Bundles are specific to the off disk logs, not the monitored logs

      SettingsBundle entryBundle;      // individual LoggerContestLog
      MinosItem<std::string> entryBundleName;
      SettingsBundle QTHBundle;        // individual QTH
      MinosItem<std::string> QTHBundleName;
      SettingsBundle stationBundle;    // individual station
      MinosItem<std::string> stationBundleName;

      MinosItem<std::string> VHFContestName;

      // Most of this info is bundle related, and not relevant when monitored (?)

      MinosItem<std::string> entrant;
      MinosItem<std::string> entSect;
      MinosItem<std::string> sqth1;
      MinosItem<std::string> sqth2;
      MinosItem<std::string> entTx;
      MinosItem<std::string> entRx;
      MinosItem<std::string> entAnt;
      MinosItem<std::string> entAGL;
      MinosItem<std::string> entASL;
      MinosItem<std::string> ops1;        // list of ops for contest
      MinosItem<std::string> ops2;        // list of ops for contest
      MinosItem<std::string> entCondx1;
      MinosItem<std::string> entCondx2;
      MinosItem<std::string> entCondx3;
      MinosItem<std::string> entCondx4;
      MinosItem<std::string> entName;
      MinosItem<std::string> entCall;
      MinosItem<std::string> entAddr1;
      MinosItem<std::string> entAddr2;
      MinosItem<std::string> entPostCode;
      MinosItem<std::string> entCity;
      MinosItem<std::string> entCountry;
      MinosItem<std::string> entPhone;
      MinosItem<std::string> entEMail;
      MinosItem<std::string> sectionList; // needed to be carried forward

      // dirty info is only relevant when it is being editted
      // but needs to stay with the data

      virtual void clearDirty();
      virtual void setDirty();
      // end of contest details


      std::string bands;
      std::string sections;

      void setINIDetails();

      // startup/closedown

      LoggerContestLog( void );
      void initialiseINI();
      bool initialise( int slotno );
      bool initialise( const std::string &, bool, int slotno );
      ~LoggerContestLog();


      // common file stuff
      bool commonSave( bool newfile );
      void closeFile( void );

      // GJV file manipulation
      int readBlock( int bno );
      int writeBlock( HANDLE fd, int bno );
      bool GJVsave( GJVParams & );
      virtual bool GJVload( void );
      bool GJVloadContacts( void );

      // minos file
      virtual bool minosSaveFile( bool newfile );
      virtual bool minosSaveContestContact( const ContestContact *lct );
      // Import/export etc

      bool export_contest( HANDLE, ExportType );
      bool exportGJV( HANDLE );
      void procUnknown( BaseContact *cct, writer &wr );
      bool exportADIF( HANDLE expfd );
      bool exportKML( HANDLE expfd );
      bool exportREG1TEST( HANDLE expfd );
      bool exportMinos( HANDLE expfd );

      virtual void makeContact( bool time_now, BaseContact *& );
      DisplayContestContact *addContact( int newctno, int extra_flags, bool save_new );

};

#endif
