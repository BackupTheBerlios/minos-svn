/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef EntryOptionsH
#define EntryOptionsH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp> 
//---------------------------------------------------------------------------
class TEntryOptionsForm : public TForm
{
   __published:  	// IDE-managed Components
      TRadioGroup *EntryGroup;
      TButton *CloseButton;
      TStringGrid *DetailGrid;
      TButton *CancelButton;
      TSaveDialog *SaveDialog;
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
      void __fastcall DetailGridSelectCell( TObject *Sender, int ACol, int ARow,
                                            bool &CanSelect );
   private:  	// User declarations
      LoggerContestLog * ct;
      //      void mod_operators();
      ExportType expformat;
      bool minosSave;
   public:  		// User declarations
      __fastcall TEntryOptionsForm( TComponent* Owner, LoggerContestLog * , bool minosSave );
      std::string doFileSave( );
};
//---------------------------------------------------------------------------
#endif
