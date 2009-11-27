/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef LogMainH
#define LogMainH
#include <ActnCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ActnMenus.hpp>
#include <BandActn.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <XPStyleActnCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include "adpMRU.hpp"
#include <AppEvnts.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <ExtActns.hpp>
#include <StdActns.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------
class TContactListDetails;
class TContestEntryDetails;
//---------------------------------------------------------------------------
class TLogContainer : public TForm
{
   __published:  	// IDE-managed Components
      TActionManager *ActionManager;
      TEditCut *EditCut1;
      TEditCopy *EditCopy1;
      TEditPaste *EditPaste1;
      TEditSelectAll *EditSelectAll1;
      TEditUndo *EditUndo1;
      TEditDelete *EditDelete1;
      TSearchFind *SearchFind1;
      TSearchFindNext *SearchFindNext1;
      TSearchReplace *SearchReplace1;
      TSearchFindFirst *SearchFindFirst1;
      TCustomizeActionBars *CustomizeActionBars1;
      TAction *HelpAboutAction;
      TFileOpen *FileOpen1;
      TFileExit *FileExit1;
      TAction *FileCloseAction1;
      TPageControl *ContestPageControl;
      TAction *ContestDetailsAction;
      TStatusBar *StatusBar1;
      TTimer *TimeDisplayTimer;
      TAction *LocCalcAction;
      TTimer *StartupTimer;
      TAction *LoggerAction;
      TTimer *SyncTimer;
      TAction *GoToSerialAction;
      TAction *MakeEntryAction;
      TAction *AnalyseMinosLogAction;
      TOpenDialog *OpenDialog1;
      TPopupMenu *TabPopup;
      TMenuItem *CloseContest1;
      TMenuItem *Cancel1;
      TMenuItem *Details1;
      TMenuItem *GoToSerial1;
      TMenuItem *ProduceEntryExportFile1;
      TMainMenu *MainMenu1;
      TMenuItem *File1;
      TMenuItem *Open1;
      TMenuItem *Deta1;
      TMenuItem *New1;
      TMenuItem *ReopenMenu;
      TMenuItem *ProduceEntryExportFile2;
      TMenuItem *Close1;
      TMenuItem *Exit1;
      TMenuItem *Edit1;
      TMenuItem *Cut1;
      TMenuItem *Copy1;
      TMenuItem *Paste1;
      TMenuItem *SelectAll1;
      TMenuItem *Undo1;
      TMenuItem *Delete1;
      TMenuItem *Search1;
      TMenuItem *Find1;
      TMenuItem *GoToContactSerial1;
      TMenuItem *GotonextUnfilled1;
      TMenuItem *FindNext1;
      TMenuItem *Replace1;
      TMenuItem *FindFirst1;
   TMenuItem *Tools1;
      TMenuItem *LocatorCalculator1;
      TMenuItem *Logger1;
      TMenuItem *AnalyseMinosLog1;
      TMenuItem *Help1;
      TMenuItem *About1;
      TadpMRU *ContestMRU;
      TTimer *GridHintTimer;
      TApplicationEvents *ApplicationEvents1;
      TAction *NextUnfilledAction;
      TMenuItem *GotoNextUnfilled2;
      TAction *FileNewAction;
      TMenuItem *AnalyseMinosLog2;
      TMenuItem *AnalyseMinosLog3;
      TMenuItem *SetTimeToNow1;
      TAction *SetTimeNowAction;
      TAction *NextContactDetailsOnLeftAction;
      TFileOpen *ListOpen1;
      TMenuItem *OpenList1;
      TAction *ManageListsAction;
      TMenuItem *ManageLists1;
      TMenuItem *ShiftLeft1;
      TMenuItem *ShiftRight1;
      TAction *ShiftTabLeftAction;
      TAction *ShiftTabRightAction;
   TMenuItem *CloseAllContest;
   TAction *CloseAllAction;
   TAction *CloseAllButAction;
   TMenuItem *Closeallbutthiscontest1;
   TMenuItem *N1;
   TMenuItem *N2;
   TMenuItem *N3;
   TMenuItem *N4;
   TMenuItem *N5;
   TMenuItem *N6;
   TMenuItem *N7;
   TMenuItem *Correctdatetime1;
   TAction *CorrectDateTimeAction;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall HelpAboutActionExecute( TObject *Sender );
      void __fastcall FileOpen1BeforeExecute( TObject *Sender );
      void __fastcall FileOpen1Accept( TObject *Sender );
      void __fastcall FileOpen1Cancel( TObject *Sender );
      void __fastcall FileCloseAction1Execute( TObject *Sender );
      void __fastcall ContestPageControlChange( TObject *Sender );
      void __fastcall ContestDetailsActionExecute( TObject *Sender );
      void __fastcall TimeDisplayTimerTimer( TObject *Sender );
      void __fastcall FormKeyUp( TObject *Sender, WORD &Key,
                                 TShiftState Shift );
      void __fastcall LocCalcActionExecute( TObject *Sender );
      void __fastcall StartupTimerTimer( TObject *Sender );
      void __fastcall LoggerActionExecute( TObject *Sender );
      void __fastcall SyncTimerTimer( TObject *Sender );
      void __fastcall GoToSerialActionExecute( TObject *Sender );
      void __fastcall MakeEntryActionExecute( TObject *Sender );
      void __fastcall AnalyseMinosLogActionExecute( TObject *Sender );
      void __fastcall ContestPageControlMouseDown( TObject *Sender,
            TMouseButton Button, TShiftState Shift, int X, int Y );
      void __fastcall ContestMRUClick( TObject *Sender, const AnsiString FileName );
      void __fastcall File1Click( TObject *Sender );
      void __fastcall GridHintTimerTimer( TObject *Sender );
      void __fastcall ApplicationEvents1Deactivate( TObject *Sender );
      void __fastcall ContestPageControlMouseMove( TObject *Sender,
            TShiftState Shift, int X, int Y );
      void __fastcall FormResize( TObject *Sender );
      void __fastcall NextUnfilledActionExecute( TObject *Sender );
      void __fastcall FileNewActionExecute( TObject *Sender );
      void __fastcall SetTimeNowActionExecute( TObject *Sender );
      void __fastcall NextContactDetailsOnLeftActionExecute( TObject *Sender );
      void __fastcall ListOpen1BeforeExecute( TObject *Sender );
      void __fastcall ListOpen1Accept( TObject *Sender );
      void __fastcall ListOpen1Cancel( TObject *Sender );
      void __fastcall ManageListsActionExecute( TObject *Sender );
      void __fastcall ShiftRightAction( TObject *Sender );
      void __fastcall ShiftLeftAction( TObject *Sender );
   void __fastcall CloseAllActionExecute(TObject *Sender);
   void __fastcall CloseAllButActionExecute(TObject *Sender);
   void __fastcall CorrectDateTimeActionExecute(TObject *Sender);
   void __fastcall StatusBar1Resize(TObject *Sender);
   void __fastcall Tools1Click(TObject *Sender);
   private:  	// User declarations
      BaseContestLog * addSlot( TContestEntryDetails *ced, const std::string &fname, bool newfile, bool read_only, int slotno );
      ContactList * addListSlot( TContactListDetails *ced, const std::string &fname, int slotno );
      void closeSlot( bool addToMRU );
      void preloadFiles( const std::string &conarg );
      void enableActions();
      String getDefaultDirectory( bool IsList );

      bool saveResize;
      bool syncCaption;
      String captionToSet;
      void __fastcall doSetCaption();
      bool syncMode;
      String modeToSet;
      void __fastcall doSetMode();
      bool syncFreq;
      String freqToSet;
      void __fastcall doSetFreq();
      bool bandMapLoaded;

   public:  		// User declarations
      static void showContestScore( const std::string &score );
      TSingleLogFrame *findCurrentLogFrame();
      void selectContest( BaseContestLog * , BaseContact * );
      __fastcall TLogContainer( TComponent* Owner );
      TSingleLogFrame *findContest( const std::string &pubname );

      TGridHint *GridHintWindow;
      int oldX;
      int oldY;

      void setCaption( String );
      void setMode( String );
      void setFreq( String );
      void setBandMapLoaded();
      bool isBandMapLoaded();
      bool isNextContactDetailsOnLeft();

      void __fastcall WmMove( TMessage &Msg );
      BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
      END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE TLogContainer *LogContainer;
//---------------------------------------------------------------------------
#endif
