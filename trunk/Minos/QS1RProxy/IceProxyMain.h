//---------------------------------------------------------------------------

#ifndef IceProxyMainH
#define IceProxyMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TQRigSyncMain : public TForm
{
__published:	// IDE-managed Components
   TButton *CloseButton;
   TTimer *Timer1;
   TLabel *Label1;
   TButton *MuteButton;
   TTimer *LogTimer;
   TTimer *StartUpTimer;
   void __fastcall CloseButtonClick(TObject *Sender);
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall MuteButtonClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall LogTimerTimer(TObject *Sender);
   void __fastcall StartUpTimerTimer(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
private:	// User declarations
   Ice::CommunicatorPtr ic;
   QS1R::RadioCommanderPrx qs1r;
   bool muted;

   bool saveResize;
   void __fastcall logMessage( std::string s );

   static void makeRPCObjects();
   void secondRXClientCallback( bool err, MinosRPCObj *mro, const std::string &from );
   void secondRXServerCallback( bool err, MinosRPCObj *mro, const std::string &from );

public:		// User declarations
   __fastcall TQRigSyncMain(TComponent* Owner);
   __fastcall ~TQRigSyncMain();

   void publishCallback( bool err, MinosRPCObj *mro, const std::string &from );
   void __fastcall WmMove( TMessage &Msg );
   BEGIN_MESSAGE_MAP
   MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
   END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE TQRigSyncMain *QRigSyncMain;
//---------------------------------------------------------------------------
#endif
