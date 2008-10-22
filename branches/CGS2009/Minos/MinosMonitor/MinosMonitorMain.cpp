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

#include "base_pch.h"
#include "display_pch.h"
#pragma hdrstop
#include <inifiles.hpp>
#include "mtrace.h"
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
#include "LoggerRPCObj.h"
#include "MinosMonitorMain.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "JvExExtCtrls"
#pragma link "JvNetscapeSplitter"
#pragma resource "*.dfm"
TMonitorMain *MonitorMain = 0;
//---------------------------------------------------------------------------
class MonitorParameters : public MinosParametersAdapter
{
      int p1;
      int p2;
   public:
      MonitorParameters() : p1( 10 ), p2( 60 )
      {}
      ~MonitorParameters()
      {}
      virtual int getStatsPeriod1();
      virtual int getStatsPeriod2();
      virtual void setStatsPeriod1( int );
      virtual void setStatsPeriod2( int );
      virtual bool yesNoMessage( TComponent* Owner, String mess );
      virtual void mshowMessage( String mess, TComponent* Owner = 0 );
      virtual void showContestScore( const std::string &score );
};
static MonitorParameters mp;

int MonitorParameters::getStatsPeriod1()
{
   return p1;
}
int MonitorParameters::getStatsPeriod2()
{
   return p2;
}
void MonitorParameters::setStatsPeriod1( int p )
{
   p1 = p;
}
void MonitorParameters::setStatsPeriod2( int p )
{
   p2 = p;
}
bool MonitorParameters::yesNoMessage( TComponent* Owner, String mess )
{
   return mShowYesNoMessage( Owner, mess );
}
void MonitorParameters::mshowMessage( String mess, TComponent* Owner )
{
   mShowMessage( mess, Owner );
}
void MonitorParameters::showContestScore( const std::string &score )
{
   MonitorMain->ScoreLabel->Caption = score.c_str();
}
//---------------------------------------------------------------------------
/*static*/
void makeRPCObjects()
{
   TRPCFunctor * pscb = new TRPCCallback <TMonitorMain> ( MonitorMain, &TMonitorMain::notifyCallback );
   RPCPubSub::initialisePubSub( pscb );

   TRPCFunctor *lsccb = new TRPCCallback <TMonitorMain> ( MonitorMain, &TMonitorMain::loggerSubscribeClientCallback );
   MinosRPCObj::addObj( new RPCLogSubscribeClient( lsccb ) );
}
//---------------------------------------------------------------------------
void __fastcall TMonitorMain::logMessage( std::string s )
{
   trace( s );
}
//---------------------------------------------------------------------------
__fastcall TMonitorMain::TMonitorMain( TComponent* Owner )
      : TForm( Owner ), subscribed( false ), saveResize( false ), MonitorTreeClickNode( 0 )
{
   enableTrace( ".\\TraceLog\\MinosMonitor_" );
}
//---------------------------------------------------------------------------
__fastcall TMonitorMain::~TMonitorMain( )
{
   /*
      freeAll();           // closes slots - and maybe bundles

      loggerBundle.closeProfile();
      preloadBundle.closeProfile();
      displayBundle.closeProfile();
      contestApp = 0;
   */
   for ( std::vector<MonitoredStation *>::iterator i = stationList.begin(); i != stationList.end(); i++ )
   {
      for ( std::vector<MonitoredLog *>::iterator j = ( *i ) ->slotList.begin(); j != ( *i ) ->slotList.end(); j++ )
      {
         delete ( *j );
      }
      ( *i ) ->slotList.clear();
      delete ( *i );
   }
   stationList.clear();
   delete MultLists::getMultLists();
   CsGuard::ClearDown();
}
//---------------------------------------------------------------------------
void __fastcall TMonitorMain::CloseButtonClick( TObject */*Sender*/ )
{
   Close();                         // and finally close

}
//---------------------------------------------------------------------------
void __fastcall TMonitorMain::FormClose( TObject *Sender,
      TCloseAction &/*Action*/ )
{
   saveResize = false;
   MinosRPCObj::clearRPCObjects();

   XMPPClosedown();
   LogTimerTimer( Sender );
   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::LogTimerTimer( TObject *Sender )
{
   syncStations();
   // check log queue; if anything on it then log to disk
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
         CloseButtonClick( Sender );
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::FormShow( TObject */*Sender*/ )
{
   StartupTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void TMonitorMain::loggerSubscribeClientCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   logMessage( "logger subscribe client callback from " + from + ( err ? ":Error" : ":Normal" ) );
   if ( !err )
   {
      // This will return stanza id, pubname, and stanza content
      boost::shared_ptr<RPCParam> psLogName;
      boost::shared_ptr<RPCParam> psStanzaData;
      boost::shared_ptr<RPCParam> psStanza;
      boost::shared_ptr<RPCParam> psResult;
      RPCArgs *args = mro->getCallArgs();
      if ( args->getStructArgMember( 0, "LogName", psLogName )
           && args->getStructArgMember( 0, "LoggerResult", psResult )
           && args->getStructArgMember( 0, "Stanza", psStanza )
           && args->getStructArgMember( 0, "StanzaData", psStanzaData )
         )
      {
         std::string logName;
         std::string stanzaData;
         bool result;
         int stanza;

         if ( psLogName->getString( logName ) && psStanzaData->getString( stanzaData )
              && psStanza->getInt( stanza ) && psResult->getBoolean( result )
            )
         {
            logMessage( "Name " + logName + " stanza " + makeStr( stanza ) );
            // Find the matching MonitoredLog and send the stanza their for processing
            for ( std::vector<MonitoredStation *>::iterator i = stationList.begin(); i != stationList.end(); i++ )
            {
               // "from" is something like Logger@dev-station
               if ( "Logger@" + ( *i ) ->stationName == from )
               {
                  for ( std::vector<MonitoredLog *>::iterator j = ( *i ) ->slotList.begin(); j != ( *i ) ->slotList.end(); j++ )
                  {
                     if ( ( *j ) ->getPublishedName() == logName.c_str() )
                     {
                        logMessage( "||" + stanzaData + "||" );
                        ( *j ) ->processLogStanza( stanza, stanzaData );
                        return ;
                     }
                  }
               }
            }

         }
      }
   }

}

void __fastcall TMonitorMain::StartupTimerTimer( TObject */*Sender*/ )
{
   StartupTimer->Enabled = false;

   ForceDirectories( ".\\Configuration" );
   std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosMonitor.ini" ) );

   int L = Left, T = Top, W = Width, H = Height;
   L = iniFile->ReadInteger( "Display", "Left", L );
   T = iniFile->ReadInteger( "Display", "Top", T );
   W = iniFile->ReadInteger( "Display", "Width", W );
   H = iniFile->ReadInteger( "Display", "Height", H );
   SetBounds( L, T, W, H );
   Repaint();
   saveResize = true;

   MultLists::getMultLists(); // make sure everything is loaded

   ConnectTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::FormResize( TObject */*Sender*/ )
{
   if ( saveResize )
   {
      std::auto_ptr<TIniFile> iniFile( new TIniFile( "Configuration\\MinosMonitor.ini" ) );
      iniFile->WriteInteger( "Display", "Left", Left );
      iniFile->WriteInteger( "Display", "Top", Top );
      iniFile->WriteInteger( "Display", "Width", Width );
      iniFile->WriteInteger( "Display", "Height", Height );
      iniFile->UpdateFile();
   }
}
void __fastcall TMonitorMain::WmMove( TMessage &/*Msg*/ )
{
   FormResize( this );
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::ConnectTimerTimer( TObject */*Sender*/ )
{
   if ( !connected && checkServerReady() )
   {
      makeRPCObjects();
      XMPPInitialise( "Monitor" );
      connected = true;
      RPCPubSub::subscribe( "LocalStation" );  //want ALL keys
   }
}
//---------------------------------------------------------------------------
bool syncstat = false;


void TMonitorMain::syncStations()
{
   // Here we want to subscribe to the loggers of the notified stations
   // Shouldn't matter if we end up doing it twice

   // Strictly I suppose we could ignore ourselves, but normally we won't run
   // a full monitor on a logging computer - we need a module that can
   // manage single stations for that.

   // And this module must make use of that single station monitor!

   // Probably it is basically the "MonitoredStation" class as a model, with a viewer
   // and maybe a controller...

   CsGuard g;
   if ( syncstat )
   {
      syncstat = false;

      MonitorTree->RootNodeCount = 0;
      MonitorTree->RootNodeCount = stationList.size();
      //MonitorTree->InvalidateNode(0);
      MonitorTree->ValidateNode( 0, true );
      MonitorTree->FullExpand();
   }
}
struct MonitoredStationCmp
{
   std::string cmpstr;
   MonitoredStationCmp( const std::string &s ) : cmpstr( s )
   {}

   bool operator() ( MonitoredStation * &s1 ) const
   {
      return stricmp( s1->stationName.c_str(), cmpstr.c_str() ) == 0;
   }
};
struct MonitoredLogCmp
{
   std::string cmpstr;
   MonitoredLogCmp( const std::string &s ) : cmpstr( s )
   {}

   bool operator() ( MonitoredLog * &s1 ) const
   {
      return stricmp( s1->getPublishedName().c_str(), cmpstr.c_str() ) == 0;
   }
};
void TMonitorMain::notifyCallback( bool err, MinosRPCObj *mro, const std::string &from )
{
   CsGuard g;
   syncstat = true;
   logMessage( "Notify callback from " + from + ( err ? ":Error" : ":Normal" ) );
   AnalysePubSubNotify an( err, mro );

   if ( an.getOK() )
   {
      PublishState state = an.getState();
      std::string key = an.getKey();
      std::string value = an.getValue();

      if ( an.getCategory() == "LocalStation" )
      {
         // This state better not be anything other than puiblished!
         localServerName = an.getKey();
         RPCPubSub::subscribe( "Station" );  //want ALL keys - but do it once we know who WE are!
      }
      if ( an.getCategory() == "Station" )
      {
         CsGuard g;
         logMessage( "Station " + key + " " + value );
         std::vector<MonitoredStation *>::iterator stat = std::find_if( stationList.begin(), stationList.end(), MonitoredStationCmp( key ) );

         if (state != psRevoked)
         {
            if ( stat == stationList.end() )
            {
               MonitoredStation * ms = new MonitoredStation();
               ms->stationName = key;
               ms->state = state;
               stationList.push_back( ms );
               RPCPubSub::subscribeRemote( key, "ContestLog" );
            }
            else
            {
               (*stat)->state = state;
            }
         }
         else
         {
            (*stat)->state = state;
         }
      }
      if ( an.getCategory() == "ContestLog" )
      {
         CsGuard g;
         std::string server = an.getServer();
         if ( server.size() == 0 )
         {
            // it is for us...
            server = localServerName;
         }

         std::string logval = server + " : " + key ;
         logMessage( "ContestLog " + logval + " " + value );

         std::vector<MonitoredStation *>::iterator stat = std::find_if( stationList.begin(), stationList.end(), MonitoredStationCmp( server ) );
         if ( stat != stationList.end() )
         {
            std::vector<MonitoredLog *>::iterator log = std::find_if( ( *stat ) ->slotList.begin(), ( *stat ) ->slotList.end(), MonitoredLogCmp( key ) );
            if (state == psPublished)
            {
               if ( log == ( *stat ) ->slotList.end() )
               {
                  MonitoredLog * ml = new MonitoredLog();
                  ml->initialise( server, key );
                  ml->setExpectedStanzaCount( atoi( value.c_str() ) );
                  ml->setState(state);
                  ( *stat ) ->slotList.push_back( ml );
               }
               else
               {
                  ( *log ) ->setExpectedStanzaCount( atoi( value.c_str() ) );
                  (*log)->setState(state);
               }
            }
            else
            {
               if ( log != ( *stat ) ->slotList.end() )
               {
                  (*log)->setState(state);
               }
            }
         }
      }
   }
}

void TMonitorMain::addSlot( MonitoredLog *ct )
{
   static int namegen = 0;
   TTabSheet *t = new TTabSheet( this );
   String baseFname = ExtractFileName( ct->getPublishedName().c_str() );
   t->Caption = baseFname;

   TMonitoringFrame *f = new TMonitoringFrame( this );
   f->initialise( ct->getContest() );
   f->Name = String( "LogFrame" ) + namegen++;
   f->Parent = t;
   f->Align = alClient;
   f->ParentFont = true;

   t->PageControl = ContestPageControl;
   ContestPageControl->ActivePage = t;
   ct->setFrame( f );
   f->showQSOs();
}

void __fastcall TMonitorMain::MonitorTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = sizeof( int );
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::MonitorTreeInitNode( TBaseVirtualTree *Sender,
      PVirtualNode ParentNode, PVirtualNode Node,
      TVirtualNodeInitStates &InitialStates )
{
   if ( ParentNode == 0 )
   {
      *( int * ) Sender->GetNodeData( Node ) = 0;
   }
   else
   {
      *( int * ) Sender->GetNodeData( Node ) = 1;
   }
   if ( ParentNode == 0 && stationList[ Node->Index ] ->slotList.size() )
   {
      InitialStates << ivsHasChildren;
   }
   else
   {}
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::MonitorTreeInitChildren( TBaseVirtualTree */*Sender*/,
      PVirtualNode Node, DWORD &ChildCount )
{
   ChildCount = stationList[ Node->Index ] ->slotList.size();
}
//---------------------------------------------------------------------------
char *stateList[] =
{
   "P",
   "R",
   "NC"
};
void __fastcall TMonitorMain::MonitorTreeGetText( TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex /*Column*/, TVSTTextType /*TextType*/,
      UnicodeString &CellText )
{
	#warning if the state is not published we need to change the font, or colour
	std::string text;
	if ( *( int * ) Sender->GetNodeData( Node ) == 0 )
	{
		text =  stationList[ Node->Index ] ->stationName + " " + stateList[stationList[ Node->Index ] ->state];
	}
	else
	{
		text = std::string(stateList[stationList[ Node->Parent->Index ] ->slotList[ Node->Index ] ->getState()])
						+ " " + stationList[ Node->Parent->Index ] ->slotList[ Node->Index ] ->getPublishedName();
	}
	CellText = text.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::MonitorTreeDblClick( TObject */*Sender*/ )
{
   // apply double click to node MonitorTreeClickNode
   if (!MonitorTreeClickNode)
   {
      return;
   }
   void *ndata = MonitorTree->GetNodeData( MonitorTreeClickNode );
   if ( *(int *)ndata == 0 )
   {
      // station
   }
   else
   {
      if (MonitorTreeClickNode->Parent->Index > stationList.size() )
      {
         return;
      }
      MonitoredStation *ms = stationList[ MonitorTreeClickNode->Parent->Index ];
      if (MonitorTreeClickNode->Index > ms->slotList.size() )
      {
         return;
      }
      // log
      if (ms->slotList.size() > MonitorTreeClickNode->Index)
      {
         MonitoredLog * ml = ms ->slotList[ MonitorTreeClickNode->Index ];
         if ( !ml->enabled() )
         {
            ml->startMonitor();
            addSlot( ml );
         }
         else
         {
            TTabSheet *cttab = findContestPage( ml->getContest() );
            if ( cttab )
            {
               ContestPageControl->ActivePage = cttab;
            }
         }
      }
   }

}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::MonitorTreeMouseDown( TObject */*Sender*/,
      TMouseButton /*Button*/, TShiftState /*Shift*/, int X, int Y )
{

   PVirtualNode Node = MonitorTree->GetNodeAt( X, Y );
   MonitorTreeClickNode = Node;
}
//---------------------------------------------------------------------------
TMonitoringFrame *TMonitorMain::findCurrentLogFrame()
{
   // we need to find the embedded frame...
   if ( !ContestPageControl->ActivePage )
      return 0;
   int cc = ContestPageControl->ActivePage->ControlCount;
   for ( int i = 0; i < cc; i++ )
   {
      if ( TMonitoringFrame * f = dynamic_cast<TMonitoringFrame *>( ContestPageControl->ActivePage->Controls[ i ] ) )
      {
         return f;
      }
   }
   return 0;
}
TTabSheet *TMonitorMain::findContestPage( BaseContestLog *ct )
{
   // we need to find the embedded frame...
   if ( !ContestPageControl->ActivePage || !ct )
      return 0;
   int pc = ContestPageControl->PageCount;
   for ( int i = 0; i < pc; i++ )
   {
      int cc = ContestPageControl->Pages[ i ] ->ControlCount;
      for ( int j = 0; j < cc; j++ )
      {
         if ( TMonitoringFrame * f = dynamic_cast<TMonitoringFrame *>( ContestPageControl->Pages[ i ] ->Controls[ j ] ) )
         {
            if ( f->getContest() == ct )
               return ContestPageControl->Pages[ i ];
         }
      }
   }
   return 0;
}

bool nolog( MonitoredLog *ip )
{
   if ( ip == 0 )
      return true;
   else
      return false;
}
void __fastcall TMonitorMain::MonitorTimerTimer( TObject */*Sender*/ )
{
   CsGuard g;
   for ( std::vector<MonitoredStation *>::iterator i = stationList.begin(); i != stationList.end(); i++ )
   {
      for ( std::vector<MonitoredLog *>::iterator j = ( *i ) ->slotList.begin(); j != ( *i ) ->slotList.end(); j++ )
      {
         if ((*j)->getState() == psRevoked)
         {
            TTabSheet *cttab = findContestPage( (*j)->getContest() );
            if ( cttab )
            {
               delete cttab;
            }
            // take it out of the slot list and close it
            // and we need to redo the list
            delete(*j);
            (*j) = 0;
            syncstat = true;

         }
         else
         {
            ( *j ) ->checkMonitor();
         }
      }
      if (syncstat)
      {
         ( *i ) ->slotList.erase(remove_if(( *i ) ->slotList.begin(), ( *i ) ->slotList.end(), nolog), ( *i ) ->slotList.end());
         syncStations();
      }
   }
   TMonitoringFrame *f = findCurrentLogFrame();
   std::string statbuf;
   if ( f && f->getContest() )
   {
      f->getContest() ->setScore( statbuf );
   }
   MinosParameters::getMinosParameters() ->showContestScore( statbuf );
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::Close1Click( TObject */*Sender*/ )
{
   //
   TTabSheet * t = ContestPageControl->ActivePage;
   if ( t )
   {
      TMonitoringFrame * f = findCurrentLogFrame();
      BaseContestLog *ct = f->getContest();
      for ( std::vector<MonitoredStation *>::iterator i = stationList.begin(); i != stationList.end(); i++ )
      {
         for ( std::vector<MonitoredLog *>::iterator j = ( *i ) ->slotList.begin(); j != ( *i ) ->slotList.end(); j++ )
         {
            if ( ( *j ) ->getContest() == ct )
            {
               ( *j ) ->stopMonitor();
            }
         }
      }
      t->PageControl = 0;
      delete t;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::Cancel1Click( TObject */*Sender*/ )
{
   // no action needed
}
//---------------------------------------------------------------------------

void __fastcall TMonitorMain::FocusCurrentButtonClick(TObject */*Sender*/)
{
//
   TMonitoringFrame *f = findCurrentLogFrame();
   if (f)
   {
      PVirtualNode l = f->LogMonitorFrame->QSOTree->GetLastChild( f->LogMonitorFrame->QSOTree->RootNode );
      f->LogMonitorFrame->QSOTree->FocusedNode = l;
      f->LogMonitorFrame->QSOTree->Selected[ l ] = true;
   }
}
//---------------------------------------------------------------------------

