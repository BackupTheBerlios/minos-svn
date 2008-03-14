/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef SettingsEditorH
#define SettingsEditorH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp> 
//---------------------------------------------------------------------------
class TSettingsEditDlg : public TForm
{
   __published:  	// IDE-managed Components
      TPanel *Panel1;
      TButton *OKButton;
      TButton *CopyButton;
      TPanel *Panel2;
      TListBox *SectionsList;
      TStringGrid *SectionGrid;
      TButton *DeleteButton;
      TButton *CancelButton;
      TButton *NewSectionButton;
      TTimer *GridHintTimer;
      void __fastcall OKButtonClick( TObject *Sender );
      void __fastcall CopyButtonClick( TObject *Sender );
      void __fastcall SectionsListClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall DeleteButtonClick( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
      void __fastcall NewSectionButtonClick( TObject *Sender );
      void __fastcall GridHintTimerTimer( TObject *Sender );
      void __fastcall SectionGridMouseMove( TObject *Sender, TShiftState Shift,
                                            int X, int Y );
      void __fastcall FormMouseDown( TObject *Sender, TMouseButton Button,
                                     TShiftState Shift, int X, int Y );
   private:  	// User declarations
      SettingsBundle *bundle;
      std::string initialSection;
      void showSections();
      void showSection();
      void showDetails();
      void getDetails();
      TGridHint *GridHintWindow;
      int HintRow;
      int HintCol;
      int oldX;
      int oldY;
   public:  		// User declarations
      __fastcall TSettingsEditDlg( TComponent* Owner, SettingsBundle *b );
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsEditDlg *SettingsEditDlg;
//---------------------------------------------------------------------------
#endif
