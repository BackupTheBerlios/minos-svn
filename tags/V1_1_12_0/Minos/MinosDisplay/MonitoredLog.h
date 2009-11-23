/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MonitoredLogH
#define MonitoredLogH 
//---------------------------------------------------------------------------
class TLogMonitorFrame;
class TMonitoringFrame;

class MonitoredLog
{
   private:

      // want the log file, log name,
      // We also need the subscribed position, so we can bring it up to date

      // The ContestLog is then aggregated rather than being inherited from,
      // and (?) gets a normal slot in the logger if used there - a ContestLog
      // is aggregated there as well - looks like we need boost::shared_ptr

      // So do we bite the bullet and write remote logs to disk as well.
      // Then it is just a matter of sorting out name (ideally as for the remote
      // station) and path - use a default path + station name subdirs

      // A BIG difference is that contacts are entered by pubsub/RPC call
      // rather than being typed in

      // Currently contacts are added in in TGJVQSOLogFrame::logScreenEntry

      // This will eventually lend itself to a second op position being able to
      // claim a serial and then later "editing" it

      void getLogStanza( int sno );
      bool monitorEnabled;

      DWORD inStanzaRequest;
      std::set
         <int> stanzasPulled;

      int lastScannedStanza;

      PublishState state;
      std::string server;
      std::string publishedName;
      int expectedStanzaCount;
      MinosTestImport *mt;
      BaseContestLog * contest;
      TMonitoringFrame *frame;

   public:


      MonitoredLog();
      ~MonitoredLog();

      bool enabled()
      {
         return monitorEnabled;
      }

      void initialise( const std::string &server, const std::string &name );
      BaseContestLog * getContest()
      {
         return contest;
      }
      std::string getPublishedName()
      {
         return publishedName;
      }
      void setExpectedStanzaCount( int ec )
      {
         expectedStanzaCount = ec;
      }
      void setFrame( TMonitoringFrame *f )
      {
         frame = f;
      }
      void setState(PublishState s)
      {
         state = s;
      }
      PublishState getState()
      {
         return state;
      }
      void startMonitor();
      void stopMonitor();
      void checkMonitor();
      void processLogStanza( int stanza, const std::string &stanzaData );
};
#endif
