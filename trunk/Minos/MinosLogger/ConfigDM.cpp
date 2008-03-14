/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------

#include "base_pch.h"
#pragma hdrstop

#include <inifiles.hpp>
#include "ConfigDM.h"
#include "GuardianThread.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIniFile *config = 0;
bool terminated = false;
/*static*/
TMConfigDM *TMConfigDM::thisDM = 0;
/*static*/
TMConfigDM *TMConfigDM::getConfigDM( TComponent* Owner )
{
   if ( !thisDM )
   {
      thisDM = new TMConfigDM( Owner );
   }
   return thisDM;
}
//---------------------------------------------------------------------------
TConfigElement::TConfigElement()
      : run( false ), hide( false ), test( false ), minimise( false )
{}
//---------------------------------------------------------------------------
bool TConfigElement::initialise( String sect )
{
   if ( config )
   {
      name = sect;
      std::string noprog = "nOpRoGrAm";
      commandLine = config->ReadString( sect, "program", noprog.c_str() ).c_str();
      params = config->ReadString( sect, "params", "" ).c_str();
      if ( commandLine == noprog )
      {
         //      ConfigForm->logMessage( ("No program configured ([" + sect + "]program=xxxx)").c_str() );
         return false;
      }
      String S = UpperCase( config->ReadString( sect, "Run", "Yes" ) ).Trim();
      run = !( S[ 1 ] == 'N' || S[ 1 ] == 'F' || S[ 1 ] == '0' );

      if ( S[ 1 ] == 'T' )
      {
         run = true;
         test = true;
         minimise = true;
      }

      rundir = config->ReadString( sect, "Directory", "" ).Trim().c_str();
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
__fastcall TMConfigDM::TMConfigDM( TComponent* Owner )
      : TDataModule( Owner )
{
   if ( FileExists( ".\\Configuration\\MinosConfig.ini" ) )
   {
      TStringList * lsect = new TStringList;
      config = new TIniFile( ".\\Configuration\\MinosConfig.ini" );
      config->ReadSections( lsect );

      for ( int i = 0; i < lsect->Count; i++ )
      {
         String sect = lsect->Strings[ i ];
         if ( sect.UpperCase() == "CIRCLE OF HELL" )
         {
            getCircleOfHell();
         }
         else
         {
            TConfigElement *tce = new TConfigElement();
            if ( tce->initialise( sect ) )
            {
               elelist.push_back( tce );
            }
            else
               delete tce;
         }
      }
   }
   else
      config = 0;
}
//---------------------------------------------------------------------------
__fastcall TMConfigDM::~TMConfigDM()
{
   if ( !terminated )
      stop();

   for ( std::vector <TConfigElement *>::iterator i = elelist.begin(); i != elelist.end(); i++ )
   {
      delete ( *i );
   }
   elelist.clear();
   // and tidy up all loose ends
   GJV_scoped_lock::ClearDown();
}
//---------------------------------------------------------------------------
void GuardianThreadExecute( void *arg )
{
   TConfigElement * s = ( TConfigElement * ) arg;
   String app = s->name;
   TGuardianThread *guardth = new TGuardianThread( s );
   while ( !terminated )
   {
      guardth->execute();  // one pass at a time...
      if ( terminated )
         break;
      Sleep( 1000 );
   }
   delete guardth;
}
//---------------------------------------------------------------------------
void TMConfigDM::start()
{
   if ( !config )
   {
      return ;
   }
   terminated = false;
   resetCloseEvent();

   for ( std::vector <TConfigElement *>::iterator i = elelist.begin(); i != elelist.end(); i++ )
   {
      if ( ( *i ) ->run )
      {
         GJV_thread * GThreadObj = new GJV_thread( ( ( *i ) ->name + " Guardian" ).c_str(), &GuardianThreadExecute, ( *i ) );
         guardv.push_back( GThreadObj );
      }
   }
}
void TMConfigDM::stop()
{
   if ( !config )
   {
      return ;
   }
   terminated = true;
   signalCloseEvent();

   for ( std::vector <GJV_thread *> ::iterator i = guardv.begin(); i != guardv.end(); i++ )
   {
      if ( ( *i ) )
      {
         logMessage( "Joining Thread", "" );
         ( *i ) ->GJV_join();
         logMessage( "Joined Thread", "" );
         delete ( *i );
      }
   }
   guardv.clear();

}
void TMConfigDM::setCircleOfHell( const std::string &circle )
{
   if ( !config )
   {
      return ;
   }
   circleOfHell = circle;
   config->WriteString( "Circle Of Hell", "Name", circleOfHell.c_str() );
}
std::string TMConfigDM::getCircleOfHell()
{
   if ( !config )
   {
      return "";
   }
   circleOfHell = config->ReadString( "Circle Of Hell", "Name", "" ).Trim().c_str();
   return circleOfHell;
}

