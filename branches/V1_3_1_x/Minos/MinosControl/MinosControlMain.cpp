//---------------------------------------------------------------------------
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

#define NO_WIN32_LEAN_AND_MEAN
#include "MinosControl_pch.h"
#pragma hdrstop

#include "ServerEvent.h"
#include "GJVThreads.h"
#include "LogEvents.h"
#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "Dispatcher.h"
#include "MinosThread.h"
#include "XMPPEvents.h"
#include "XMPPRPCObj.h"
#include "RPCPubSub.h"
#include "ControlRPCObj.h"

#include "CommonMonitor.h"
#include "controlport.h"
#include "portconf.h"
#include "MinosControlMain.h"
#include "LineEvents.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMinosControlForm *MinosControlForm = 0;
CRITICAL_SECTION disableInterrupts::intCriticalSection;
//---------------------------------------------------------------------------
/*static*/
void TMinosControlForm::makeRPCObjects()
{
   //RPCPubSub::initialisePubSub( 0 );   // we don't subscribe, so no callback needed - and now we don't publish either
   TRPCCallback<TMinosControlForm> *rcb = new TRPCCallback <TMinosControlForm> ( MinosControlForm, &TMinosControlForm::controlClientCallback );
   MinosRPCClient *mro = new RPCControlClient( rcb );
   MinosRPCObj::addObj( mro );
   MinosRPCObj::addObj( new RPCControlServer( new TRPCCallback <TMinosControlForm> ( MinosControlForm, &TMinosControlForm::controlServerCallback ) ) );
}
//---------------------------------------------------------------------------
void __fastcall TMinosControlForm::logMessage( std::string s )
{
   trace( s );
}
//---------------------------------------------------------------------------
void LineLog( const std::string &msg )
{
   trace( msg );
}
__fastcall TMinosControlForm::TMinosControlForm( TComponent* Owner )
      : TForm( Owner ), connected( false ), controlLineEventThread( 0 )
{
   disableInterrupts::initialise();
   enableTrace( ".\\TraceLog\\MinosControl_" );
   LineSet *ls = LineSet::GetLineSet();
   ls->lsLog = LineLog;
}
//---------------------------------------------------------------------------
void __fastcall TMinosControlForm::FormClose( TObject *Sender,
      TCloseAction &/*Action*/ )
{
   closing = true;

   // we need to close down all the ports
   monitor.closeDown();

   controlLineEventThread->GJV_join();
   delete controlLineEventThread;

   LineSet *ls = LineSet::GetLineSet();
   delete ls;

   MinosRPCObj::clearRPCObjects();
   XMPPClosedown();

   LogTimerTimer( Sender );
   disableTrace();

   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
   disableInterrupts::terminate();
   CsGuard::ClearDown();
}
//---------------------------------------------------------------------------

void __fastcall TMinosControlForm::LogTimerTimer( TObject */*Sender*/ )
{
   bool show = getShowServers();
   if ( !Application->MainForm->Visible && show )
   {
      Application->MainForm->Visible = true;
   }
   if ( Application->MainForm->Visible && !show )
   {
      Application->MainForm->Visible = false;
   }
   // check log queue; if anything on it then log to main window
   while ( true )
   {
      LogEvent * ev = deQueueLog();
      if ( ev )
      {
         logMessage( "Log : " + ev->eMessageType + " " + ev->eMessage );
         delete ev;
      }
      else
         break;
   }

   static bool closed = false;
   if ( !closed )
   {
      if ( checkCloseEvent() )
      {
         closed = true;
         Close();
      }
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TMinosControlForm::ConnectTimerTimer( TObject */*Sender*/ )
{
   if ( !connected && checkServerReady() )
   {
      ConnectTimer->Enabled = false;
      makeRPCObjects();
      XMPPInitialise( "Control" );
      connected = true;
      ConnectTimer->Enabled = true;
   }
}
//---------------------------------------------------------------------------

void TMinosControlForm::controlClientCallback( bool /*err*/, MinosRPCObj */*mro*/, const std::string &/*from*/ )
{
   // call back says OK/not OK
}
//---------------------------------------------------------------------------
void TMinosControlForm::controlServerCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   trace( "control callback from " + from + ( err ? ":Error" : ":Normal" ) );

   if ( !err )
   {
      boost::shared_ptr<RPCParam> psName;
      boost::shared_ptr<RPCParam>psLine;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "Name", psName ) && args->getStructArgMember( 0, "Line", psLine ) )
      {
         std::string Name;
         std::string Line;

         if ( psName->getString( Name ) && psLine->getString( Line ) )
         {
            boost::shared_ptr<RPCParam>st(new RPCParamStruct);

            if ( Name == "GetLine" )
            {
               // action the message
               // can be set line, get line
               // also want to allow integer valued commands and responses
               // to allow a group of lines to be set (e.g. a BCD control output)

               // get commands need a response, set commands are true/false
               // to describe if the line was recognised or not.

               // may be good to allow bundled set/get as well - especially if
               // we can pass the bundle down further

               // Do we also want to have subscribe option, so that changes are
               // forced out? Probably... But we would need to mark the
               // lines(s) as subscription lines in the configuration

               commonLineControl *l = monitor.findLine( Line, true );   // input line
               if ( l )
               {
                  st->addMember( l->getState(), "LineState" );

               }
               else
               {
                  st->addMember( false, "ControlResult" );
               }

            }
            else
               if ( Name == "SetLine" )
               {
                  boost::shared_ptr<RPCParam> psState;
                  if ( args->getStructArgMember( 0, "State", psState ) )
                  {
                     bool state;
                     psState->getBoolean( state );
                     // action the message
                     // can be set line, get line
                     // also want to allow integer valued commands and responses
                     // to allow a group of lines to be set (e.g. a BCD control output)

                     // get commands need a response, set commands are true/false
                     // to describe if the line was recognised or not.

                     // may be good to allow bundled set/get as well - especially if
                     // we can pass the bundle down further

                     // Do we also want to have subscribe option, so that changes are
                     // forced out? Probably... But we would need to mark the
                     // lines(s) as subscription lines in the configuration

                     commonLineControl *l = monitor.findLine( Line, false );   // output line
                     if ( l )
                     {
                        l->setState( state );
                        st->addMember( true, "ControlResult" );

                     }
                     else
                     {
                        st->addMember( false, "ControlResult" );
                     }
                  }

               }
            mro->clearCallArgs();
            mro->getCallArgs() ->addParam( st );
            mro->queueResponse( from );
         }
      }
   }
}
//---------------------------------------------------------------------------
void TMinosControlForm::doProcLineEvents( )
{
   LineSet * ls = LineSet::GetLineSet();
   while ( !closing && !checkCloseEvent() )
   {
      ls->waitForChange( 500 );

      ls->readLines();
      // check on the input lines - PTTOut and Key
      commonLineControl *l = monitor.findLine( "PTTOut", false );   // output line
      if ( l )
      {
         l->setState( ls->getState( "PTTOut" ) );
      }
      l = monitor.findLine( "Key", false );   // output line
      if ( l )
      {
         l->setState( ls->getState( "Key" ) );
      }
   }
}
void procLineEvents( void *p )
{
   //---- Place thread code here ----
   TMinosControlForm * f = ( TMinosControlForm* ) p;
   f->doProcLineEvents();
}

void __fastcall TMinosControlForm::FormShow( TObject */*Sender*/ )
{
	static bool shown = false;
	if (!shown)
	{
		shown = true;
		configurePorts( monitor );
		// and now we wish to set up our line monitor(s) and attach their ports
		monitor.initialise();

		controlLineEventThread = new GJV_thread( "controlLineEventThread", procLineEvents, ( void * ) this, true ) ;
		ConnectTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void setLines( bool PTTOut, bool PTTIn, bool L1, bool L2 )
{
   // This ought to be synchronised...
   MinosControlForm->PTTOutCheckBox->Checked = PTTOut;
   MinosControlForm->PTTInCheckBox->Checked = PTTIn;
   MinosControlForm->L1CheckBox->Checked = L1;
   MinosControlForm->L2CheckBox->Checked = L2;
}
//---------------------------------------------------------------------------


