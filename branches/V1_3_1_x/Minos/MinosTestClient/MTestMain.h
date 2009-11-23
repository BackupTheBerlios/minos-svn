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

#ifndef MTestMainH
#define MTestMainH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TClientMain : public TForm
{
   __published:  	// IDE-managed Components
      TTimer *LogTimer;
      TPanel *SPanel1;
      TLabel *Label1;
      TLabel *Label4;
      TLabel *Label5;
      TLabel *Label8;
      TMemo *LogMemo;
      TButton *CloseButton;
      TEdit *FromUser;
      TEdit *ToUser;
      TEdit *ToHost;
      TEdit *MessageEdit;
      TButton *SendButton;
      TButton *SubscribeButton;
      TTimer *StartupTimer;
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall SendButtonClick( TObject *Sender );
      void __fastcall LogTimerTimer( TObject *Sender );
      void __fastcall SubscribeButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall StartupTimerTimer( TObject *Sender );
      void __fastcall FormResize( TObject *Sender );
   private:  	// User declarations
      bool subscribed;
      bool saveResize;
   public:  		// User declarations
      __fastcall TClientMain( TComponent* Owner );
      void __fastcall logMessage( std::string s );
      void notifyCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void controlClientCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void controlServerCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void __fastcall WmMove( TMessage &Msg );
      BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
      END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE TClientMain *ClientMain;
//---------------------------------------------------------------------------
#endif

