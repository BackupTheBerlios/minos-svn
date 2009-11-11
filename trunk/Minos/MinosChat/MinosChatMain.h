//---------------------------------------------------------------------------

#ifndef MinosChatMainH
#define MinosChatMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Server
{
   public:
      std::string name;
      std::string ip;
      PublishState state;
};
class TMinosChatForm : public TForm
{
   __published: 	// IDE-managed Components
      TTimer *LogTimer;
      TTimer *ConnectTimer;
      TTimer *StartUpTimer;
   TPanel *Panel2;
   TListBox *StationList;
   TMemo *ChatMemo;
   TSplitter *Splitter1;
   TPanel *Panel1;
   TLabeledEdit *ChatEdit;
   TButton *SendButton;
   TPanel *Panel3;
   TImage *Splitter_Image;
      void __fastcall LogTimerTimer( TObject *Sender );
      void __fastcall ConnectTimerTimer( TObject *Sender );
      void __fastcall SendButtonClick( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall StartUpTimerTimer( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall FormResize( TObject *Sender );
   void __fastcall Splitter1Moved(TObject *Sender);
   private: 	// User declarations
      TWhisperSplitter *WSplitter1;
      bool connected;
      bool subscribed;
      bool saveResize;
      std::vector<Server> serverList;
      void __fastcall logMessage( std::string s );
      void chatClientCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void chatServerCallback( bool err, MinosRPCObj *mro, const std::string &from );
      static void makeRPCObjects();
      void syncChat();
      void syncStations();
   public: 		// User declarations
      __fastcall TMinosChatForm( TComponent* Owner );
      void notifyCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void addChat(const std::string &mess);
      void __fastcall WmMove( TMessage &Msg );
      BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
      END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE TMinosChatForm *MinosChatForm;
//---------------------------------------------------------------------------
#endif
