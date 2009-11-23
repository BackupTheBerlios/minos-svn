/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef KeyerRPCServerH
#define KeyerRPCServerH 
//---------------------------------------------------------------------------
class MinosRPCObj;
class KeyerServer
{
   private:
      std::map<std::string, bool> lineStates;
      bool connected;
      bool subscribed;
      void doCheckConnection();
      void keyerCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void notifyCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void doPublishState( const std::string &state );
      void doPublishCommand( const std::string &cmd );
   public:
      KeyerServer();
      ~KeyerServer();
      static void checkConnection();
      static void publishState( const std::string &state );
      static void publishCommand( const std::string &cmd );
      bool getState( const std::string &line );
};
extern KeyerServer *KS;
#endif
