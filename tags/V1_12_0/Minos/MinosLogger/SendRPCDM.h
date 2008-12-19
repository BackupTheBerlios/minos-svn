/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef SendRPCDMH
#define SendRPCDMH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
class MinosRPCObj;
//---------------------------------------------------------------------------
class TSendDM : public TDataModule
{
   __published:  	// IDE-managed Components
      TTimer *ConnectTimer;
      TTimer *LogTimer;
      void __fastcall ConnectTimerTimer( TObject *Sender );
      void __fastcall LogTimerTimer( TObject *Sender );
   private:  	// User declarations
      bool connected;
      bool subscribed;
      void __fastcall logMessage( std::string s );
      static void makeRPCObjects();
      void notifyCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void bandMapClientCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void bandMapServerCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void keyerCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void zconfCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void loggerServerCallback( bool err, MinosRPCObj *mro, const std::string &from );
   public:  		// User declarations
      __fastcall TSendDM( TComponent* Owner );
      __fastcall ~TSendDM();
      static void subscribeAll();
      static void sendKeyerPlay( int fno );
      static void sendKeyerRecord( int fno );
      static void sendBandMap( const std::string &freq, const std::string &call, const std::string &utc, const std::string &loc, const std::string &qth );
};
//---------------------------------------------------------------------------
extern PACKAGE TSendDM *SendDM;
//---------------------------------------------------------------------------
#endif
