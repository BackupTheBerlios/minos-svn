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

#include "BandList.h"
#include "LoggerContest.h"
#include "MultDisp.h"
#include "MinosTestImport.h"

ContestContact::ContestContact( LoggerContestLog * ct, bool time_now ) : DisplayContestContact( ct, time_now )
{}
ContestContact::~ContestContact()
{}
void ContestContact::clearDirty()
{
   DisplayContestContact::clearDirty();
}
void ContestContact::setDirty()
{
   DisplayContestContact::setDirty();
}
static void catmult( char *multbuff, const std::string &text, int limit = 255 )
{
   strcat( multbuff, "*" );
   strncat( multbuff, text.c_str(), limit );
   strcat( multbuff, "* " );
}
int ContestContact::getRSGBLogText( std::string &sdest, short maxlen )
{
   BaseContestLog * clp = contest;
   sdest = "";
   int thisscore = 0;
   if ( contactFlags.getValue() & ( LOCAL_COMMENT | DONT_PRINT ) )
      return 0;

   TEMPBUFF( multbuff, 60 );
   TEMPBUFF( dest, 1024 );

   memset( dest, ' ', maxlen - 1 );
   dest[ maxlen ] = 0;
   if ( contactFlags.getValue() & COMMENT_ONLY )
   {
      memset( ContactBuffs::buff2, ' ', 120 );
      placestr( ContactBuffs::buff2, time.getTime( DTGDISP ), 4, 5 );
      placestr( ContactBuffs::buff2, "LOGGED COMMENT", 21, 14 );
      placestr( ContactBuffs::buff2, comments.getValue(), 29, 60 );

   }
   else
   {
      makestrings( clp->serialField.getValue() );
      if ( districtMult )
         sprintf( ContactBuffs::qthbuff, "%3.3s", districtMult->districtCode.c_str() );
      else
      {
         strncpy( ContactBuffs::qthbuff, extraText.getValue().c_str(), EXTRALENGTH );
         ContactBuffs::qthbuff[ EXTRALENGTH ] = 0;
      }

      TEMPBUFF( exp_buff, 60 );
      exp_buff[ 0 ] = 0;
      if ( contactFlags.getValue() & NON_SCORING )
      {
         strcpy( ContactBuffs::scorebuff, "0" );
         strcpy( exp_buff, "No Score Claimed " );
      }
      else
         if ( cs.valRes == ERR_DUPCS )
         {
            strcpy( ContactBuffs::scorebuff, "0" );
            strcpy( exp_buff, "Duplicate " );
         }
         else
         {
            if ( contactFlags.getValue() & VALID_DUPLICATE )
               strcpy( exp_buff, "BackPacker " );

            if ( contactFlags.getValue() & VALID_DISTRICT )
               strcat( exp_buff, "No district code " );

            thisscore = contactScore.getValue();
            switch ( clp->scoreMode.getValue() )
            {
               case PPKM:
                  if ( contactFlags.getValue() & XBAND )
                  {
                     thisscore = ( thisscore + 1 ) / 2;
                  }
                  break;

               case PPQSO:
                  if ( thisscore > 0 )
                     thisscore = 1;
                  else
                     thisscore = 0;

                  break;

            }
            if ( contactScore.getValue() < 0 )
               thisscore = 0;
            sprintf( ContactBuffs::scorebuff, "%5d", thisscore );
         }
      ContactBuffs::scorebuff[ 5 ] = 0;
      multbuff[ 0 ] = 0;

      if ( multCount )
      {
         if ( newCtry )
            catmult( multbuff, ctryMult->basePrefix );

         if ( newDistrict )
            catmult( multbuff, districtMult->districtCode );

         if ( newLoc )
         {
            catmult( multbuff, loc.loc.getValue(), 4 );
         }
      }
      strcpysp( ContactBuffs::buff, comments.getValue().c_str(), 42 );
      if ( ContactBuffs::buff[ 0 ] )
      {
         strcpysp( ContactBuffs::buff2, ContactBuffs::qthbuff, 20 );
         strcat( ContactBuffs::buff2, " | " );
         strcat( ContactBuffs::buff2, ContactBuffs::buff );
      }
      else
      {
         strcpysp( ContactBuffs::buff2, ContactBuffs::qthbuff, 42 );
      }
      sprintf( ContactBuffs::buff, "%s %s %s", exp_buff, ContactBuffs::buff2, multbuff );

      memset( ContactBuffs::buff2, ' ', 120 );
      int next = 0;
      next = placestr( ContactBuffs::buff2, time.getTime( DTGDISP ), 4, 5 );
      next = placestr( ContactBuffs::buff2, cs.fullCall.getValue(), next + 1, 10 );
      next = placestr( ContactBuffs::buff2, reps.getValue(), next + 1, 3 );
      next = placestr( ContactBuffs::buff2, ContactBuffs::ssbuff, next, -4 );
      next = placestr( ContactBuffs::buff2, repr.getValue(), next + 1, 3 );
      next = placestr( ContactBuffs::buff2, ContactBuffs::srbuff, next, -4 );
      next = placestr( ContactBuffs::buff2, loc.loc.getValue(), next + 1, clp->allowLoc8.getValue() ? 8 : 6 );
      next = placestr( ContactBuffs::buff2, ContactBuffs::buff, next + 1, 30 );
      placestr( ContactBuffs::buff2, ContactBuffs::scorebuff, 75, -5 );
   }
   strncpy( dest, ContactBuffs::buff2, maxlen );
   dest[ maxlen ] = 0;
   // return value is point score for this line, for accumulation

   sdest = std::string( dest ).substr( 0, maxlen );

   if ( contactScore.getValue() < 0 )
      return 0;

   return thisscore;
}
void ContestContact::getRSGBFileText( std::string &sdest, short maxlen )
{
   BaseContestLog * clp = contest;
   sdest = "";
   if ( contactFlags.getValue() & ( LOCAL_COMMENT | DONT_PRINT ) )
      return ;

   TEMPBUFF( exp_buff, 60 );
   exp_buff[ 0 ] = 0;
   ContactBuffs::qthbuff[ 0 ] = 0;

   TEMPBUFF( dest, 1024 );
   *dest = 0;

   if ( contactFlags.getValue() & COMMENT_ONLY )
   {
      memset( ContactBuffs::buff2, ' ', 120 );
      memcpy( ContactBuffs::buff2, time.getDate( DTGLOG ).c_str(), 6 );
      memcpy( &ContactBuffs::buff2[ 7 ], time.getTime( DTGLOG ).c_str(), 4 );
      placestr( ContactBuffs::buff2, clp->band.getValue(), 12, 4 );
      placestr( ContactBuffs::buff2, mode.getValue(), 17, 3 );
      placestr( ContactBuffs::buff2, "COMMENT", 93, 7 );
      placestr( ContactBuffs::buff2, comments.getValue(), 101, 60 );
      strncpy( dest, ContactBuffs::buff2, maxlen );
      dest[ maxlen ] = 0;
      sdest = std::string( dest ).substr( 0, maxlen );
      return ;
   }

   makestrings( clp->serialField.getValue() );

   if ( !districtMult )
   {
      strncpy( ContactBuffs::qthbuff, extraText.getValue().c_str(), EXTRALENGTH );
      ContactBuffs::qthbuff[ EXTRALENGTH ] = 0;
   }

   strcpy( ContactBuffs::scorebuff, "0" );
   if ( contactFlags.getValue() & NON_SCORING )
   {
      strcpy( exp_buff, "Non Scoring" );
   }
   else
      if ( cs.valRes == ERR_DUPCS )
      {
         strcpy( exp_buff, "Duplicate" );
      }
      else
      {
         if ( contactFlags.getValue() & VALID_DUPLICATE )
            strcpy( exp_buff, "BackPacker " );

         if ( contactFlags.getValue() & VALID_DISTRICT )
            strcat( exp_buff, "No district code " );

         //      if (contactFlags & UNKNOWN_COUNTRY)
         //         strcat(exp_buff, "Unknown Country ");

         int temp = contactScore.getValue();
         switch ( clp->scoreMode.getValue() )
         {
            case PPKM:
               if ( contactFlags.getValue() & XBAND )
               {
                  temp = ( temp + 1 ) / 2;
               }
               break;

            case PPQSO:
               if ( temp > 0 )
                  temp = 1;
               else
                  temp = 0;
               break;

         }
         if ( contactScore.getValue() < 0 )
            temp = 0;
         sprintf( ContactBuffs::scorebuff, "%5d", temp );
      }
   ContactBuffs::scorebuff[ 5 ] = 0;
   strcpysp( ContactBuffs::buff, comments.getValue().c_str(), 42 );
   if ( ContactBuffs::buff[ 0 ] )
   {
      strcpysp( ContactBuffs::buff2, ContactBuffs::qthbuff, 20 );
      strcat( ContactBuffs::buff2, " | " );
      strcat( ContactBuffs::buff2, ContactBuffs::buff );
   }
   else
      strcpysp( ContactBuffs::buff2, ContactBuffs::qthbuff, 42 );

   sprintf( ContactBuffs::qthbuff, "%s %s", exp_buff, ContactBuffs::buff2 );

   memset( ContactBuffs::buff, ' ', 255 );
   memcpy( ContactBuffs::buff, time.getDate( DTGLOG ).c_str(), 6 );
   memcpy( &ContactBuffs::buff[ 7 ], time.getTime( DTGLOG ).c_str(), 4 );
   placestr( ContactBuffs::buff, clp->band.getValue(), 12, 4 );	// only 4 chars
   placestr( ContactBuffs::buff, mode.getValue(), 17, 3 );
   placestr( ContactBuffs::buff, cs.fullCall.getValue(), 21, 15 );
   placestr( ContactBuffs::buff, reps.getValue(), 37, 3 );
   placestr( ContactBuffs::buff, ContactBuffs::ssbuff, 41, -4 );
   placestr( ContactBuffs::buff, repr.getValue(), 46, 3 );
   placestr( ContactBuffs::buff, ContactBuffs::srbuff, 50, -4 );
   if ( multCount )
   {
      memset( ContactBuffs::buff2, ' ', 120 );
      sprintf( ContactBuffs::buff2, "%d", multCount );
      placestr( ContactBuffs::buff, ContactBuffs::buff2, 55, -3 );		// mult count
   }
   placestr( ContactBuffs::buff, ContactBuffs::scorebuff, 59, -5 );
   placestr( ContactBuffs::buff, op1.getValue(), 65, 6 );

   placestr( ContactBuffs::buff, loc.loc.getValue(), 72, clp->allowLoc8.getValue() ? 8 : 6 );
   if ( newLoc )
      ContactBuffs::buff[ 79 ] = 'M';

   if ( districtMult )
      placestr( ContactBuffs::buff, districtMult->districtCode, 81, 3 );
   if ( newDistrict )
      ContactBuffs::buff[ 85 ] = 'M';

   if ( ctryMult )
      placestr( ContactBuffs::buff, ctryMult->basePrefix, 87, -3 );
   if ( newCtry )
      ContactBuffs::buff[ 91 ] = 'M';

   placestr( ContactBuffs::buff, ContactBuffs::qthbuff, 93, 80 );
   strncpy( dest, ContactBuffs::buff, maxlen );
   dest[ maxlen ] = 0;
   sdest = std::string( dest ).substr( 0, maxlen );
}
void ContestContact::addReg1TestComment( std::vector <std::string> &remarks )
{
   std::string cmnt;
   cmnt += time.getDate( DTGLOG );
   cmnt += ';';
   cmnt += time.getTime( DTGLOG );
   cmnt += ';';
   if ( contactFlags.getValue() & ( LOCAL_COMMENT | DONT_PRINT ) )
      return ;
   if ( contactFlags.getValue() & ( COMMENT_ONLY ) )
   {
      cmnt += comments.getValue();
      remarks.push_back( cmnt );
      return ;
   }

   if ( trim( comments.getValue() ).size() )
   {
      cmnt += cs.fullCall.getValue();
      cmnt += ';';
      cmnt += trim( comments.getValue() );
      remarks.push_back( cmnt );
      return ;
   }
   return ;
}
void ContestContact::getReg1TestText( std::string &sdest )
{
   /*
      Date(6);Time(4);Call(3 - 14);Mode code(1 below);
      Sent-RST(0 or 2 or 3);Sent QSO number(0 or 3 or 4, with leading zeroes);
      Received-RST(0 or 2 or 3);Received QSO number(0 or 3 or 4, with leading zeroes);
      Received exchange(0 or 1-6);Received-WWL(0 or 4 or 6);
      QSO-Points(1-6 inc band mult);New-Exchange-(N)(0 or 1);
      New-WWL-(N)(0 or 1);New-DXCC-(N)(0 or 1);
      Duplicate-QSO-(D)(0 or 1)

      (N) says either "N;" or ";"

      code TXmode/RXmode
   0 none/none (or not specified)
   1 SSB/SSB
   2 CW/CW
   3 SSB/CW
   4 CW/SSB
   5 AM/AM
   6 FM/FM
   7 RTTY/RTTY
   8 SSTV/SSTV
   9 ATV/ATV
   If the mode is not important it can be left blank, i.e. not stated
   in rules/invitation.

   If the format is used for a LoggerContestLog which does not use some
   of the QSO exchanges, i.e. QSO no., WWL and Exchange,
   these fields are left blank. Proper interpretation must be
   ensured by manager program.

   Faulty QSOs

   A duplicate QSO is marked with a "D" in the Duplicate-QSO
   field, and the QSO-points field is set to 0 (zero). The format
   does not define when a QSO is a duplicate.
   An incomplete QSO is written with the information received,
   and the QSO-points field is set to zero (0).
   In case of a mistake, an error mark must be inserted in the
   Callsign field to keep a correct flow in the number of QSOs
   records. The error mark must be an "ERROR" and the other
   fields except Time and Sent QSO no., if used, can be left
   empty. In case the empty field is accumulated, e.g. QSOpoints,
   it is set to 0 (zero).

   Missing information

   If a LoggerContestLog log program can not fill in all the information, the
   missing information can be left blank, except if information is
   needed for claiming/calculating scores, e.g. log program
   cannot identify WWLs, DXCCs etc. If the information is
   required for the scores this log program can not be used for
   this particular LoggerContestLog anyway.

   */
   sdest = "";
   if ( contactFlags.getValue() & ( LOCAL_COMMENT | COMMENT_ONLY | DONT_PRINT ) )
      return ;

   BaseContestLog * clp = contest;

   sdest += time.getDate( DTGReg1Test );
   sdest += ';';
   sdest += time.getTime( DTGReg1Test );
   sdest += ';';

   //callsign
   sdest += cs.fullCall.getValue();
   sdest += ';';

   sdest += "";   // Mode code - always blank (not important) for now
   sdest += ';';

   sdest += reps.getValue();   // TX RST
   sdest += ';';

   AnsiString ssbuff;
   int ss = atoi( serials.getValue().c_str() );
   if ( ss )
      ssbuff.printf( "%03.3d", ss );
   sdest += ssbuff.c_str();   // TX sno
   sdest += ';';

   sdest += repr.getValue();   // RX RST
   sdest += ';';

   AnsiString srbuff;
   int sr = atoi( serialr.getValue().c_str() );
   if ( sr )
      srbuff.printf( "%03.3d", sr );
   sdest += srbuff.c_str();   // RX sno
   sdest += ';';

   // exch
   if ( clp->districtMult.getValue() )
   {
      if ( districtMult )
         sdest += districtMult->districtCode;
   }
   else
      if ( clp->otherExchange.getValue() )
      {
         sdest += extraText.getValue();
      }
   sdest += ';';
   sdest += loc.loc.getValue();
   sdest += ';';
   //points
   int temp = contactScore.getValue();
   switch ( clp->scoreMode.getValue() )
   {
      case PPKM:
         break;

      case PPQSO:
         if ( temp > 0 )
            temp = 1;
         else
            temp = 0;
         break;

   }
   if ( contactScore.getValue() <= 0 )
      temp = 0;

   sprintf( ContactBuffs::scorebuff, "%d", temp );
   sdest += ContactBuffs::scorebuff;

   sdest += ';';
   if ( newDistrict )
      sdest += 'N';
   sdest += ';';

   if ( newLoc )
      sdest += 'N';
   sdest += ';';

   if ( newCtry )
      sdest += 'N';
   sdest += ';';

   if ( cs.valRes == ERR_DUPCS )
      sdest += 'D';
}

// this is the "archive" copy that forms the real log

// we form our log by having a TSortedCollection derivative (class LoggerContestLog)
// this has to be sorted on the logSequence field


std::string ContestContact::getADIFLine()
{
   //band
   //power
   //date
   //time
   //call
   //mode
   //RST sent
   //SN sent
   //RST Received
   //SN received
   //GridSquare
   //QTH, district, whatever
   //Comments
   //QSO Pts

   std::string outstr;
   BaseContestLog * clp = contest;
   if ( !bool( clp ) )
      return outstr;

   TEMPBUFF( exp_buff, 60 );

   if ( contactFlags.getValue() & ( LOCAL_COMMENT | DONT_PRINT ) )
      return outstr;

   memcpy( exp_buff, time.getDate( DTGLOG ).c_str(), 6 );
   exp_buff[ 6 ] = 0;
   std::string cent = "19";
   if ( atoi( exp_buff ) < 300000 )
      cent = "20";

   outstr += makeADIFField( "QSO_DATE", cent + exp_buff );

   memcpy( exp_buff, time.getTime( DTGLOG ).c_str(), 4 );
   exp_buff[ 4 ] = 0;
   outstr += makeADIFField( "TIME_ON", exp_buff );

   std::string cband = clp->band.getValue();

   std::string cb = trim(cband);
   BandList &blist = BandList::getBandList();
   for (unsigned int i = 0; i < blist.bandList.size(); i++)
   {
      if (stricmp(cb, blist.bandList[ i ].uk) == 0
            || stricmp(cb, blist.bandList[ i ].wlen) == 0
            || stricmp(cb, blist.bandList[ i ].adif) == 0
            || stricmp(cb, blist.bandList[ i ].cabrillo) == 0
            || stricmp(cb, blist.bandList[ i ].reg1test) == 0)
      {
         cb = blist.bandList[ i ].adif;
         break;
      }
   }
   outstr += makeADIFField( "BAND", cb );


   if ( stricmp( mode.getValue(), "A1A" ) == 0 )
      outstr += makeADIFField( "MODE", "CW" );
   else
      if ( stricmp( mode.getValue(), "J3E" ) == 0 )
         outstr += makeADIFField( "MODE", "SSB" );
      else
         if ( stricmp( mode.getValue(), "F3E" ) == 0 )
            outstr += makeADIFField( "MODE", "FM" );
         else
            outstr += makeADIFField( "MODE", mode.getValue() );

   if ( contactFlags.getValue() & COMMENT_ONLY )
   {
      outstr += makeADIFField( "COMMENT", comments.getValue() );
      return outstr;
   }
   outstr += makeADIFField( "CALL", cs.fullCall.getValue() );

   outstr += makeADIFField( "RST_SENT", reps.getValue() );
   outstr += makeADIFField( "STX", serials.getValue() );
   outstr += makeADIFField( "RST_RCVD", repr.getValue() );
   outstr += makeADIFField( "SRX", serialr.getValue() );

   outstr += makeADIFField( "GRIDSQUARE", loc.loc.getValue() );
   if ( districtMult )
      outstr += makeADIFField( "QTH", districtMult->districtCode );
   else
      outstr += makeADIFField( "QTH", extraText.getValue() );

   outstr += makeADIFField( "TX_PWR", clp->power.getValue() );
   outstr += makeADIFField( "COMMENT", comments.getValue() );

   int temp = 0;
   if ( contactFlags.getValue() & NON_SCORING )
   {
      temp = 0;
   }
   else
      if ( cs.valRes == ERR_DUPCS )
      {
         temp = 0;
      }
      else
      {
         temp = contactScore.getValue();
         switch ( clp->scoreMode.getValue() )
         {
            case PPKM:
               if ( contactFlags.getValue() & XBAND )
               {
                  temp = ( temp + 1 ) / 2;
               }
               break;

            case PPQSO:
               if ( temp > 0 )
                  temp = 1;
               else
                  temp = 0;
               break;

         }
         if ( contactScore.getValue() < 0 )
            temp = 0;
      }
   outstr += makeADIFField( "OPERATOR", op1.getValue() );
   outstr += makeADIFField( "QSO_PTS", temp );
   outstr += makeADIFField( "QSO_COMPL", temp ? "YES" : "NO" );

   return outstr;
}
bool ContestContact::commonSave( )
{
   bool ret = false;
   LoggerContestLog * clp = dynamic_cast<LoggerContestLog *>( contest );
   if ( !clp->isReadOnly() )
   {
      if ( clp->isGJVFile() )
      {
         MinosParameters::getMinosParameters() ->mshowMessage( "GJV file should have been opened read only!" );
         return false;
      }
      else
         if ( clp->isMinosFile() )
         {
            ret = minosSave();
         }

      if ( ret )
      {
         // search for the country, district, locator multipliers as required
         // Add to the worked counts as required

         cs.valRes = CS_NOT_VALIDATED;
         loc.valRes = LOC_NOT_VALIDATED;
         loc.validate();
         check( );
      }
   }
   return ret;
}
bool ContestContact::minosSave( )
{
   LoggerContestLog * clp = dynamic_cast<LoggerContestLog *>( contest );
   clp->minosSaveContestContact( this );
   BaseContact bc( *this );
   bc.updtime = dtg( true ); // update time is now
   getHistory().push_back( bc );
   setModificationCount( getModificationCount() + 1 );
   return true;
}
bool ContestContact::GJVsave( GJVParams &gp )
{
   const std::string nulc;
   TEMPBUFF( temp, 50 );
   buffpt = 0;

   // write it all into sbuff

   int thisDiskBlock = gp.diskBlock++;
   setLogSequence( ( unsigned long ) ( thisDiskBlock ) << 16 );
   HANDLE fd = gp.fd;

   sprintf( temp, "%d", thisDiskBlock );
   strtobuf( temp );

   sprintf( temp, "%ld", getLogSequence() );
   strtobuf( temp );

   sprintf( temp, "%d", ( unsigned int ) contactFlags.getValue() );
   strtobuf( temp );

   strtobuf( op1.getValue() );
   strtobuf( op2.getValue() );
   strtobuf( time.getDate( DTGDISP ) );
   strtobuf( time.getTime( DTGDISP ) );

   if ( contactFlags.getValue() & ( LOCAL_COMMENT | COMMENT_ONLY ) )
   {
      strtobuf( nulc );   //cs
      strtobuf( nulc );   //reps
      strtobuf( nulc );   //serials
      strtobuf( nulc );   //repr
      strtobuf( nulc );   //serialr
      strtobuf( nulc );   //loc
      strtobuf( nulc );   //extra
   }
   else
   {
      strtobuf( cs.fullCall.getValue() );
      strtobuf( reps.getValue() );
      strtobuf( serials.getValue() );
      strtobuf( repr.getValue() );
      strtobuf( serialr.getValue() );
      strtobuf( loc.loc.getValue() );
      strtobuf( extraText.getValue() );
   }
   strtobuf( comments.getValue() );

   //	strtobuf(power);
   strtobuf( "0" );			// power - removed
   strtobuf( mode.getValue() );

   sprintf( temp, "%d", contactScore );
   strtobuf( temp );

   strtobuf( forcedMult.getValue() );
   //   opyn(PowerWatts);

   strtobuf();    // clear tail
   LoggerContestLog * clp = dynamic_cast<LoggerContestLog *>( contest );
   clp->writeBlock( fd, thisDiskBlock );

   return true;

}
bool ContestContact::GJVload( int diskBlock )
{
   LoggerContestLog * clp = dynamic_cast<LoggerContestLog *>( contest );
   std::string temp;
   clearBuffer();
   clp->readBlock( diskBlock );
   buffpt = 0;

   buftostr( temp );
   if ( atoi( temp.c_str() ) != diskBlock )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "Invalid block number in contact block" );
      return false;
   }

   buftostr( temp );
   setLogSequence( atol( temp.c_str() ) );

   buftostr( temp );
   contactFlags.setInitialValue( atoi( temp.c_str() ) );

   buftostr( temp );
   op1.setInitialValue( temp );
   buftostr( temp );
   op2.setInitialValue( temp );

   buftostr( temp );
   time.setDate( temp, DTGDISP );
   buftostr( temp );
   time.setTime( temp, DTGDISP );

   buftostr( temp );
   cs = callsign( strupr( temp ) );
   cs.valRes = CS_NOT_VALIDATED;

   buftostr( temp );
   reps.setInitialValue( temp );
   buftostr( temp );
   serials.setInitialValue( temp );
   buftostr( temp );
   repr.setInitialValue( temp );
   buftostr( temp );
   serialr.setInitialValue( temp );
   buftostr( temp );
   loc.loc.setInitialValue( temp );
   loc.valRes = LOC_NOT_VALIDATED;
   buftostr( temp );
   extraText.setInitialValue( temp );

   buftostr( temp );
   comments.setInitialValue( temp );

   buftostr( temp );              // power - removed

   buftostr( temp );
   mode.setInitialValue( temp );

   buftostr( temp );
   contactScore.setInitialValue( atoi( temp.c_str() ) );
   bearing = -1;

   buftostr( temp );
   forcedMult.setInitialValue( temp );
   //   if (inyn(myHeap))
   //   {
   //      PowerWatts = true;
   //   }
   //   else
   //   {
   //   	LoggerContestLog * cnt =  dc_contest(contest);
   //      if (cnt)
   //	   	PowerWatts = cnt->PowerWatts;
   //   }
   return true;
}
bool ContestContact::setField( int ACol, const std::string Value )
{
   // This really ought to validate it first...
   switch ( ACol )
   {
         /*case egDate:
            break;
         */
      case egTime:
         time.setTime( Value, DTGDISP );  // VALIDATE!
         break;
         /*case egBand:
            break;
         */
         /*case egPower:
            break;
         */
         /*case egMode:
            break;
         */
      case egCall:
         cs.valRes = CS_NOT_VALIDATED;
         cs = callsign( Value );
         cs.validate( );
         break;
         /*case egCountry:
            break;
         */
      case egRSTTx:
         reps.setValue( Value );
         break;
      case egSNTx:
         serials.setValue( Value );
         break;
      case egRSTRx:
         repr.setValue( Value );
         break;
      case egSNRx:
         serialr.setValue( Value );
         break;
      case egLoc:
         loc.loc.setValue( Value );
         loc.valRes = LOC_NOT_VALIDATED;
         loc.validate();
         break;
      case egBrg:
         return false;
      case egScore:
         Beep();
         return false;
      case egExchange:
         extraText.setValue( Value );
         break;
      case egComments:
         comments.setValue( Value );
         break;
   }
   // here we should save the contact
   commonSave();
   return true;
}
void ContestContact::processMinosStanza( const std::string &methodName, MinosTestImport * const mt )
{
   DisplayContestContact::processMinosStanza( methodName, mt );
}

