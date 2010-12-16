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
      bool mlpa = isScrollingContestTabs();
      ScrollingContestTabsAction->Checked = mlpa;
      ContestPageControl->MultiLine = !mlpa;

      bool so = isShowOperators();
      ShowOperatorsAction->Checked = so;

      SendDM = new TSendDM( this );
      if ( contestAppLoadFiles() )
      {
         // here need to pre-open the contest list
         std::string conarg;
         if ( ParamCount() >= 1 )
         {
            conarg = ParamStr( 1 ).c_str();
         }
         preloadFiles( conarg );
         enableActions();
      }
   }
   else
      Close();
}
//---------------------------------------------------------------------------
void TLogContainer::preloadFiles( const std::string &conarg )
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
            addSlot( 0, pathlst[ i ], false, slotno );
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

   if ( conarg.size() )
   {
      // open the "argument" one last - which will make it current
      ct = addSlot( 0, conarg, false, -1 );
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
BaseContestLog * TLogContainer::addSlot( TContestEntryDetails *ced, const std::string &fname, bool newfile, int slotno )
{
   static int namegen = 0;
   // openFile ends up calling ContestLog::initialise which then
   // calls TContestApp::insertContest

   LoggerContestLog * contest = TContestApp::getContestApp() ->openFile( fname, newfile, slotno );

   if ( contest )
   {
      bool show = false;
      if ( ced )
      {

         ced->setDetails( contest );

         {
            if ( ced->ShowModal() == mrOk )
            {
               contest->scanContest();
               show = true;
            }
            else
            {
               TContestApp::getContestApp() ->closeFile( contest );
               contest = 0;
            }
         }
      }
      else
      {
         contest->scanContest();
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

         f->logColumnsChanged = true;  // also causes show QSOs
         f->splittersChanged = true;

         t->PageControl = ContestPageControl;
         ContestPageControl->ActivePage = t;
         ContestPageControlChange( this );

         if ( contest->needsExport() )      // imported from an alien format (e.g. .log)
         {
            String expName = f->makeEntry( true );
            if ( expName.Length() )
            {
               closeSlot(t, true );
               addSlot( 0, expName.c_str(), false, -1 );
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
TSingleLogFrame *TLogContainer::findLogFrame(TTabSheet *t)
{
   // we need to find the embedded frame...
   // now ONLY used in closeSlot!
   if ( !t )
      return 0;
   int cc = t->ControlCount;
   for ( int i = 0; i < cc; i++ )
   {
      if ( TSingleLogFrame * f = dynamic_cast<TSingleLogFrame *>( t->Controls[ i ] ) )
      {
         return f;
      }
   }
   return 0;
}
//---------------------------------------------------------------------------
/*
TSingleLogFrame *TLogContainer::findCurrentLogFrame()
{
   return findLogFrame(ContestPageControl->ActivePage);
}
*/
//---------------------------------------------------------------------------
void TLogContainer::showContestScore( const std::string &score )
{
   StatusBar1->Panels->Items[ 0 ] ->Text = score.c_str();
}
//---------------------------------------------------------------------------
void TLogContainer::closeSlot(TTabSheet *t, bool addToMRU )
{
   if ( t )
   {
      TSingleLogFrame * f = findLogFrame(t);
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
   TTabSheet * t = ContestPageControl->ActivePage;
   closeSlot(t, true );
}

//---------------------------------------------------------------------------

void __fastcall TLogContainer::CloseAllActionExecute(TObject */*Sender*/)
{
   TWaitCursor fred;
   while ( ContestPageControl->PageCount)
   {
      // Keep closing the current (and hence visible) contest
      TTabSheet * t = ContestPageControl->ActivePage;
      closeSlot(t, true);
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
      closeSlot(ctab, true);
   }
   ContestPageControlChange( this );
   enableActions();
}
//---------------------------------------------------------------------------

void TLogContainer::enableActions()
{
   bool f = ( ContestPageControl->ActivePage != 0 );

   LoggerAction->Enabled = true;
   LocCalcAction->Enabled = true;
   FileNewAction->Enabled = true;
   HelpAboutAction->Enabled = true;

   FileCloseAction1->Enabled = f;
   CloseAllAction->Enabled = f;
   CloseAllButAction->Enabled = f;

   NextContactDetailsOnLeftAction->Enabled = true;

   ContestDetailsAction->Enabled = f;
   GoToSerialAction->Enabled = f;
   MakeEntryAction->Enabled = f;

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
bool TLogContainer::isScrollingContestTabs()
{
   bool ncdol;
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpScrollingContestTabs, ncdol );
   return ncdol;
}
//---------------------------------------------------------------------------
bool TLogContainer::isShowOperators()
{
   bool ncdol;
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpShowOperators, ncdol );
   return ncdol;
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestPageControlChange( TObject */*Sender*/ )
{
   MinosLoggerEvents::SendContestPageChanged();
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
   FileOpen1->Dialog->DefaultExt = "minos";
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
   addSlot( pced.get(), FileOpen1->Dialog->FileName.c_str(), false, -1 );   // not automatically read only
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::FileOpen1Cancel( TObject */*Sender*/ )
{
   TContestApp::getContestApp() ->writeContestList();
}
//---------------------------------------------------------------------------

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
   addListSlot( pced.get(), ListOpen1->Dialog->FileName.c_str(), -1 );
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

   // we default to ".\\logs" or ".\\lists. On Vista this won't be relative to
   // the executable, but to the user data directory (I think). This is why Minos
   // invents a very strange default directory which you then cannot find.

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
      strcpy( fileNameBuff, InitialDir.c_str() );
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

   String initName = InitialDir + "\\" + nfileName;
   std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );
   BaseContestLog * c = addSlot( pced.get(), initName.c_str(), true, -1 );

   bool repeatDialog = (c != 0);
   while ( repeatDialog )
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

      closeSlot(ContestPageControl->ActivePage, false );
      if ( OpenDialog1->Execute() )
      {
         suggestedfName = OpenDialog1->FileName;
         if ( !RenameFile( initName, suggestedfName ) )
         {
            ShowMessage( String( "Failed to rename " ) + initName + " as " + suggestedfName );
            suggestedfName = initName;
         }

         // we want to (re)open it WITHOUT using the dialog!
         addSlot( 0, suggestedfName.c_str(), false, -1 );
         repeatDialog = false;
      }
      else
      {
            std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );
            c = addSlot( pced.get(), initName.c_str(), false, -1 );
            repeatDialog = (c != 0);
      }
   }
   if (!c)
   {
      if ( !DeleteFile( initName ) )
      {
         ShowMessage( String( "Failed to delete " ) + initName );
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestDetailsActionExecute( TObject */*Sender*/ )
{
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendContestDetails(ct);
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::TimeDisplayTimerTimer( TObject */*Sender*/ )
{
   String disp = dtg::getCorrectedUTC( ).FormatString( "dd/mm/yyyy hh:nn:ss" ) + " UTC       ";

   LogContainer->StatusBar1->Panels->Items[ 2 ] ->Text = disp;

   if ( TContestApp::getContestApp() )
   {
      std::string statbuf;
      BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
      if ( ct )
      {
         ct->setScore( statbuf );
      }
      showContestScore( statbuf );
   }

}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::CorrectDateTimeActionExecute(TObject */*Sender*/)
{
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
               ContestPageControlChange( this );              // so the contest gets properly switched
               f->QSOTreeSelectContact( pct );         // which triggers edit on the contact
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
         BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
         MinosLoggerEvents::SendFormKey(&Key, ct);
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
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendSetMode(modeToSet, ct);
}
void __fastcall TLogContainer::doSetFreq()
{
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendSetFreq(freqToSet, ct);
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
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendGoToSerial(ct);
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::MakeEntryActionExecute( TObject */*Sender*/ )
{
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendMakeEntry(ct);
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
      HANDLE contestFile = CreateFile( OpenDialog1->FileName.c_str(),
                                       GENERIC_READ,
                                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                                       0,                  // security
                                       OPEN_EXISTING,
                                       0,
                                       0 );               // template handle
      if ( contestFile == INVALID_HANDLE_VALUE )
      {
         std::string lerr = lastError();
         std::string emess = std::string( "Failed to open ContestLog file " ) + OpenDialog1->FileName.c_str() + " : " + lerr;
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
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendNextUnfilled(ct);
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
      MinosLoggerEvents::SendContestPageChanged();
      enableActions();
   }
}
//---------------------------------------------------------------------------
void __fastcall TLogContainer::ContestMRUClick( TObject */*Sender*/,
      const AnsiString FileName )
{
   TWaitCursor fred;
   if ( FileExists( FileName ) )
   {
      std::auto_ptr <TContestEntryDetails> pced( new TContestEntryDetails( this ) );
      addSlot( pced.get(), FileName.c_str(), false, -1 );
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
         POINT mpos, mpos2;
         ::GetCursorPos( &mpos );
         if ( ContestPageControl->ActivePage )
         {
            mpos2 = ContestPageControl->ActivePage->ScreenToClient( mpos );
            if ( PtInRect( &( ContestPageControl->ActivePage->ClientRect ), mpos2 ) )
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
            // Grid Hint is only actually used for hover over the actual tab to show full contest detail
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
void __fastcall TLogContainer::NextContactDetailsOnLeftActionExecute( TObject */*Sender*/ )
{
   bool ncdol = !isNextContactDetailsOnLeft();
   NextContactDetailsOnLeftAction->Checked = ncdol;
   TContestApp::getContestApp() ->displayBundle.setBoolProfile( edpNextContactDetailsOnLeft, ncdol );
   TContestApp::getContestApp() ->displayBundle.flushProfile();

   MinosLoggerEvents::SendNextContactDetailsOnLeft();
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

void __fastcall TLogContainer::StatusBar1Resize(TObject */*Sender*/)
{
   int w1 = StatusBar1->Canvas->TextWidth("XXOverwriteXX");
   int w2 = StatusBar1->Canvas->TextWidth("XXXXX25/12/1953 01:00:00 UTCXXXXXX");
   StatusBar1->Panels->Items[0]->Width = StatusBar1->Width - w1 - w2;
   StatusBar1->Panels->Items[1]->Width = w1;
   // and Items[2] fills the rest
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::Tools1Click(TObject */*Sender*/)
{
   AnalyseMinosLog1->Visible = IsDebuggerPresent();
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::EditContactActionExecute(TObject */*Sender*/)
{
   BaseContestLog * ct = TContestApp::getContestApp() ->getCurrentContest();
   MinosLoggerEvents::SendEditMatchContact(ct);
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ScrollingContestTabsActionExecute(TObject */*Sender*/)
{
   bool mlpa = !isScrollingContestTabs();
   ScrollingContestTabsAction->Checked = mlpa;
   ContestPageControl->MultiLine = !mlpa;
   TContestApp::getContestApp() ->displayBundle.setBoolProfile( edpScrollingContestTabs, mlpa );
   TContestApp::getContestApp() ->displayBundle.flushProfile();
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ShowOperatorsActionExecute(TObject */*Sender*/)
{
   bool so = !isShowOperators();
   ShowOperatorsAction->Checked = so;
   TContestApp::getContestApp() ->displayBundle.setBoolProfile( edpShowOperators, so );
   TContestApp::getContestApp() ->displayBundle.flushProfile();
   MinosLoggerEvents::SendShowOperators();
}
//---------------------------------------------------------------------------

void __fastcall TLogContainer::ContestPageControlDrawTab(
      TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active)
{
   if (Active)
   {
      Control->Canvas->Brush->Color = clAqua;
      Control->Canvas->FillRect(Rect);
   }
   else
   {
      TSingleLogFrame *f = findLogFrame(ContestPageControl->Pages[TabIndex]);
      if (f && f->getContest() && f->getContest()->isProtected())
      {
         if (f->getContest()->isProtectedSuppressed())
         {
            Control->Canvas->Brush->Color = clLime;
         }
         else
         {
            Control->Canvas->Brush->Color = clMoneyGreen;
         }

      }
      else
      {
         Control->Canvas->Brush->Color = clSkyBlue;
      }
      Control->Canvas->FillRect(Rect);
   }
   TTabSheet *ttc = ContestPageControl->Pages[TabIndex];
   if (ttc)
   {
      Control->Canvas->TextRect(
            Rect,
            Rect.Left + (Rect.Right - Rect.Left - Control->Canvas->TextWidth(ttc->Caption)) / 2,
            Rect.Top + (Rect.Bottom - Rect.Top - Control->Canvas->TextHeight(ttc->Caption)) / 2,
            ttc->Caption);
   }
}
//---------------------------------------------------------------------------


