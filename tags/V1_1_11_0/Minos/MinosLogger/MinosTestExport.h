/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosTestExportH
#define MinosTestExportH 
//---------------------------------------------------------------------------

class RPCParam;
class RPCParamStruct;
class RPCRequest;
class MinosTestExport
{
   private:
      LoggerContestLog *ct;

      int stanzaCount;
      void makeHeader( RPCParamStruct * st, unsigned long ts );
      void sendRequest( HANDLE expfd, const std::string &cmd, RPCParamStruct *st );
      void exportMode( HANDLE expfd );
      void exportContest( HANDLE expfd );
      void exportQTH( HANDLE expfd );
      void exportEntry( HANDLE expfd );
      void exportStation( HANDLE expfd );
      void exportCurrent( HANDLE expfd );
      void exportOperators( HANDLE expfd );
      void exportBundles( HANDLE expfd );

   public:
      MinosTestExport( LoggerContestLog * const ct );
      MinosTestExport( );
      ~MinosTestExport();

      int exportAllDetails( HANDLE expfd, bool newfile );
      int exportTest( HANDLE expfd, int mindump, int maxdump );

      int exportComment( HANDLE expfd, const ContestContact *lct );
      int exportQSO( HANDLE expfd, const ContestContact *lct );

};
#endif
