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

class TOptionFrame;
//---------------------------------------------------------------------------
class TEntryOptionsForm : public TForm
{
   __published:  	// IDE-managed Components
   TScrollBox *OptionsScrollBox;
   TPanel *Panel1;
   TButton *CloseButton;
   TTimer *GridHintTimer;
   TSaveDialog *SaveDialog;
   TButton *CancelButton;
   TRadioGroup *EntryGroup;
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall CancelButtonClick( TObject *Sender );
   private:  	// User declarations
      LoggerContestLog * ct;

      std::vector<TOptionFrame *> options;

      ExportType expformat;
      bool minosSave;
      void getContestOperators();
      int opsQSOLine1;
      int opsQSOLine2;
      int opsEntryLine1;
      int opsEntryLine2;
      TGridHint *GridHintWindow;
      int HintRow;
      int HintCol;
      int oldX;
      int oldY;
   public:  		// User declarations
      __fastcall TEntryOptionsForm( TComponent* Owner, LoggerContestLog * , bool minosSave );
      String doFileSave( );
};
//---------------------------------------------------------------------------
#endif
