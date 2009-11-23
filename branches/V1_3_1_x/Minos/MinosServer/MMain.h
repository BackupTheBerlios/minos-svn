/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef MMainH
#define MMainH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <AppEvnts.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp> 
//---------------------------------------------------------------------------
class TMinosMainForm : public TForm
{
   __published:  	// IDE-managed Components
      TTimer *LogTimer;
      TApplicationEvents *ApplicationEvents1;
      TMainMenu *MainMenu1;
      TMenuItem *File1;
      TMenuItem *N1;
      TMenuItem *Exit1;
      TPanel *SPanel1;
      TButton *CloseButton;
      TTimer *ScanTimer;
      TLabel *ClientLabel;
      TLabel *ServerLabel;
      TLabel *PubLabel;
      TLabel *SubLabel;
      TLabel *Label1;
      TLabel *Label2;
      TLabel *Label3;
      TLabel *Label4;
      void __fastcall LogTimerTimer( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall Exit1Click( TObject *Sender );
      void __fastcall ApplicationEvents1Exception( TObject *Sender,
            Exception *E );
      void __fastcall ScanTimerTimer( TObject *Sender );
   private:  	// User declarations
   public:  		// User declarations
      void __fastcall logMessage( std::string s );
      __fastcall TMinosMainForm( TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TMinosMainForm *MinosMainForm;
extern bool closeApp;
//---------------------------------------------------------------------------
#endif
