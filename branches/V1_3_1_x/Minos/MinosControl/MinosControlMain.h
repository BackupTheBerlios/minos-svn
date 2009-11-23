/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosControlMainH
#define MinosControlMainH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class MinosRPCObj;
class TMinosControlForm : public TForm
{
   __published: 	// IDE-managed Components
      TTimer *LogTimer;
      TTimer *ConnectTimer;
      TCheckBox *PTTInCheckBox;
      TCheckBox *L1CheckBox;
      TCheckBox *L2CheckBox;
      TCheckBox *PTTOutCheckBox;
      void __fastcall LogTimerTimer( TObject *Sender );
      void __fastcall ConnectTimerTimer( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall FormShow( TObject *Sender );
   private: 	// User declarations
      GJV_thread *controlLineEventThread;
      bool connected;
      bool closing;
      std::vector<std::string> serverList;
      void __fastcall logMessage( std::string s );
      void controlClientCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void controlServerCallback( bool err, MinosRPCObj *mro, const std::string &from );
      static void makeRPCObjects();

   public: 		// User declarations
      __fastcall TMinosControlForm( TComponent* Owner );
      commonController monitor;
      void doProcLineEvents( );

};
//---------------------------------------------------------------------------
extern PACKAGE TMinosControlForm *MinosControlForm;
//---------------------------------------------------------------------------
#endif
