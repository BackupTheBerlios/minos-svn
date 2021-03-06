/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop
#include "LogEvents.h"
#include "LoggerContest.h"

#include "gridhint.h"
#include "LogFrame.h"
#include "LogMain.h"

#include "GJVThreads.h"
#include "ConfigDM.h"
#include "GuardianThread.h"
#include "ConfigElementFrame.h"
#include "ConfigMainFrame.h"
#include "LoggerAbout.h"

#include "ListDetails.h"
#include "ContestDetails.h"
#include "ClockDialog.h"
#include "MultDisp.h"
#include "loccalc.h"
#include "SendRPCDM.h"
#include "StatsDisp.h"
#include "MinosTestImport.h"
#include "TManageListsDlg.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LogFrame"
#pragma link "adpMRU"
#pragma resource "*.dfm"
TLogContainer *LogContainer = 0;
//---------------------------------------------------------------------------
__fastcall TLogContainer::TLogContainer( TComponent* Owner )
      : TForm( Owner ), GridHintWindow( 0 ), oldX( 0 ), oldY( 0 ),
      syncCaption( false ), syncMode( false ), saveResize( false )
{
   enableTrace( ".\\TraceLog\\MinosLogger_" );
   GridHintWindow = new TGridHint( this );
   GridHintWindow->SetHintControl( ContestPageControl );
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::FormShow( TObject */*Sender*/ )
{
   StartupTimer->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::StartupTimerTimer( TObject */*Sender*/ )
{
   StartupTimer->Enabled = false;
   if ( TContestApp::getContestApp() )
   {
      //      ScaleBy( TContestApp::getContestApp() ->sysfont->Size, Font->Size );
      int L = Left, T = Top, W = Width, H = Height;
      TContestApp::getContestApp() ->displayBundle.getIntProfile( edpLeft, L );
      TContestApp::getContestApp() ->displayBundle.getIntProfile( edpTop, T );
      TContestApp::getContestApp() ->displayBundle.getIntProfile( edpWidth, W );
      TContestApp::getContestApp() ->displayBundle.getIntProfile( edpHeight, H );
      SetBounds( L, T, W, H );
      Repaint();
      saveResize = true;
      //      Font->Assign( TContestApp::getContestApp() ->sysfont );
      if ( TAboutBox::ShowAboutBox( this, true ) == false )
      {
         Close();
         return ;
      }

      SendDM = new TSendDM( this );
      if ( contestAppLoadFiles() )
      {
         // here need to pre-open the contest list
         char * conarg = 0;
         if ( ParamCount() >= 2 )
				conarg = ParamStr( 1 ).t_str();
         preloadFiles( conarg );
         enableActions();
      }
   }
   else
      Close();
}
//---------------------------------------------------------------------------
void TLogContainer::preloadFiles( char *conarg )
{
   // and here we want to pre-load lists and contests from the INI file
   // based on what was last open

#warning getProfileEntries gets the Current entry as well... not good
   std::vector<std::string> slotlst = TContestApp::getContestApp() ->preloadBundle.getProfileEntries();
   std::vector<std::string> pathlst;
   for ( std::vector<std::string>::iterator i = slotlst.begin(); i != slotlst.end(); i++ )
   {
      std::string ent;
      TContestApp::getContestApp() ->preloadBundle.getStringProfile( ( *i ).c_str(), ent, "" );
      pathlst.push_back( ent );
   }
   int curSlot = 0;
   TContestApp::getContestApp() ->preloadBundle.getIntProfile( eppCurrent, curSlot );
   for ( unsigned int i = 0; i < slotlst.size(); i++ )
   {
      std::string s = slotlst[ i ].substr( 0, 4 );
      if ( s == "List" )
      {
         int slotno = atoi( slotlst[ i ].substr( 4, 2 ).c_str() ) - 1; // even a 2 char number is a BIT excessive
         if ( slotno >= 0 )
         {
            addListSlot( 0, pathlst[ i ], slotno );
         }
      }
      else
      {
         int slotno = atoi( slotlst[ i ].c_str() ) - 1;
         if ( slotno >= 0 )
         {
            addSlot( 0, pathlst[ i ], false, false, slotno );
         }
      }
   }
   BaseContestLog *ct = 0;

   if ( TContestApp::getContestApp() ->getContestSlotCount() )
   {
      if ( curSlot > 0 )
      {
         ct = TContestApp::getContestApp() ->contestSlotList[ curSlot - 1 ] ->slot;
      }
   }

   TContestApp::getContestApp() ->writeContestList();	// to clear the unopened and changed ones

   if ( conarg )
   {
      // open the "argument" one last - which will make it current
      ct = addSlot( 0, conarg, false, false, -1 );
      TContestApp::getContestApp() ->writeContestList();	// or this one will not get included
   }

   if ( ct )
   {
      selectContest( ct, 0 );
   }
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::FormClose( TObject */*Sender*/,
      TCloseAction &/*Action*/ )
{
   trace( "Logger close initiated" );
   saveResize = false;
   delete TMConfigDM::getConfigDM( 0 );
   delete SendDM;
   SendDM = 0;
   closeContestApp();

}
//---------------------------------------------------------------------------
BaseContestLog * TLogContainer::addSlot( TContestEntryDetails *ced, const std::string &fname, bool newfile, bool read_only, int slotno )
{
   static int namegen = 0;
   // openFile ends up calling ContestLog::initialise which then
   // calls TContestApp::insertContest

   LoggerContestLog * contest = TContestApp::getContestApp() ->openFile( fname, newfile, read_only, slotno );

   if ( contest )
   {
      bool show = false;
      if ( ced )
      {

         ced->setDetails( contest );

         {
            if ( ced->ShowModal() == mrOk )
            {
               contest->commonSave( false );
               contest->startScan();
               show = true;
            }
            else
            {
               ContestMRU->AddItem( fname.c_str() );
               TContestApp::getContestApp() ->closeFile( contest );
               contest = 0;
            }
         }
      }
      else
      {
         contest->startScan();
         show = true;
      }

      if ( show )
      {
         TContestApp::getContestApp() ->setCurrentContest( contest );
         TTabSheet *t = new TTabSheet( this );
         String baseFname = ExtractFileName( contest->cfileName.c_str() );
         t->Caption = baseFname;

         TSingleLogFrame *f = new TSingleLogFrame( this, contest );
         f->Name = String( "LogFrame" ) + namegen++;
         f->Parent = t;
         f->Align = alClient;
         f->ParentFont = true;
         f->TabStop = false;

         t->PageControl = ContestPageControl;
         ContestPageControl->ActivePage = t;
         ContestPageControlChange( this );
         f->showQSOs();

         if ( contest->needsExport() )      // imported from an alien format (e.g. .log)
         {
				std::string expName = f->makeEntry( true );
				if ( expName.size() )
            {
               closeSlot( true );
					addSlot( 0, expName, false, false, -1 );
            }
         }
         ContestMRU->RemoveItem( fname.c_str() );
      }
   }
   TContestApp::getContestApp() ->writeContestList();
   enableActions();
   return contest;
}
ContactList * TLogContainer::addListSlot( TContactListDetails *ced, const std::string &fname, int slotno )
{
   // Is this the correct return type, or do we have an even more basic one? Or even a useful interface...

   // openFile ends up calling ContactList::initialise which then
   // calls TContestApp::insertList

   ContactList * list = TContestApp::getContestApp() ->openListFile( fname, slotno );
   if ( list && ced )
   {

      ced->setDetails( list );
      if ( ced->ShowModal() == mrOk )
      {
         ced->getDetails( list );
      }
      else
      {
         TContestApp::getContestApp() ->closeListFile( list );
         list = 0;
      }
   }

   TContestApp::getContestApp() ->writeContestList();
   enableActions();
   return list;
}
//---------------------------------------------------------------------------
TSingleLogFrame *TLogContainer::findCurrentLogFrame()
{
   // we need to find the embedded frame...
   if ( !ContestPageControl->ActivePage )
      return 0;
   int cc = ContestPageControl->ActivePage->ControlCount;
   for ( int i = 0; i < cc; i++ )
   {
      if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( ContestPageControl->ActivePage->Controls[ i ] ) )
      {
         return f;
      }
   }
   return 0;
}
//---------------------------------------------------------------------------
void TLogContainer::setError( int err )
{
   if ( LogContainer )
   {
      TSingleLogFrame * f = LogContainer->findCurrentLogFrame();
      if ( f )
         f->GJVQSOLogFrame->lgTraceerr( err );
   }
}
void TLogContainer::showErrorList( ErrorList &errs )
{
   if ( LogContainer )
   {
      TSingleLogFrame * f = LogContainer->findCurrentLogFrame();
      f->showErrorList( errs );
   }
}
/*static*/ void TLogContainer::showContestScore( const std::string &score )
{
   if ( LogContainer )
   {
      LogContainer->StatusBar1->Panels->Items[ 0 ] ->Text = score.c_str();
   }
}
//---------------------------------------------------------------------------
void TLogContainer::closeSlot( bool addToMRU )
{
   TTabSheet * t = ContestPageControl->ActivePage;
   if ( t )
   {
      TSingleLogFrame * f = findCurrentLogFrame();
      if ( addToMRU )
      {
         BaseContestLog * contest = f->getContest();
         String curPath = contest->cfileName.c_str();
         ContestMRU->AddItem( curPath );
      }
      f->closeContest();    // which should close the contest
      t->PageControl = 0;
      delete t;
      ContestPageControlChange( this );
      enableActions();
   }
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::FileCloseAction1Execute( TObject */*Sender*/ )
{
   TWaitCursor fred;
   closeSlot( true );
}

//---------------------------------------------------------------------------

void __fastcall TLogContainer::CloseAllActionExecute(TObject */*Sender*/)
{
   TWaitCursor fred;
   while ( ContestPageControl->PageCount)
   {
      TTabSheet *ctab = ContestPageControl->Pages[ ContestPageControl->PageCount - 1 ];
      int cc = ctab->ControlCount;

      for ( int i = 0; i < cc; i++ )
      {
         if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( ctab->Controls[ i ] ) )
         {
            BaseContestLog * contest = f->getContest();
            String curPath = contest->cfileName.c_str();
            ContestMRU->AddItem( curPath );

            f->closeContest();    // which should close the contest
            ctab->PageControl = 0;
            delete ctab;
         }
      }
   }
   ContestPageControlChange( this );
   enableActions();
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::CloseAllButActionExecute(TObject */*Sender*/)
{
   TWaitCursor fred;
   TTabSheet * thisContest = ContestPageControl->ActivePage;
   while ( ContestPageControl->PageCount > 1)
   {
      TTabSheet *ctab = ContestPageControl->Pages[ ContestPageControl->PageCount - 1 ];
      if (ctab == thisContest)
      {
         ctab = ContestPageControl->Pages[ ContestPageControl->PageCount - 2 ];
      }
      int cc = ctab->ControlCount;

      for ( int i = 0; i < cc; i++ )
      {
         if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( ctab->Controls[ i ] ) )
         {
            BaseContestLog * contest = f->getContest();
            String curPath = contest->cfileName.c_str();
            ContestMRU->AddItem( curPath );

            f->closeContest();    // which should close the contest
            ctab->PageControl = 0;
            delete ctab;
         }
      }
   }
   ContestPageControlChange( this );
   enableActions();
}
//---------------------------------------------------------------------------

void TLogContainer::enableActions()
{
   TSingleLogFrame * lf = findCurrentLogFrame();
   bool f = ( lf != 0 );

   LoggerAction->Enabled = true;
   LocCalcAction->Enabled = true;
   FileNewAction->Enabled = true;
   HelpAboutAction->Enabled = true;
   NextContactDetailsOnLeftAction->Enabled = true;

   FileCloseAction1->Enabled = f;
   CloseAllAction->Enabled = f;
   CloseAllButAction->Enabled = f;
   ContestDetailsAction->Enabled = f;
   GoToSerialAction->Enabled = f;
   MakeEntryAction->Enabled = f;
   SetTimeNowAction->Enabled = f;

   ManageListsAction->Enabled = ( TContestApp::getContestApp() ->getOccupiedListSlotCount() > 0 );

   NextContactDetailsOnLeftAction->Checked = isNextContactDetailsOnLeft();

   if ( ContestPageControl->ActivePage )
   {
      int tno = ContestPageControl->ActivePage->PageIndex;
      ShiftTabLeftAction->Enabled = ( tno > 0 );
      ShiftTabRightAction->Enabled = ( tno < ContestPageControl->PageCount - 1 );
   }
   else
   {
      ShiftTabLeftAction->Enabled = false;
      ShiftTabRightAction->Enabled = false;
   }

}
//---------------------------------------------------------------------------
bool TLogContainer::isNextContactDetailsOnLeft()
{
   bool ncdol;
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpNextContactDetailsOnLeft, ncdol );
   return ncdol;
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestPageControlChange( TObject */*Sender*/ )
{
   TSingleLogFrame * f = findCurrentLogFrame();
   if ( f )
   {
      BaseContestLog * ct = f->getContest();
      TContestApp::getContestApp() ->setCurrentContest( ct );

      if ( f->logColumnsChanged )
         f->showQSOs();

      f->OnShowTimer->Enabled = true;
      f->GJVQSOLogFrame->CallsignEdit->SetFocus();
   }
   enableActions();
   Repaint();     // make sure the trees get repainted
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::HelpAboutActionExecute( TObject */*Sender*/ )
{
   TAboutBox::ShowAboutBox( this, false );
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::FileOpen1BeforeExecute( TObject */*Sender*/ )
{
   FileOpen1->Dialog->InitialDir = getDefaultDirectory( false );
   FileOpen1->Dialog->DefaultExt = "GJV";
   FileOpen1->Dialog->Filter = "Minos contest files (*.minos)|*.MINOS|"
                               "Reg1Test Files (*.edi)|*.EDI|"
                               "GJV contest files (*.gjv)|*.GJV|"
                               "RSGB Log Files (*.log)|*.LOG|"
                               "ADIF Files (*.adi)|*.ADI|"
                               "All Files (*.*)|*.*" ;
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::FileOpen1Accept( TObject */*Sender*/ )
{
   TWaitCursor fred;
   std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );
	addSlot( pced.get(), FileOpen1->Dialog->FileName.t_str(), false, false, -1 );   // not automatically read only
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::FileOpen1Cancel( TObject */*Sender*/ )
{
   TContestApp::getContestApp() ->writeContestList();
}

void __fastcall TLogContainer::ListOpen1BeforeExecute( TObject */*Sender*/ )
{
   ListOpen1->Dialog->InitialDir = getDefaultDirectory( true );
   ListOpen1->Dialog->DefaultExt = "CSL";
   ListOpen1->Dialog->Filter = "Contact list files (*.csl)|*.CSL|"
                               "All Files (*.*)|*.*" ;
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ListOpen1Accept( TObject */*Sender*/ )
{
   TWaitCursor fred;
   std::auto_ptr <TContactListDetails> pced( new TContactListDetails( this ) );
	addListSlot( pced.get(), ListOpen1->Dialog->FileName.t_str(), -1 );
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ListOpen1Cancel( TObject */*Sender*/ )
{
   TContestApp::getContestApp() ->writeContestList();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String TLogContainer::getDefaultDirectory( bool IsList )
{
   std::string temp;
   if ( IsList )
      TContestApp::getContestApp() ->loggerBundle.getStringProfile( elpListDirectory, temp );
   else
      TContestApp::getContestApp() ->loggerBundle.getStringProfile( elpLogDirectory, temp );
   String fileName = temp.c_str();

   if ( fileName.Length() && DirectoryExists( fileName ) )
   {
      // need to check possible validity of directory string
      if ( fileName[ fileName.Length() ] != '\\' )
         fileName += "\\";
      if ( fileName[ fileName.Length() ] == '\\' )
      {
         fileName = fileName.SubString( 1, fileName.Length() - 1 );
      }
   }
   else
      fileName = "";

   return fileName;
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::FileNewActionExecute( TObject */*Sender*/ )
{
   String InitialDir = getDefaultDirectory( false );
   // generate a default filename for a new contest
   char nfileName [ MAXPATH + 10 ];
   nfileName[ 0 ] = 0;
   // create a default filename
   nfileName[ 0 ] = 'C';
   nfileName[ 1 ] = 0;
   dtg d( true );      // get time now
   strncat( nfileName, d.getDate( DTGLOG ).c_str(), 6 );
   nfileName[ 7 ] = 0;
   strcat( nfileName, "A.Minos" );
   while ( nfileName[ 7 ] < 'Z' )
   {
      TEMPBUFF( fileNameBuff, MAXPATH + 10 );
		strcpy( fileNameBuff, InitialDir.t_str() );
      strcat( fileNameBuff, "\\" );
      strcat( fileNameBuff, nfileName );
      struct stat sbuf;
      if ( stat( fileNameBuff, &sbuf ) == 0 )
      {
         nfileName[ 7 ] ++;
      }
      else
         break;
   }

   AnsiString initName = InitialDir + "\\" + nfileName;
   std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );
   BaseContestLog * c = addSlot( pced.get(), initName.c_str(), true, false, -1 );

   if ( c )
   {
      String suggestedfName;
      c->mycall.validate();
      suggestedfName = ( c->mycall.prefix + c->mycall.number + c->mycall.body ).c_str();
      suggestedfName += '_';
      if ( c->DTGStart.getValue().size() )
      {
         suggestedfName += CanonicalToTDT( c->DTGStart.getValue().c_str() ).FormatString( "yyyymmmdd" );
      }
      else
      {
         suggestedfName += TDateTime::CurrentDate().FormatString( "yyyymmmdd" );
      }
      std::string band = c->band.getValue();
      if ( band.size() )
      {
         suggestedfName += '_';
         suggestedfName += band.c_str();
      }
      suggestedfName += ".Minos";

      OpenDialog1->Title = "Save new contest as";
      OpenDialog1->InitialDir = InitialDir;
      OpenDialog1->DefaultExt = "Minos";
      OpenDialog1->Filter = "Minos contest files (*.Minos)|*.Minos|All Files (*.*)|*.*" ;
      OpenDialog1->FileName = suggestedfName;

      closeSlot( false );
      if ( OpenDialog1->Execute() )
      {
         suggestedfName = OpenDialog1->FileName;
         if ( !RenameFile( initName, suggestedfName ) )
         {
            ShowMessage( String( "Failed to rename " ) + initName + " as " + suggestedfName );
            suggestedfName = initName;
         }

         // we want to (re)open it WITHOUT using the dialog!
			addSlot( 0, suggestedfName.t_str(), false, false, -1 );   // not automatically read only
      }
      else
      {
         if ( !DeleteFile( initName ) )
         {
            ShowMessage( String( "Failed to delete " ) + initName );
         }
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestDetailsActionExecute( TObject */*Sender*/ )
{
   BaseContestLog * contest = TContestApp::getContestApp() ->getCurrentContest();
   std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );

   LoggerContestLog *ct = dynamic_cast<LoggerContestLog *>( contest );
   if ( ct )
   {
      pced->setDetails( ct );
      if ( pced->ShowModal() == mrOk )
      {
         ct->commonSave( false );
         // and we need to do some re-init on the display
         TSingleLogFrame *f = findCurrentLogFrame();
         if ( f )
         {
            f->updateQSODisplay();
         }
         ct->startScan();
      }

   }
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::TimeDisplayTimerTimer( TObject */*Sender*/ )
{
   String disp = dtg::getUTC( bigClockCorr ).FormatString( "dd/mm/yyyy hh:nn:ss" ) + " UTC       ";

   LogContainer->StatusBar1->Panels->Items[ 2 ] ->Text = disp;

   if ( TContestApp::getContestApp() )
   {
      std::string statbuf;
      BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
      if ( ct )
      {
         TSingleLogFrame * f = findCurrentLogFrame();
         if ( f )
         {
            f->updateQSOTime();
         }
         ct->setScore( statbuf );
      }
      TContestApp::getContestApp() ->showContestScore( statbuf );
   }

}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::StatusBar1DblClick( TObject */*Sender*/ )
{
   // At the moment, we only want to bring up the clock correction dialog...
   std::auto_ptr <TClockDlg> clockdlg( new TClockDlg( this ) );
   clockdlg->ShowModal();
}
//---------------------------------------------------------------------------
void TLogContainer::selectContest( BaseContestLog *pc, BaseContact *pct )
{
   // we have double clicked on a contact in "other" or "archive" trees
   // so we want to (a) switch tabs and (b) go to that contact edit

   for ( int j = 0; j < ContestPageControl->PageCount; j++ )
   {
      TTabSheet *ctab = ContestPageControl->Pages[ j ];
      int cc = ctab->ControlCount;

      BaseContestLog * clp = pc;
      for ( int i = 0; i < cc; i++ )
      {
         if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( ctab->Controls[ i ] ) )
         {
            if ( f->getContest() == clp )
            {
               ContestPageControl->ActivePage = ctab;         // This doesn't call ContestPageControlChange (see TPageControl::OnChange in  help)
               ContestPageControlChange( this );                // so the contest gets properly switched
               f->GJVQSOLogFrame->selectEntry( pct );
               return ;
            }
         }
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::FormKeyUp( TObject */*Sender*/, WORD &Key,
      TShiftState Shift )
{
   if ( ( Shift.Contains( ssCtrl ) || Shift.Contains( ssShift ) ) && Key >= VK_F1 && Key <= VK_F12 )
   {
      if ( Shift.Contains( ssCtrl ) && Shift.Contains( ssShift ) )
      {
         // keyer record keys
         TSendDM::sendKeyerRecord( Key - VK_F1 + 1 );
      }
      else
      {
         // Keyer play keys
         TSendDM::sendKeyerPlay( Key - VK_F1 + 1 );
      }
   }
   else
      if ( Shift.Empty() )
      {
         if ( ( Key == VK_F1 || Key == VK_F2 || Key == VK_F3 || Key == VK_F5 || Key == VK_PRIOR || Key == VK_NEXT ) )
         {
            TSingleLogFrame * cf = findCurrentLogFrame();
            if ( cf )
            {
               cf->setActiveControl( Key );
            }
         }
      }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::LocCalcActionExecute( TObject */*Sender*/ )
{
   // Show/hide the stand alone locator calculator
   if ( !LocCalcForm )
   {
      LocCalcForm = new TLocCalcForm( 0 );
   }
   LocCalcForm->Visible = true;
   LocCalcForm->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::LoggerActionExecute( TObject */*Sender*/ )
{
   BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::doSetCaption()
{
   if ( captionToSet.Length() )
   {
      if ( captionToSet != Caption )
         Caption = captionToSet;
   }
   else
      if ( Caption != "Minos contest Logger Application" )
         Caption = "Minos contest Logger Application";
}
void __fastcall TLogContainer::doSetMode()
{
   TSingleLogFrame * f = LogContainer->findCurrentLogFrame();
   if ( f )
      f->setMode( modeToSet );
}
void __fastcall TLogContainer::doSetFreq()
{
   TSingleLogFrame * f = LogContainer->findCurrentLogFrame();
   if ( f )
      f->setFreq( freqToSet );
}
void TLogContainer::setCaption( String c )
{
   captionToSet = c;
   syncCaption = true;
}
void TLogContainer::setMode( String m )
{
   modeToSet = m;
   syncMode = true;
}
void TLogContainer::setFreq( String f )
{
   freqToSet = f;
   syncFreq = true;
}
void __fastcall TLogContainer::SyncTimerTimer( TObject */*Sender*/ )
{
   if ( syncCaption )
   {
      syncCaption = false;
      doSetCaption();
   }
   if ( syncMode )
   {
      syncMode = false;
      doSetMode();
   }
   if ( syncFreq )
   {
      syncFreq = false;
      doSetFreq();
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::GoToSerialActionExecute( TObject */*Sender*/ )
{
   TSingleLogFrame * f = findCurrentLogFrame();
   if ( f )
   {
      f->goSerial( );
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::MakeEntryActionExecute( TObject */*Sender*/ )
{
   TSingleLogFrame * f = findCurrentLogFrame();
   if ( f )
   {
      f->makeEntry( false );
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::AnalyseMinosLogActionExecute( TObject */*Sender*/ )
{
   OpenDialog1->InitialDir = getDefaultDirectory( false );
   OpenDialog1->DefaultExt = "Minos";
   OpenDialog1->Filter = "Minos contest files (*.minos)|*.MINOS|All Files (*.*)|*.*" ;
   BaseContestLog * contest = TContestApp::getContestApp() ->getCurrentContest();
   if ( contest )
   {
      OpenDialog1->FileName = contest->cfileName.c_str();
   }
   if ( OpenDialog1->Execute() )
   {
		HANDLE contestFile = CreateFile( OpenDialog1->FileName.t_str(),
													GENERIC_READ,
													FILE_SHARE_READ | FILE_SHARE_WRITE,
													0,                  // security
													OPEN_EXISTING,
													0,
													0 );               // template handle
      if ( contestFile == INVALID_HANDLE_VALUE )
      {
         std::string lerr = lastError();
			std::string emess = std::string( "Failed to open ContestLog file " ) + OpenDialog1->FileName.t_str() + " : " + lerr;
         MinosParameters::getMinosParameters() ->mshowMessage( emess.c_str() );
         return ;
      }

      MinosTestImport mt;
      mt.analyseTest( contestFile );
      CloseHandle( contestFile );
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::NextUnfilledActionExecute( TObject */*Sender*/ )
{
   if ( LogContainer )
   {
      TSingleLogFrame * f = LogContainer->findCurrentLogFrame();
      if ( f )
      {
         f->GoNextUnfilled( );
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestPageControlMouseDown( TObject */*Sender*/,
      TMouseButton Button, TShiftState /*Shift*/, int X, int Y )
{
   if ( GridHintWindow->Showing )
   {
      GridHintWindow->Showing = false;
   }
   GridHintTimer->Enabled = false;
   if ( Button == mbRight )
   {
      // need to select the RC tab
      int RCtab = ContestPageControl->IndexOfTabAt( X, Y );
      ContestPageControl->TabIndex = RCtab;
      enableActions();
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestMRUClick( TObject */*Sender*/,
      const UnicodeString FileName )
{
	TWaitCursor fred;
   if ( FileExists( FileName ) )
   {
		std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );
		String fn = FileName;
		addSlot( pced.get(), fn.t_str(), false, false, -1 );   // not automatically read only
   }
   else
   {
      ContestMRU->RemoveItem( FileName );
   }
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::File1Click( TObject */*Sender*/ )
{
	ReopenMenu->Enabled = ( ContestMRU->Items->Count != 0 );
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::GridHintTimerTimer( TObject */*Sender*/ )
{
   // timer used to time out the grid hint - but we don't
   GridHintTimer->Enabled = false;
   if ( !GridHintWindow )
      return ;
   if ( GridHintWindow->Showing || !Application->Active )
   {
      GridHintWindow->Showing = false;
   }
   else
   {
      if ( GridHintWindow->GetHintControl() == ContestPageControl )
      {
         TSingleLogFrame * lf = findCurrentLogFrame();
         POINT mpos, mpos2;
         ::GetCursorPos( &mpos );
         if ( lf )
         {
            mpos2 = lf->ScreenToClient( mpos );
				if ( PtInRect( &( lf->ClientRect ), mpos2 ) )
            {
               GridHintWindow->Showing = false;
               return ;
            }

         }
         mpos2 = ContestPageControl->ScreenToClient( mpos );

         if ( PtInRect( &( ContestPageControl->ClientRect ), mpos2 ) )
         {
            GridHintWindow->SetXY( mpos.x, mpos.y + 20 );
            GridHintWindow->Showing = true;
         }
      }
      else
      {
         GridHintWindow->Showing = false;
      }
   }

}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ApplicationEvents1Deactivate( TObject */*Sender*/ )
{
   if ( !GridHintWindow )
      return ;
   // kill the hint
   if ( GridHintWindow->Showing )
   {
      GridHintWindow->Showing = false;
   }
   GridHintTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestPageControlMouseMove( TObject */*Sender*/,
      TShiftState /*Shift*/, int X, int Y )
{
   GridHintWindow->SetHintControl( ContestPageControl );
   if ( ( X != oldX ) || ( Y != oldY ) )
   {
      GridHintWindow->Showing = false;
      GridHintTimer->Enabled = false;

      oldX = X;
      oldY = Y;

      try
      {
         int tabno = ContestPageControl->IndexOfTabAt( X, Y );

         if ( tabno != -1 )
         {
            // and we want to set the hint position to the current mouse position
            GridHintTimer->Enabled = true;
            GridHintTimer->Interval = 500;
            // We actually want the contest cfileName and the contest name etc here
            // - caption should be short file name
            TTabSheet *ctab = ContestPageControl->Pages[ tabno ];
            int cc = ctab->ControlCount;

            for ( int i = 0; i < cc; i++ )
            {
               if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( ctab->Controls[ i ] ) )
               {
                  // DTGStart should be cut to date part only, and NOT be in ISO format!
                  // For now, leave it out!
                  ContestPageControl->Hint = ( f->getContest() ->cfileName +
                                               "\r\n" +
                                               f->getContest() ->name.getValue()
                                             ).c_str();
                  ContestPageControl->ShowHint = false;
                  return ;
               }
            }
         }
      }
      catch ( ... )
      {}
   }

}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::FormResize( TObject */*Sender*/ )
{
   if ( saveResize )
   {
      TContestApp::getContestApp() ->displayBundle.setIntProfile( edpLeft, Left );
      TContestApp::getContestApp() ->displayBundle.setIntProfile( edpTop, Top );
      TContestApp::getContestApp() ->displayBundle.setIntProfile( edpWidth, Width );
      TContestApp::getContestApp() ->displayBundle.setIntProfile( edpHeight, Height );
      TContestApp::getContestApp() ->displayBundle.flushProfile();
   }
}
void __fastcall TLogContainer::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::SetTimeNowActionExecute( TObject *Sender )
{
   TSingleLogFrame * f = LogContainer->findCurrentLogFrame();
   if ( f )
   {
      f->SetTimeNowClick( Sender );
   }
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::NextContactDetailsOnLeftActionExecute( TObject */*Sender*/ )
{
   bool ncdol = !isNextContactDetailsOnLeft();
   NextContactDetailsOnLeftAction->Checked = ncdol;
   TContestApp::getContestApp() ->displayBundle.setBoolProfile( edpNextContactDetailsOnLeft, ncdol );
   TSingleLogFrame * f = LogContainer->findCurrentLogFrame();
   if ( f )
   {
      f->OnShowTimer->Enabled = true;
   }
   TContestApp::getContestApp() ->displayBundle.flushProfile();
}
//---------------------------------------------------------------------------
void TLogContainer::setBandMapLoaded()
{
   bandMapLoaded = true;
}
//---------------------------------------------------------------------------
bool TLogContainer::isBandMapLoaded()
{
   return bandMapLoaded;
}
//---------------------------------------------------------------------------
TSingleLogFrame *TLogContainer::findContest( const std::string &pubname )
{
   for ( int j = 0; j < ContestPageControl->PageCount; j++ )
   {
      TTabSheet *ctab = ContestPageControl->Pages[ j ];
      int cc = ctab->ControlCount;

      for ( int i = 0; i < cc; i++ )
      {
         if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( ctab->Controls[ i ] ) )
         {
            if ( f->getContest() ->publishedName == pubname )
            {
               return f;
            }
         }
      }
   }

   return 0;
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ManageListsActionExecute( TObject */*Sender*/ )
{
   std::auto_ptr <TManageListsDlg> manageListsDlg( new TManageListsDlg( this ) );
   manageListsDlg->ShowModal();
   enableActions();
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ShiftRightAction( TObject */*Sender*/ )
{
   // We want to reorder the tabs so that this one goes right
   if ( !ContestPageControl->ActivePage )
      return ;
   int tno = ContestPageControl->ActivePage->PageIndex;
   if ( tno < ContestPageControl->PageCount - 1 )
   {
      ContestSlot * cs = TContestApp::getContestApp() ->contestSlotList[ tno ];
      int s = cs->slotno;

      ContestSlot *csp1 = TContestApp::getContestApp() ->contestSlotList[ tno + 1 ];
      int sp1 = csp1->slotno;

      TContestApp::getContestApp() ->contestSlotList[ tno ] = csp1;
      csp1->slotno = s;

      TContestApp::getContestApp() ->contestSlotList[ tno + 1 ] = cs;
      cs->slotno = sp1;

      TContestApp::getContestApp() ->writeContestList();

      tno++;
      ContestPageControl->ActivePage->PageIndex = tno;

      enableActions();
   }
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ShiftLeftAction( TObject */*Sender*/ )
{
   //
   if ( !ContestPageControl->ActivePage )
      return ;
   int tno = ContestPageControl->ActivePage->PageIndex;
   if ( tno > 0 )
   {
      ContestSlot * cs = TContestApp::getContestApp() ->contestSlotList[ tno ];
      int s = cs->slotno;
      ContestSlot *csm1 = TContestApp::getContestApp() ->contestSlotList[ tno - 1 ];
      int sm1 = csm1->slotno;
      TContestApp::getContestApp() ->contestSlotList[ tno ] = csm1;
      csm1->slotno = s;

      TContestApp::getContestApp() ->contestSlotList[ tno - 1 ] = cs;
      cs->slotno = sm1;

      tno--;
      ContestPageControl->ActivePage->PageIndex = tno;
      TContestApp::getContestApp() ->writeContestList();

      enableActions();
   }
}
//---------------------------------------------------------------------------


