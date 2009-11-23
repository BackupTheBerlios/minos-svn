/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MServerPubSubH
#define MServerPubSubH 
//---------------------------------------------------------------------------
class MinosRPCObj;
class TPubSubMain
{
   private:  	// User declarations
      void logMessage( std::string s );
   public:  		// User declarations
      TPubSubMain( );
      ~TPubSubMain( );
      void publishCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void subscribeCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void remoteSubscribeCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void serverSubscribeCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void notifyCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void serverNotifyCallback( bool err, MinosRPCObj *mro, const std::string &from );

      bool publish( const std::string &pubId, const std::string &category, const std::string &key, const std::string &value, PublishState state );
      bool serverPublish( const std::string &pubId, const std::string &svr, const std::string &category, const std::string &key, const std::string &value, PublishState state );
      bool analyseNode( TiXmlElement *pak );

      void revokeClient(const std::string &pubId);
      void disconnectServer(const std::string &pubId);

};
extern TPubSubMain *PubSubMain;
#endif
