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
class TOptionFrame;
class TSettingsEditDlg : public TForm
{
   __published:  	// IDE-managed Components
      TPanel *Panel1;
      TButton *OKButton;
      TButton *CopyButton;
      TPanel *Panel2;
      TListBox *SectionsList;
      TButton *DeleteButton;
      TButton *CancelButton;
      TButton *NewSectionButton;
   TScrollBox *OptionsScrollBox;
      void __fastcall OKButtonClick( TObject *Sender );
      void __fastcall CopyButtonClick( TObject *Sender );
      void __fastcall SectionsListClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall DeleteButtonClick( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
      void __fastcall NewSectionButtonClick( TObject *Sender );
   private:  	// User declarations
      std::vector<TOptionFrame *> options;
      bool currSectionOnly;
      SettingsBundle *bundle;
      std::string initialSection;
      void showSections();
      void showSection();
      void showDetails();
      void getDetails();
   public:  		// User declarations
      __fastcall TSettingsEditDlg( TComponent* Owner, SettingsBundle *b );
      void ShowCurrentSectionOnly();
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsEditDlg *SettingsEditDlg;
//---------------------------------------------------------------------------
#endif
