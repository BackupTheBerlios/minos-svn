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
#include "AdifImport.h"
#include "reg1test.h"
#include "printfile.h"
#include "enqdlg.h"
#include "MinosTestExport.h"
#include "LoggerContest.h"
#include "TMinosBandChooser.h"
#include "BandList.h"

LoggerContestLog::LoggerContestLog( void ) : BaseContestLog(),
      GJVcontestFile( INVALID_HANDLE_VALUE ), minosContestFile( INVALID_HANDLE_VALUE ),
      logContestFile( INVALID_HANDLE_VALUE ), adifContestFile( INVALID_HANDLE_VALUE ),
      ediContestFile( INVALID_HANDLE_VALUE ),
      GJVFile( false ), minosFile( false ),
      logFile( false ), adifFile( false ), ediFile( false ),
      needExport( false )
{
}
void LoggerContestLog::makeContact( bool timeNow, BaseContact *&lct )
{
   lct = new ContestContact( this, timeNow );
}
LoggerContestLog::~LoggerContestLog()
{
   closeFile();
}
void LoggerContestLog::initialiseINI()
{
   entryBundle.setProfile( BundleFile::bundleFiles[ epENTRYPROFILE ] );
   QTHBundle.setProfile( BundleFile::bundleFiles[ epQTHPROFILE ] );
   stationBundle.setProfile( BundleFile::bundleFiles[ epSTATIONPROFILE ] );
   entryBundleName.setValue( entryBundle.getSection() );
   QTHBundleName.setValue( QTHBundle.getSection() );
   stationBundleName.setValue( stationBundle.getSection() );
}
void LoggerContestLog::clearDirty()
{
   entryBundleName.clearDirty();
   QTHBundleName.clearDirty();
   stationBundleName.clearDirty();

   entrant.clearDirty();
   sqth1.clearDirty();
   sqth2.clearDirty();
   entTx.clearDirty();
   entRx.clearDirty();
   entAnt.clearDirty();
   entAGL.clearDirty();
   entASL.clearDirty();
   ops1.clearDirty();
   ops2.clearDirty();
   entCondx1.clearDirty();
   entCondx2.clearDirty();
   entCondx3.clearDirty();
   entCondx4.clearDirty();
   entName.clearDirty();
   entCall.clearDirty();
   entSect.clearDirty();
   entAddr1.clearDirty();
   entAddr2.clearDirty();
   entPostCode.clearDirty();
   entCity.clearDirty();
   entCountry.clearDirty();
   entPhone.clearDirty();
   entEMail.clearDirty();
   sectionList.clearDirty();
   BaseContestLog::clearDirty();
}
void LoggerContestLog::setDirty()
{
   entryBundleName.setDirty();
   QTHBundleName.setDirty();
   stationBundleName.setDirty();

   entrant.setDirty();
   sqth1.setDirty();
   sqth2.setDirty();
   entTx.setDirty();
   entRx.setDirty();
   entAnt.setDirty();
   entAGL.setDirty();
   entASL.setDirty();
   sectionList.setDirty();
   ops1.setDirty();
   ops2.setDirty();
   entCondx1.setDirty();
   entCondx2.setDirty();
   entCondx3.setDirty();
   entCondx4.setDirty();
   entName.setDirty();
   entCall.setDirty();
   entSect.setDirty();
   entAddr1.setDirty();
   entAddr2.setDirty();
   entPostCode.setDirty();
   entCity.setDirty();
   entCountry.setDirty();
   entPhone.setDirty();
   entEMail.setDirty();

   BaseContestLog::setDirty();
}
bool LoggerContestLog::initialise( int sno )
{
   if ( !TContestApp::getContestApp() ->insertContest( this, sno ) )
   {
      return false;
   }
   slotno = sno;
   return true;
}

bool LoggerContestLog::initialise( const std::string &fn, bool newFile, int slotno )
{
   if ( !initialise( slotno ) )
      return false;

   if ( TContestApp::getContestApp() ->isContestOpen( fn ) )
   {
      return false;
   }

   // open the settings bundle files
   initialiseINI();

   // open the LoggerContestLog file

   cfileName = fn;
   String ext = ExtractFileExt( fn.c_str() );
   publishedName = ExtractFileName( fn.c_str() ).c_str();
   if ( stricmp( ext.c_str(), ".gjv" ) == 0 )
   {
      GJVFile = true;
      setUnwriteable(true);
   }
   else
      if ( stricmp( ext.c_str(), ".minos" ) == 0 )
      {
         minosFile = true;
      }
      else
         if ( stricmp( ext.c_str(), ".log" ) == 0 )
         {
            logFile = true;
            setUnwriteable(true);
         }
         else
            if ( stricmp( ext.c_str(), ".adi" ) == 0 )
            {
               adifFile = true;
               setUnwriteable(true);
            }
            else
               if ( stricmp( ext.c_str(), ".edi" ) == 0 )
               {
                  ediFile = true;
                  setUnwriteable(true);
               }
               else
               {
                  MinosParameters::getMinosParameters() ->mshowMessage( "Not a known file type! (" + ext + ")" );
                  return false;
               }
   if ( !newFile )
   {

      struct stat sbuf;
      if ( stat( fn.c_str(), &sbuf ) == 0 )
      {
         if ( !( sbuf.st_mode & S_IWRITE ) )
         {
            setUnwriteable(true);
         }
      }

      DWORD openMode = GENERIC_READ | GENERIC_WRITE;
      if ( isUnwriteable() )
         openMode = GENERIC_READ;

      // populate the LoggerContestLog object from the file
      //      contest_file = open( fn.c_str(), open_mode );
      HANDLE contestFile = CreateFile( fn.c_str(),
                                       openMode,
                                       FILE_SHARE_READ,
                                       0,                  // security
                                       OPEN_EXISTING,
                                       FILE_FLAG_WRITE_THROUGH,
                                       0 );               // template handle
      if ( contestFile == INVALID_HANDLE_VALUE )
      {
         std::string lerr = lastError();
         std::string emess = "Failed to open LoggerContestLog file " + fn + " : " + lerr;
         MinosParameters::getMinosParameters() ->mshowMessage( emess.c_str() );
         return false;
      }

      bool loadOK = false;
      needExport = false;

      if ( GJVFile )
      {
         GJVcontestFile = contestFile;
         if ( !GJVload() )     // load the header so that we can display it
            return false;
         loadOK = GJVloadContacts();
      }
      else
         if ( minosFile )
         {
            minosContestFile = contestFile;
            MinosTestImport mt( this );
            stanzaCount = mt.importTest( minosContestFile );
            if ( stanzaCount > 0 )
            {
               // set the bundles accordingly
               entryBundle.openSection( entryBundleName.getValue() );
               QTHBundle.openSection( QTHBundleName.getValue() );
               stationBundle.openSection( stationBundleName.getValue() );
               loadOK = true;
            }
         }
         else
            if ( logFile )
            {
               logContestFile = contestFile;
               loadOK = importLOG( logContestFile );
               needExport = true;
            }
            else
               if ( adifFile )
               {
                  adifContestFile = contestFile;
                  loadOK = importAdif( adifContestFile );
                  needExport = true;
               }
               else
                  if ( ediFile )
                  {
                     ediContestFile = contestFile;
                     loadOK = importReg1Test( ediContestFile );
                     needExport = true;
                  }
      scanContest();
      clearDirty();  // what we have just read CAN'T be dirty
      validateLoc();
      // run_contest_dialog has already loaded the LoggerContestLog and set log_count
      // here, we display a "loading" box
      if ( isUnwriteable() )     // Minos files can be unprotected if not realy RO
         closeFile();				// to preserve file handles

      if ( !loadOK )    // sets ct as well
      {
         return false;
      }
   }
   else
      if ( minosFile )
      {
         // Create a new file - they will get to change the name later
         HANDLE contestFile = CreateFile( fn.c_str(),
                                          GENERIC_READ | GENERIC_WRITE,
                                          FILE_SHARE_READ,
                                          0,                  // security
                                          CREATE_NEW,
                                          FILE_FLAG_WRITE_THROUGH,
                                          0 );               // template handle

         if ( contestFile == INVALID_HANDLE_VALUE )
         {
            MinosParameters::getMinosParameters() ->mshowMessage( "Failed to create file" );
            return false;
         }
         if ( GJVFile )
         {
            GJVcontestFile = contestFile;
         }
         else
            if ( minosFile )
            {
               minosContestFile = contestFile;
            }
      }
      else
      {
         return false;
      }
   commonSave( newFile );

   return true;
}
void LoggerContestLog::setINIDetails()
{
   // extras that CAN come from INI file - implements bundle override
   if ( entryBundle.getSection() != noneBundle )
   {
      entryBundle.getStringProfile( eepCall, mycall.fullCall );
      entryBundle.getStringProfile( eepEntrant, entrant );
      entryBundle.getStringProfile( eepMyName, entName );
      entryBundle.getStringProfile( eepMyCall, entCall );
      //   entryBundle.getStringProfile(eepSection, entSect);

      entryBundle.getStringProfile( eepMyAddress1, entAddr1 );
      entryBundle.getStringProfile( eepMyAddress2, entAddr2 );
      entryBundle.getStringProfile( eepMyCity, entCity );
      entryBundle.getStringProfile( eepMyPostCode, entPostCode );
      entryBundle.getStringProfile( eepMyCountry, entCountry );
      entryBundle.getStringProfile( eepMyPhone, entPhone );
      entryBundle.getStringProfile( eepMyEmail, entEMail );
      //   entryBundle.getStringProfile(eepBand, band);
      //   entryBundle.getStringProfile(eepSection, entSect);
   }


   if ( QTHBundle.getSection() != noneBundle )
   {
      QTHBundle.getStringProfile( eqpLocator, myloc.loc );

      if ( districtMult.getValue() )
         QTHBundle.getStringProfile( eqpDistrict, location );
      else
         if ( otherExchange.getValue() && location.getValue().size() == 0 )
            QTHBundle.getStringProfile( eqpLocation, location );

      QTHBundle.getStringProfile( eqpStationQTH1, sqth1 );
      QTHBundle.getStringProfile( eqpStationQTH2, sqth2 );
      QTHBundle.getStringProfile( eqpASL, entASL );
   }

   if ( stationBundle.getSection() != noneBundle )
   {
      stationBundle.getStringProfile( espPower, power );
      stationBundle.getStringProfile( espTransmitter, entTx );
      stationBundle.getStringProfile( espReceiver, entRx );
      stationBundle.getStringProfile( espAntenna, entAnt );
      stationBundle.getStringProfile( espAGL, entAGL );
   }
}
/*
void LoggerContestLog::makeContact( bool timeNow, ContestContact * &lct )
{
   lct = new ContestContact( this, timeNow );
}
*/
int LoggerContestLog::readBlock( int bno )
{
   DWORD sres = ::SetFilePointer( GJVcontestFile, ( long ) ( bno * bsize ), 0, FILE_BEGIN );
   if ( sres == 0xFFFFFFFF )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "(read) seek failed!" );
   }

   DWORD rsize;
   bool res = ReadFile( GJVcontestFile, diskBuffer, bsize, &rsize, 0 );
   diskBuffer[ bsize ] = 0;
   if ( !res || rsize < bsize )
      MinosParameters::getMinosParameters() ->mshowMessage( "Short read" );

   return bsize;
}
int LoggerContestLog::writeBlock( HANDLE fd, int bno )
{
   // fd will not be contest_file if we are exporting a GJV file

   if ( fd == INVALID_HANDLE_VALUE )
      return bsize;		//tell lies

   int n = atoi( diskBuffer );
   if ( n != bno )
      MinosParameters::getMinosParameters() ->mshowMessage( "Invalid block number for write!!" );

   //lseek( fd, ( ( long ) bno ) * bsize, SEEK_SET );
   DWORD sres = ::SetFilePointer( fd, ( long ) ( bno * bsize ), 0, FILE_BEGIN );
   if ( sres == 0xFFFFFFFF )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "(write) seek failed!" );
   }
   DWORD ret = 0;
   bool res = WriteFile( fd, diskBuffer, bsize, &ret, 0 );
   if ( !res || ret != bsize )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
   }
   return ret;
}
void LoggerContestLog::closeFile( void )
{
   if ( GJVcontestFile != INVALID_HANDLE_VALUE )
   {
      CloseHandle( GJVcontestFile );
   }
   if ( minosContestFile != INVALID_HANDLE_VALUE )
   {
      CloseHandle( minosContestFile );
   }
   if ( logContestFile != INVALID_HANDLE_VALUE )
   {
      CloseHandle( logContestFile );
   }
   if ( adifContestFile != INVALID_HANDLE_VALUE )
   {
      CloseHandle( adifContestFile );
   }
   if ( ediContestFile != INVALID_HANDLE_VALUE )
   {
      CloseHandle( ediContestFile );
   }

   GJVcontestFile = INVALID_HANDLE_VALUE;
   minosContestFile = INVALID_HANDLE_VALUE;
   logContestFile = INVALID_HANDLE_VALUE;
   adifContestFile = INVALID_HANDLE_VALUE;
   ediContestFile = INVALID_HANDLE_VALUE;
}
DisplayContestContact *LoggerContestLog::addContact( int newctno, int extraFlags, bool saveNew, bool catchup )
{
   // add the contact number as an new empty contact, with disk block and log_seq

   bool timenow = true;
   if ( ( extraFlags & TO_BE_ENTERED ) || catchup )
      timenow = false;

   DisplayContestContact *lct;
   makeContact( timenow, ( BaseContact * ) lct );
   std::string temp = ( boost::format( "%3.3d" ) % newctno ).str();
   lct->serials.setValue( temp );
   lct->setLogSequence( nextBlock << 16 );
   nextBlock++;
   if ( newctno > maxSerial )
   {
      maxSerial = newctno;
   }
   lct->contactFlags.setValue( lct->contactFlags.getValue() | extraFlags );
   if ( saveNew )
   {
      lct->commonSave();		// make sure contact is correct
   }
   ctList.insert( lct );
   if ( saveNew )
   {
      commonSave( false );
   }

   return lct;
}
//==========================================================================
void LoggerContestLog::removeContact( DisplayContestContact *lct )
{
   ctList.erase(lct);
   delete lct;
}
//==========================================================================
bool LoggerContestLog::commonSave( bool newfile )
{
   if ( !isReadOnly() )
   {
      if ( GJVFile )
      {
         MinosParameters::getMinosParameters() ->mshowMessage( "GJV should have been opened read only!" );
         return false; // it better be read only!
      }
      else
         if ( minosFile )
         {
            return minosSaveFile( newfile );
         }
   }
   return false;
}
//==========================================================================
bool LoggerContestLog::minosSaveFile( bool newfile )
{
   MinosTestExport mt( this );
   stanzaCount += mt.exportAllDetails( minosContestFile, newfile );
   clearDirty();
   return true;
}
bool LoggerContestLog::minosSaveContestContact( const ContestContact *lct )
{
   MinosTestExport mt( this );
   stanzaCount += mt.exportQSO( minosContestFile, lct );
   return true;
}
//==========================================================================
bool LoggerContestLog::GJVsave( GJVParams &gp )
{
   const std::string nulc;

   // save the LoggerContestLog details to file_desc
   clearBuffer();
   buffpt = 0;

   strtobuf( "0" );    // block number for LoggerContestLog block
   strtobuf( GJVVERSION );
   strtobuf( band );
   strtobuf( name );
   strtobuf( mycall.fullCall );
   strtobuf( myloc.loc );
   strtobuf( location );

   opyn( otherExchange );
   opyn( false /*Country_mult && County_mult*/ );	//CC_mult
   opyn( locMult );

   std::string temp = ( boost::format( "%d" ) % gp.count ).str(); // leave off the dummy entry!
   strtobuf( temp );

   strtobuf( power );

   strtobuf( nulc );
   //   strtobuf( mainOpNow );
   strtobuf( nulc );
   //   strtobuf( secondOpNow );
   strtobuf( mode );
   opyn( true );						// RADIAL rings are DEAD
   opyn( false );                // was post entry
   opyn( scoreMode.getValue() == PPQSO );
   opyn( countryMult );
   opyn( false /*County_mult*/ );

   opyn( districtMult );

   opyn( powerWatts );
   opyn( false /*scoreMode == GSPECIAL*/ );
   opyn( allowLoc8 );

   opyn( !RSTField.getValue() );
   opyn( !serialField.getValue() );
   opyn( !locatorField.getValue() );
   opyn( !QTHField.getValue() );

   strtobuf(); // clear tail

   HANDLE fd = gp.fd;

   writeBlock( fd, 0 );

   // note that each contact will be saved as entered
   return true;
}
bool LoggerContestLog::GJVload( void )
{
   std::string temp;
   logCount = 0;

   // load the LoggerContestLog details from file_desc
   clearBuffer();
   readBlock( 0 );
   buffpt = 0;
   buftostr( temp );
   if ( atoi( temp.c_str() ) != 0 )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "Invalid block 0 in LoggerContestLog file" );
      return false;
   }

   buftostr( temp );
   if ( strnicmp( temp, GJVVERSION, VERLENGTH ) != 0 )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( String( "Invalid LoggerContestLog file format (" ) + temp.c_str() + ", " + GJVVERSION + " expected)" );
      return false;
   }

   buftostr( band );
   buftostr( name );
   buftostr( temp );
   mycall = callsign( strupr( temp ) );
   buftostr( myloc.loc );
   buftostr( location );

   otherExchange.setValue( inyn() );
   /*bool CC_mult =*/
   inyn();
   /*
   if ( CC_mult )
   {
      Country_mult = true;
      County_mult = true;
      //      District_mult = true;
   }
   */
   locMult.setValue( inyn() );

   buftostr( temp );
   logCount = atoi( temp.c_str() );

   buftostr( power );

   buftostr( temp );
   //   buftostr( mainOpNow );

   buftostr( temp );
   //   buftostr( secondOpNow );

   buftostr( mode );
   inyn();
   scoreMode.setValue( PPKM );			// don't take any notice of radial flag!

   inyn();        // was post entry
   if ( inyn() )
   {
      scoreMode.setValue( PPQSO );
   }
   countryMult.setValue( inyn() );
   if ( inyn() )
   {
      //      County_mult = true;
      //      District_mult = true;
   }
   districtMult.setValue( inyn() );

   powerWatts.setValue( inyn() );

   if ( inyn() )
   {
      //      scoreMode = GSPECIAL;
   }

   allowLoc8.setValue( inyn() );
   RSTField.setValue( !inyn() );
   serialField.setValue( !inyn() );
   locatorField.setValue( !inyn() );

   QTHField.setValue( !inyn() );

   return true;

}
bool LoggerContestLog::GJVloadContacts( void )
{
   ContestContact * rct = 0;

   nextBlock = 1;

   for ( int i = 0; i < logCount; i++ )
   {
      makeContact( false, ( BaseContact * ) rct );
      if ( rct->GJVload( ( int ) nextBlock ) )
      {
         nextBlock++;
         ctList.insert( rct );
         int maxct = atoi( rct->serials.getValue().c_str() );
         if ( maxct > maxSerial )
            maxSerial = maxct;

      }
   }
   return true;
}
bool LoggerContestLog::export_contest( HANDLE expfd, ExportType exptype )
{
   bool ret = false;
   commonSave( false );
   setDirty();                // We always need to export everything
   switch ( exptype )
   {
         //enum ExportType {EREG1TEST, EADIF, EG0GJV, EMINOS };

      case EG0GJV:
         ret = exportGJV( expfd );
         break;

      case EMINOS:
         ret = exportMinos( expfd );
         break;

      case EADIF:
         ret = exportADIF( expfd );
         break;

      case EKML:
         ret = exportKML( expfd );
         break;

      case EREG1TEST:
         ret = exportREG1TEST( expfd );
         break;

      case EPRINTFILE:
         ret = exportPrintFile(expfd);

      default:
         return false;
   }
   clearDirty();    // BUT don't leave it dirty!!
   return ret;
}
static bool uhNeeded = false;
static bool utNeeded = false;
void LoggerContestLog::procUnknown( BaseContact *cct, writer &wr )
{
   String lbuff;

   if ( cct->QSOValid
        && !( ( cct->cs.valRes == ERR_DUPCS )
              || ( cct->contactFlags.getValue() & NON_SCORING )
              || ( cct->contactScore.getValue() <= 0 )
            )
      )
   {

      // no district when required
      if ( countryMult.getValue() && cct->ctryMult == 0 )   	// invalid country
         lbuff = "Unknown Country  ";

      else
         if ( districtMult.getValue() && cct->ctryMult && cct->ctryMult->hasDistricts()     // continentals dont have counties
              && cct->districtMult == 0 && !( cct->contactFlags.getValue() & VALID_DISTRICT ) )   	// invalid country
         {
            lbuff = "Unknown District   ";
         }
         else
            return ;

      std::string sl;
      cct->getText( sl, this );

      lbuff += sl.c_str();

      if ( uhNeeded )
      {
         uhNeeded = false;
         utNeeded = true;
         wr.lwrite( "\r\n\r\n    Contacts with Unknown Country/District\r\n" );
      }
      wr.lwrite( lbuff.c_str() );
   }
}

bool LoggerContestLog::exportGJV( HANDLE fd )
{
   //	straight copy to disk

   int mind = 1;
   int maxd = maxSerial;
   if ( !enquireDialog(   /*Owner*/0, "Please give first serial to be dumped", mind ) )
      return false;
   if ( !enquireDialog(   /*Owner*/0, "Please give last serial to be dumped", maxd ) )
      return false;

   int mindump = std::min( mind, maxd );
   int maxdump = std::max( mind, maxd );

   // ????   if ( MessageBox( 0, "Do you wish to edit the file?", "Contest", MB_OKCANCEL ) != ID_CANCEL )
   //   if (cmOK != messageBox(mfOKCancel|mfConfirmation, "Dumping all contacts between serials %d and %d inclusive", mindump, maxdump))
   std::string temp = ( boost::format( "Dumping all contacts between serials %d and %d inclusive" ) % mindump % maxdump ).str();
   if ( !MinosParameters::getMinosParameters() ->yesNoMessage( 0, temp.c_str() ) )
      return false;

   GJVParams gp( fd );

   GJVsave( gp );

   bool inDump = false;

   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      ContestContact *lct = dynamic_cast<ContestContact *>( *i );
      // we need to test for "in dump"

      int serials = atoi( lct->serials.getValue().c_str() );
      // dump the contact, until serial seen

      if ( ( serials == mindump ) || ( mindump == 0 ) )
         inDump = true;

      if ( inDump && ( serials <= maxdump ) )
      {
         if ( !lct->GJVsave( gp ) )
            break;
         gp.count++;
      }

      if ( serials >= maxdump )
      {
         break;	// as all dumped
      }
   }

   GJVsave( gp );

   return true;
}
bool LoggerContestLog::exportADIF( HANDLE expfd )
{
   // ADIF format file entry
   // OP header
   // and EOH
   std::string header = "Exported by G0GJV VHF LoggerContestLog logging system\r\n\r\n";

   header += std::string( "From file " ) + cfileName + "\r\n\r\n";

   header += "<EOH>\r\n";
   DWORD ret = 0;
   bool res = WriteFile( expfd, header.c_str(), header.size(), &ret, 0 );
   if ( !res || ret != header.size() )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
   }

   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      ContestContact *lct = dynamic_cast<ContestContact *>( *i );
      std::string l = lct ->getADIFLine();
      if ( l.size() )
      {
         //         ::write( expfd, l.c_str(), l.size() );
         res = WriteFile( expfd, l.c_str(), l.size(), &ret, 0 );
         if ( !res || ret != l.size() )
         {
            MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
         }
         char *EOR = "<EOR>\r\n";
         res = WriteFile( expfd, EOR, strlen( EOR ), &ret, 0 );
         if ( !res || ret != strlen( EOR ) )
         {
            MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
         }
      }
   }

   return true;
}
bool LoggerContestLog::exportREG1TEST( HANDLE expfd )
{
   // First test validity. Reg1test dictates in particular

   // band

   std::string cb = trim(band.getValue());
   BandList &blist = BandList::getBandList();
   BandInfo bi;
   bool bandOK = blist.findBand(cb, bi);
   if (bandOK)
   {
      cb = bi.reg1test;
   }

   if ( !bandOK )
   {
      // put up a band chooser dialog
      std::auto_ptr<TMinosBandChooser> mshowMessage( new TMinosBandChooser( LogContainer ) );

      BandList &blist = BandList::getBandList();
      for (unsigned int i = 0; i < blist.bandList.size(); i++)
      {
         if (blist.bandList[ i ].reg1test.size())  // only put up real reg1test options
         {
            mshowMessage->BandCombo->Items->Add( blist.bandList[ i ].reg1test.c_str() );
         }
      }

      mshowMessage->BandCombo->ItemIndex = 0;

      std::string capt = ( boost::format( "The band description chosen (%s) is invalid for Reg1Test"
                                          " (.EDI) entry. Please choose a valid band description." )
                           % band.getValue() ).str();
      mshowMessage->ScreedLabel->Caption = capt.c_str();

      mshowMessage->ShowModal();

      band.setValue( mshowMessage->BandCombo->Text.c_str() );
   }


   reg1test * rtest = new reg1test( this );
   int rep = rtest->exportTest( expfd );
   delete rtest;
   return rep;
}
bool LoggerContestLog::exportPrintFile( HANDLE expfd )
{
   PrintFile * rtest = new PrintFile( this );
   int rep = rtest->exportTest( expfd );
   delete rtest;
   return rep;
}
bool LoggerContestLog::exportMinos( HANDLE expfd )
{
   int mind = 1;
   int maxd = maxSerial;
   if ( !enquireDialog(   /*Owner*/0, "Please give first serial to be dumped", mind ) )
      return false;
   if ( !enquireDialog(   /*Owner*/0, "Please give last serial to be dumped", maxd ) )
      return false;

   int mindump = std::min( mind, maxd );
   int maxdump = std::max( mind, maxd );

   // ????   if ( MessageBox( 0, "Do you wish to edit the file?", "Contest", MB_OKCANCEL ) != ID_CANCEL )
   //   if (cmOK != messageBox(mfOKCancel|mfConfirmation, "Dumping all contacts between serials %d and %d inclusive", mindump, maxdump))
   std::string temp = ( boost::format( "Dumping all contacts between serials %d and %d inclusive" ) % mindump % maxdump ).str();
   if ( !MinosParameters::getMinosParameters() ->yesNoMessage( 0, temp.c_str() ) )
      return false;

   MinosTestExport * mtest = new MinosTestExport( this );
   int rep = mtest->exportTest( expfd, mindump, maxdump );
   delete mtest;
   return ( rep > 0 );
}
static void ccor( double &y )
{
   while ( ( y < -180.0 ) || ( y > 180.0 ) )
      y += ( y < 0.0 ) ? 360.0 : -360.0;
}
static double raddeg( double x )
{
   double y = x * 180.0 / pi;

   ccor( y );
   return ( y );

}
static AnsiString kmloutput( Location *outgrid )
{
   AnsiString kml;
   kml.printf(
      "%3.8f,"
      "%3.8f",
      raddeg( outgrid->easting ),
      raddeg( outgrid->northing )
   );
   return kml;

}
bool LoggerContestLog::exportKML( HANDLE expfd )
{
   typedef std::map <std::string, ContestContact *> cmap; // map by call
   typedef std::map <std::string, cmap> smap;       // map by prefix
   smap countries;

   for ( LogIterator i = ctList.begin(); i != ctList.end(); i++ )
   {
      ContestContact *ct = dynamic_cast<ContestContact *>( *i );
      if ( ct->ctryMult )
      {
         ( countries[ ct->ctryMult->basePrefix ] ) [ ct->cs.fullCall.getValue() ] = ct;
      }
      else
         if ( ct->QSOValid )
         {
            ( countries[ "unknown" ] ) [ ct->cs.fullCall.getValue() ] = ct;
         }
         else
         {
            // contact is invalid - don't display it
         }
   }

   TStringList *kml = new TStringList;

   kml->Add( "<kml xmlns=\"http://earth.google.com/kml/2.0\">" );
   kml->Add( "<Document><visibility>0</visibility><open>1</open>" );
   kml->Add( ( "<Folder><name><![CDATA[" + name.getValue() + " " + mycall.fullCall.getValue() + "]]></name><visibility>0</visibility><open>1</open>" ).c_str() );

   for ( smap::iterator s = countries.begin(); s != countries.end(); s++ )
   {
      kml->Add( ( "<Folder><name><![CDATA[" + ( *s ).first + "]]></name><open>0</open><visibility>0</visibility>" ).c_str() );
      for ( cmap::iterator e = ( ( *s ).second ).begin(); e != ( ( *s ).second ).end(); e++ )
      {
         Location l1;
         Location l2;
         ContestContact *ct = ( *e ).second;


         char inputbuff[ 100 ];
         strcpy( inputbuff, ct->loc.loc.getValue().c_str() );
         l1.gridstyle = LOC;
         l1.datastring = inputbuff;
         l1.datalength = ct->loc.loc.getValue().length() + 1;
         l1.dataoffset = 0;

         char outputbuff[ 100 ];
         l2.datastring = outputbuff;
         l2.gridstyle = GEO;
         l2.datalength = 40;
         l2.dataoffset = 0;

         if ( transform( &l1, &l2 ) == GRIDOK )
         {
            kml->Add( "<Placemark><visibility>0</visibility>" );
            kml->Add( ( "<description><![CDATA[" + ct->cs.fullCall.getValue() + " " + ct->loc.loc.getValue() + "]]></description>" ).c_str() );
            kml->Add( ( "<name><![CDATA[" + ct->cs.fullCall.getValue() + "]]></name>" ).c_str() );
            kml->Add( "<Point><coordinates>" + kmloutput( &l2 ) + ",0</coordinates></Point>"  );
            kml->Add( "</Placemark>" );
         }
      }
      kml->Add( "</Folder>" );
   }

   kml->Add( "</Folder>" );
   kml->Add( "</Document>" );
   kml->Add( "</kml>" );


   // and output the stringlist
   for ( int i = 0; i < kml->Count; i++ )
   {
      //         ::write( expfd, l.c_str(), l.size() );
      kml->Strings[ i ] = kml->Strings[ i ] + "\r\n";

      DWORD ret = 0;
      bool res = WriteFile( expfd, kml->Strings[ i ].c_str(), kml->Strings[ i ].Length(), &ret, 0 );
      if ( !res || ( int ) ret != kml->Strings[ i ].Length() )
      {
         MinosParameters::getMinosParameters() ->mshowMessage( "bad reply from write!" );
      }
   }
   return true;

}
//============================================================

bool LoggerContestLog::importLOG( HANDLE hLogFile )
{
   THandleStream * hs = new THandleStream( ( int ) hLogFile );
   TStringList *ls = new TStringList();
   ls->LoadFromStream( hs );

   TEMPBUFF( temp, 100 );
   // Import from LOG format
   // Needs modification for "new" log format

   locValid = false;
   //   save();
   // read remainder of lines; write each line as we go

   long next_block = 1;

   bool started = false;

   DisplayContestContact *ct = 0;
   int lineNo = -1;
   while ( ++lineNo < ls->Count )
   {
      // skip new format header information, until we can cope
      // properly with it
      std::string stemp = trimr( ls->Strings[ lineNo ].c_str() );
      if ( stemp.length() == 0 || ( !started && !isdigit( ls->Strings[ lineNo ][ 1 ] ) ) )
      {
         // GJV.LOG

         /*
         Contest Title                        : VHFNFD
         Band in MHz                          : 432
         Section Entered                      : Multi Operator
         Name of Entrant/Club/Contest Group   : Bracknell Amateur Radio Club
         Band in MHz                          : %band%
         Section Entered                      : %section%
         Callsign Used                        : %call%
         QRH Locator Sent                     : %locator%
         Location as sent(District or QTH)    : %QTH%
         Brief Details of Transmitter Used    : %Transmitter%
         Brief Details of Receiver Used       : %Receiver%
         Power Output (Watts)                 : %power%
         Antenna Type                         : %Antenna%
         Antenna Height AGL(metres)           : %AGL%
         QTH Height ASL(metres)               : %ASL%

         Difficult ones:
         Name and Callsign of all Operators   :
         G0GJV  G4AUC  G0FCT  G0HVS
         Name, Callsign, Telephone Number, Address for Correspondence :
                                     : M. J. Goodey
                                     : G0GJV
                                     : 62, Rose Hill
                                     : Binfield
                                     : Berkshire
                                     : RG42 5LG
                               Phone : 01344 428614
                               Email : mjgoodey@compuserve.com

         */
         unsigned int spos = stemp.find( ":" );
         if ( spos != std::string::npos )
         {
            std::string text = trim( stemp.substr( spos + 1, stemp.size() ) );

            if ( strupr( stemp ).find( "CONTEST TITLE" ) == 0 )
            {
               name.setValue( text );
            }
            else
               if ( strupr( stemp ).find( "BAND IN MHZ" ) == 0 )
               {
                  band.setValue( text );
               }
               else
                  if ( strupr( stemp ).find( "SECTION ENTERED" ) == 0 )
                  {
                     entSect.setValue( text );
                  }
                  else
                     if ( strupr( stemp ).find( "NAME OF ENTRANT/CLUB/CONTEST GROUP" ) == 0 )
                     {
                        entrant.setValue( text );
                     }
                     else
                        if ( strupr( stemp ).find( "CALLSIGN USED" ) == 0 )
                        {
                           // we need to tail clip the callsign - or we end up with things like
                           // M1DUD/QRP        CLASS : SO  /   50MHZ FIXED
                           unsigned int spos = text.find( " " );
                           if ( spos != std::string::npos )
                           {
                              text = trim( text.substr( 0, spos ) );
                           }
                           mycall.fullCall.setValue( strupr( text ) );
                           mycall.valRes = CS_NOT_VALIDATED;
                           mycall.validate();

                        }
                        else
                           if ( strupr( stemp ).find( "QRH LOCATOR SENT" ) == 0 ||
                                strupr( stemp ).find( "QTH LOCATOR SENT" ) == 0 )
                           {
                              // yes, contestx DOES say QRH!
                              myloc.loc.setValue( strupr( text ) );
                              validateLoc();
                           }
                           else
                              if ( strupr( stemp ).find( "POWER OUTPUT" ) == 0 )
                              {
                                 power.setValue( text );
                              }
                              else
                                 if ( strupr( stemp ).find( "LOCATION AS SENT" ) == 0 )
                                 {
                                    location.setValue( text );
                                 }
                                 else
                                    if ( strupr( stemp ).find( "ANTENNA TYPE" ) == 0 )
                                    {
                                       entAnt.setValue( text );
                                    }
                                    else
                                       if ( strupr( stemp ).find( "ANTENNA HEIGHT" ) == 0 )
                                       {
                                          entAGL.setValue( text );
                                       }
                                       else
                                          if ( strupr( stemp ).find( "QTH HEIGHT" ) == 0 )
                                          {
                                             entASL.setValue( text );
                                          }
                                          else
                                             if ( strupr( stemp ).find( "BRIEF DETAILS OF TRANSMITTER USED" ) == 0 )
                                             {
                                                entTx.setValue( text );
                                             }
                                             else
                                                if ( strupr( stemp ).find( "BRIEF DETAILS OF RECEIVER USED" ) == 0 )
                                                {
                                                   entRx.setValue( text );
                                                }
         }
         continue;
      }
      started = true;

      makeContact( false, ( BaseContact * ) ct );
      ct->setLogSequence( 0 );

      stemp += std::string( 200, ' ' );   // make sure there is plenty more...
      const char *lbuff = stemp.c_str();
      // parse contact line in

      strcpysp( temp, &lbuff[ 0 ], 6 );
      ct->time.setDate( temp, DTGLOG );
      strcpysp( temp, &lbuff[ 7 ], 4 );
      ct->time.setTime( temp, DTGLOG );
      strcpysp( temp, &lbuff[ 21 ], 15 );
      ct->cs = callsign( strupr( temp ) );
      ct->cs.valRes = CS_NOT_VALIDATED;
      strcpysp( temp, &lbuff[ 37 ], 3 );
      ct->reps.setValue( temp );
      strcpysp( temp, &lbuff[ 41 ], 4 );
      ct->serials.setValue( temp );

      int maxct = atoi( ct->serials.getValue().c_str() );
      if ( maxct > maxSerial )
         maxSerial = maxct;

      strcpysp( temp, &lbuff[ 46 ], 3 );
      ct->repr.setValue( temp );
      strcpysp( temp, &lbuff[ 51 ], 4 );
      ct->serialr.setValue( temp );

      // here is the score field
      strcpysp( temp, &lbuff[ 59 ], 5 );
      if ( atoi( temp ) == 0 )
         ct->contactFlags.setValue( NON_SCORING );

      strcpysp( temp, &lbuff[ 65 ], 6 );
      ct->op1.setValue( temp );

      strcpysp( temp, &lbuff[ 72 ], 6 );
      ct->loc.loc.setValue( temp );
      ct->loc.valRes = LOC_NOT_VALIDATED;

      //ct->comments = "";

      TEMPBUFF( extra, EXTRALENGTH + 1 );
      TEMPBUFF( comments, COMMENTLENGTH + 1 );

      strcpysp( extra, &lbuff[ 81 ], 2 );          // 81 is district code
      if ( extra[ 0 ] && extra[ 1 ] && ( extra[ 0 ] != ' ' && extra[ 1 ] != ' ' ) )
      {
         // we always attempt to import the district mult field
         strcpysp( comments, &lbuff[ 93 ], COMMENTLENGTH );
         ct->comments.setValue( comments );

      }
      else
      {
         strcpysp( extra, &lbuff[ 93 ], EXTRALENGTH );
      }
      ct->extraText.setValue( extra );

      //	   strcpysp(ct->power, power, 7);
      //      ct->PowerWatts = PowerWatts;
      // save contact

      // we have to have logSequence set before we insert - or it will cause
      // duplicates

      next_block++ ;
      ct->setLogSequence( next_block << 16 );

      ctList.insert( ct );
   }
   delete ls;
   delete hs;
   return true;
}
//====================================================================
bool LoggerContestLog::importAdif( HANDLE adifContestFile )
{
   return ADIFImport::doImportADIFLog( this, adifContestFile );
}
//====================================================================
bool LoggerContestLog::importReg1Test( HANDLE r1ContestFile )
{
   // in AdjSQL we do this as a semi-colon separated format, not as something special
   return reg1test::doImportReg1test( this, r1ContestFile );
}
//====================================================================
void LoggerContestLog::processMinosStanza( const std::string &methodName, MinosTestImport * const mt )
{
   BaseContestLog::processMinosStanza( methodName, mt );

   if ( methodName == "MinosLogContest" )
   {
      mt->getStructArgMemberValue( "section", entSect );
      mt->getStructArgMemberValue( "sectionList", sectionList );
   }
   else
      if ( methodName == "MinosLogMode" )
      {}
      else
         if ( methodName == "MinosLogQTH" )
         {
            mt->getStructArgMemberValue( "stationQTH1", sqth1 );
            mt->getStructArgMemberValue( "stationQTH2", sqth2 );
            mt->getStructArgMemberValue( "ASL", entASL );
         }
         else
            if ( methodName == "MinosLogEntry" )
            {
               mt->getStructArgMemberValue( "entrant", entrant );
               mt->getStructArgMemberValue( "myName", entName );
               mt->getStructArgMemberValue( "myCall", entCall );
               mt->getStructArgMemberValue( "myAddress1", entAddr1 );
               mt->getStructArgMemberValue( "myAddress2", entAddr2 );
               mt->getStructArgMemberValue( "myCity", entCity );
               mt->getStructArgMemberValue( "myCountry", entCountry );
               mt->getStructArgMemberValue( "myPostCode", entPostCode );
               mt->getStructArgMemberValue( "myPhone", entPhone );
               mt->getStructArgMemberValue( "myEmail", entEMail );
            }
            else
               if ( methodName == "MinosLogStation" )
               {
                  mt->getStructArgMemberValue( "transmitter", entTx );
                  mt->getStructArgMemberValue( "receiver", entRx );
                  mt->getStructArgMemberValue( "antenna", entAnt );
                  mt->getStructArgMemberValue( "AGL", entAGL );
               }
               else
                  if ( methodName == "MinosLogOperators" )
                  {
                     mt->getStructArgMemberValue( "ops1", ops1 );
                     mt->getStructArgMemberValue( "ops2", ops2 );
                     mt->getStructArgMemberValue( "currentOp1", currentOp1 );
                     oplist.insert(currentOp1.getValue());
                     mt->getStructArgMemberValue( "currentOp2", currentOp2 );
                     oplist.insert(currentOp2.getValue());
                  }
                  else
                     if ( methodName == "MinosLogCurrent" )
                     {
                        mt->getStructArgMemberValue( "condx1", entCondx1 );
                        mt->getStructArgMemberValue( "condx2", entCondx2 );
                        mt->getStructArgMemberValue( "condx3", entCondx3 );
                        mt->getStructArgMemberValue( "condx4", entCondx4 );
                     }
                     else
                        if ( methodName == "MinosLogBundles" )
                        {
                           mt->getStructArgMemberValue( "VHFContestName", VHFContestName );
                           mt->getStructArgMemberValue( "entryBundle", entryBundleName );
                           mt->getStructArgMemberValue( "QTHBundle", QTHBundleName );
                           mt->getStructArgMemberValue( "stationBundle", stationBundleName );

                        }
                        else
                           if ( methodName == "MinosLogComment" )
                           {
                              // should have been dealt with in BaseContest
                           }
                           else
                              if ( methodName == "MinosLogQSO" )
                              {
                                 // should have been dealt with in BaseContest
                              }
}
//====================================================================
void LoggerContestLog::setStanza( unsigned int stanza, int stanzaStart )
{
   StanzaPos s;
   s.stanza = stanza;
   s.stanzaStart = stanzaStart;
   stanzaLocations.push_back( s );
}
//====================================================================
bool LoggerContestLog::getStanza( unsigned int stanza, std::string &stanzaData )
{
   if ( stanza - 1 >= stanzaLocations.size() )
   {
      return false;
   }
   StanzaPos *s = &stanzaLocations[ stanza - 1 ];
   if ( s->stanza != stanza )
   {
      return false;
   }
   // read the stanza from the log file - which we will have to open for read

   HANDLE contestFile = CreateFile( cfileName.c_str(),
                                    GENERIC_READ,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    0,                  // security
                                    OPEN_EXISTING,
                                    0,
                                    0 );               // template handle
   if ( contestFile == INVALID_HANDLE_VALUE )
   {
      std::string lerr = lastError();
      std::string emess = "Failed to open LoggerContestLog file for monitoring" + cfileName + " : " + lerr;
      MinosParameters::getMinosParameters() ->mshowMessage( emess.c_str() );
      return false;
   }
   DWORD fpos;
   fpos = ::SetFilePointer( contestFile, s->stanzaStart, 0, FILE_BEGIN );
   if ( fpos == 0xFFFFFFFF )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( "(write) seek failed!" );
   }
   DWORD chRead = 0;
   char *buffer = new char [ 4096 + 1 ];      // it sounds a lot, but without parsing it...
   bool ReadOK = ReadFile ( contestFile, buffer, 4096, &chRead, 0 );
   if ( ReadOK )
   {
      if ( chRead > 0 )
      {
         buffer[ chRead ] = 0;
         stanzaData = buffer;
         unsigned int epos = stanzaData.find( "</iq>" );     // trim the excess - if there is any
         if ( epos != std::string::npos )
         {
            stanzaData = stanzaData.substr( 0, epos + 5 );
         }
      }
      else
         ReadOK = false;
   }
   delete [] buffer;
   CloseHandle( contestFile );
   return ReadOK;
}
//====================================================================

