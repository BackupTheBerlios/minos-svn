/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop

#include "MinosTestExport.h"

//==============================================================================
std::string fileHeader = "<!--\r\n"
                         "====================================================\r\n"
                         "\r\nDO NOT SEND THIS FILE AS YOUR ENTRY!\r\n\r\n"
                         "Use \"File\" | \"Produce Entry/Export File...\"\r\n"
                         "Export as Reg1Test(entry)\r\n"
                         "and send the .EDI file produced.\r\n"
                         "====================================================\r\n"
                         "-->\r\n";
//==============================================================================
MinosTestExport::MinosTestExport( LoggerContestLog * const ct ) : ct( ct ),
      stanzaCount( ct->getStanzaCount() )
{}
MinosTestExport::MinosTestExport( ) : ct( 0 ), stanzaCount( 0 )
{}
MinosTestExport::~MinosTestExport()
{}
// We need to be able to send stanzas remote - or do we do it from the disk copy?

// AND (as for import) we need to get the contest object to provide the stanzas

void MinosTestExport::sendRequest( HANDLE expfd, const std::string &cmd, RPCParamStruct *st )
{
   RPCArgs * MArgs = new RPCArgs;
   MArgs->addParam( boost::shared_ptr<RPCParam>(st) );

   RPCRequest *m = new RPCRequest( "", cmd );
   m->args = MArgs->args;

   std::string req = m->getActionMessage() + "\r\n";
   DWORD fpos;
   fpos = ::SetFilePointer( expfd, 0, 0, FILE_END );
   if ( fpos == 0xFFFFFFFF )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "(write) seek failed!" );
   }
   DWORD written;
   bool res = WriteFile( expfd, req.c_str(), req.size(), &written, 0 );
   if ( !res || written != req.size() )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
   }
   // set the stanza into the contest so we can monitor it later
   ct->setStanza( stanzaCount + 1, fpos );

   delete m;
   delete MArgs;
}
void MinosTestExport::makeHeader( RPCParamStruct * st, unsigned long ls )
{
   dtg tnow( true );
   st->addMember( ( int ) ls, "lseq" );
   st->addDtgMember( tnow.getIsoDTG(), "uDTG" );
}
void MinosTestExport::exportMode( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;

   ct->readOnly.addIfDirty( st, "readOnly", dirty );
   if ( dirty )
   {
      sendRequest( expfd, "MinosLogMode", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }
}
void MinosTestExport::exportContest( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;

   ct->name.addIfDirty( st, "name", dirty );
   ct->band.addIfDirty( st, "band", dirty );
   ct->entSect.addIfDirty( st, "section", dirty );
   ct->sectionList.addIfDirty( st, "sectionList", dirty );
   if ( ct->scoreMode.isDirty() )
   {
      st->addMember( ( ct->scoreMode.getValue( dirty ) == PPKM ), "scoreKms" );
   }
   ct->DTGStart.addIfDirty( st, "startTime", dirty );
   ct->DTGEnd.addIfDirty( st, "endTime", dirty );
   ct->districtMult.addIfDirty( st, "districtMult", dirty );
   ct->countryMult.addIfDirty( st, "DXCCMult", dirty );
   ct->locMult.addIfDirty( st, "locMult", dirty );
   ct->otherExchange.addIfDirty( st, "QTHReq", dirty );
   ct->allowLoc4.addIfDirty( st, "AllowLoc4", dirty );
   ct->allowLoc8.addIfDirty( st, "AllowLoc8", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogContest", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }
}
void MinosTestExport::exportQTH( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;
   ct->myloc.loc.addIfDirty( st, "locator", dirty );
   ct->location.addIfDirty( st, "district", dirty );  // We need to differentiate location from exchange!
   ct->location.addIfDirty( st, "location", dirty );
   ct->sqth1.addIfDirty( st, "stationQTH1", dirty );
   ct->sqth2.addIfDirty( st, "stationQTH2", dirty );
   ct->entASL.addIfDirty( st, "ASL", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogQTH", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }
}
void MinosTestExport::exportEntry( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;
   ct->mycall.fullCall.addIfDirty( st, "call", dirty );
   ct->entrant.addIfDirty( st, "entrant", dirty );
   ct->entName.addIfDirty( st, "myName", dirty );
   ct->entCall.addIfDirty( st, "myCall", dirty );
   ct->entAddr1.addIfDirty( st, "myAddress1", dirty );
   ct->entAddr2.addIfDirty( st, "myAddress2", dirty );
   ct->entCity.addIfDirty( st, "myCity", dirty );
   ct->entCountry.addIfDirty( st, "myCountry", dirty );
   ct->entPostCode.addIfDirty( st, "myPostCode", dirty );
   ct->entPhone.addIfDirty( st, "myPhone", dirty );
   ct->entEMail.addIfDirty( st, "myEmail", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogEntry", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }
}
void MinosTestExport::exportStation( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;
   ct->power.addIfDirty( st, "power", dirty );
   ct->entTx.addIfDirty( st, "transmitter", dirty );
   ct->entRx.addIfDirty( st, "receiver", dirty );
   ct->entAnt.addIfDirty( st, "antenna", dirty );
   ct->entAGL.addIfDirty( st, "AGL", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogStation", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }
}
void MinosTestExport::exportCurrent( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;
   ct->entCondx1.addIfDirty( st, "condx1", dirty );
   ct->entCondx2.addIfDirty( st, "condx2", dirty );
   ct->entCondx3.addIfDirty( st, "condx3", dirty );
   ct->entCondx4.addIfDirty( st, "condx4", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogCurrent", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }
}
void MinosTestExport::exportOperators( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;

   ct->ops1.addIfDirty( st, "ops1", dirty );
   ct->ops2.addIfDirty( st, "ops2", dirty );
   ct->currentOp1.addIfDirty( st, "currentOp1", dirty );
   ct->currentOp2.addIfDirty( st, "currentOp2", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogOperators", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }

}
void MinosTestExport::exportBundles( HANDLE expfd )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   // This wants "useBundles"(?) and the bundle names selected
   bool dirty = false;

   ct->entryBundleName.addIfDirty( st, "entryBundle", dirty );
   ct->QTHBundleName.addIfDirty( st, "QTHBundle", dirty );
   ct->stationBundleName.addIfDirty( st, "stationBundle", dirty );
   ct->VHFContestName.addIfDirty(st, "VHFContestName", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogBundles", st );
      stanzaCount++;
   }
   else
   {
      delete st;
   }

}
int MinosTestExport::exportComment( HANDLE expfd, const ContestContact *lct )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, 1 );

   bool dirty = false;
   if ( lct->time.isDirty() )
   {
      st->addDtgMember( lct->time.getIsoDTG( dirty ), "logTime" );
   }
   if ( lct->contactFlags.isDirty() )
   {
      st->addMember( bool( lct->contactFlags.getValue( dirty ) & LOCAL_COMMENT != 0 ), "LocalComment" );
      st->addMember( bool( lct->contactFlags .getValue( dirty ) & DONT_PRINT != 0 ), "DontPrint" );
   }
   lct->comments.addIfDirty( st, "comment", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogComment", st );
      stanzaCount++;
      return 1;
   }
   else
   {
      delete st;
      return 0;
   }
}
int MinosTestExport::exportQSO( HANDLE expfd, const ContestContact *lct )
{
   RPCParamStruct * st = new RPCParamStruct;
   makeHeader( st, lct->getLogSequence() );

   bool dirty = false;
   if ( lct->time.isDirty() )
   {
      st->addDtgMember( lct->time.getIsoDTG( dirty ), "logTime" );
   }
   if ( lct->contactFlags.isDirty() )
   {
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & VALID_DISTRICT ) != 0 ), "validDistrict" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & COUNTRY_FORCED ) != 0 ), "countryForced" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & UNKNOWN_COUNTRY ) != 0 ), "unknownCountry" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & NON_SCORING ) != 0 ), "nonScoring" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & MANUAL_SCORE ) != 0 ), "manualScore" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & DONT_PRINT ) != 0 ), "dontPrint" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & VALID_DUPLICATE ) != 0 ), "validDuplicate" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & TO_BE_ENTERED ) != 0 ), "toBeEntered" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & XBAND ) != 0 ), "xBand" );
      st->addMember( bool( ( lct->contactFlags.getValue( dirty ) & FORCE_LOG ) != 0 ), "Forced" );
   }
   lct->cs.fullCall.addIfDirty( st, "callRx", dirty );
   lct->reps.addIfDirty( st, "rstTx", dirty );
   lct->serials.addIfDirty( st, "serialTx", dirty );
   //   lct->extraText.addIfDirty(st, "exchangeTx", dirty);
   lct->repr.addIfDirty( st, "rstRx", dirty );
   lct->serialr.addIfDirty( st, "serialRx", dirty );
   lct->loc.loc.addIfDirty( st, "locRx", dirty );
   lct->extraText.addIfDirty( st, "exchangeRx", dirty );
   lct->extraText.addIfDirty( st, "qthRx", dirty );
   lct->comments.addIfDirty( st, "commentsTx", dirty );
   lct->comments.addIfDirty( st, "commentsRx", dirty );
   lct->mode.addIfDirty( st, "modeTx", dirty );
   lct->mode.addIfDirty( st, "modeRx", dirty );
   ct->power.addIfDirty( st, "power", dirty );
   ct->band.addIfDirty( st, "band", dirty );
   lct->forcedMult.addIfDirty( st, "forcedMult", dirty );
   lct->contactScore.addIfDirty( st, "claimedScore", dirty );
   lct->op1.addIfDirty( st, "op1", dirty );
   lct->op2.addIfDirty( st, "op2", dirty );

   if ( dirty )
   {
      sendRequest( expfd, "MinosLogQSO", st );
      stanzaCount++;
      return 1;
   }
   else
   {
      delete st;
      return 0;
   }

}
int MinosTestExport::exportAllDetails( HANDLE minosContestFile, bool newfile )
{
   stanzaCount = 0;
   if ( newfile )
   {
      //      std::string lbuff = "<?xml version='1.0'?><stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' version='1.0'>" ;
      std::string lbuff = "<?xml version='1.0'?><stream:stream xmlns:stream='http://minos.goodey.org.uk/streams' xmlns='minos:client' version='1.0'>" ;
      lbuff += "\r\n" + fileHeader;
      DWORD ret = 0;
      bool res = WriteFile( minosContestFile, lbuff.c_str(), lbuff.length(), &ret, 0 );
      if ( !res || ret != lbuff.length() )
      {
         MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
      }
   }
   exportMode( minosContestFile );
   exportContest( minosContestFile );
   exportQTH( minosContestFile );
   exportEntry( minosContestFile );
   exportStation( minosContestFile );
   exportCurrent( minosContestFile );
   exportOperators( minosContestFile );
   exportBundles( minosContestFile );

   return stanzaCount;
}
// AND we need an export operator change
int MinosTestExport::exportTest( HANDLE expfd, int mindump, int maxdump )
{
   stanzaCount = 0;
   std::string lbuff = "<?xml version='1.0'?><stream:stream xmlns:stream='http://minos.goodey.org.uk/streams' xmlns='minos:client' version='1.0'>" ;
   lbuff += "\r\n" + fileHeader;
   DWORD ret = 0;
   bool res = WriteFile( expfd, lbuff.c_str(), lbuff.length(), &ret, 0 );
   if ( !res || ret != lbuff.length() )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
   }

   // export a sequence of Minos stanzas

   // Don't want to export Minos as read only!
   bool wasRO = ct->readOnly.getValue();

   ct->readOnly.setValue( false );

   exportMode( expfd );

   ct->readOnly.setValue( wasRO );

   exportContest( expfd );
   exportQTH( expfd );
   exportEntry( expfd );
   exportStation( expfd );
   exportCurrent( expfd );
   exportOperators( expfd );     // not right... we need to log op changes
   exportBundles( expfd );

   bool inDump = false;
   for ( unsigned int i = 0; i < ct->ctList.size(); i++ )
   {
      BaseContact *dct = ct->ctList[ i ];
      ContestContact *lct = dynamic_cast<ContestContact *>( dct );

      if ( inDump && lct->contactFlags.getValue() & ( LOCAL_COMMENT | COMMENT_ONLY ) )
      {
         exportComment( expfd, lct );
         continue;
      }

      int serials = atoi( lct->serials.getValue().c_str() );
      // dump the contact, until serial seen

      if ( ( serials >= mindump ) || ( mindump == 0 ) )
         inDump = true;

      if ( inDump && ( serials <= maxdump ) )
      {
         exportQSO( expfd, lct );
      }

      // Cannot assume that all serials are in order, so we need to go through
      // the whole contest
   }

   return stanzaCount;
}
//=============================================================================

