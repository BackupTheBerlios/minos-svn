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

#ifndef MServerH
#define MServerH 
//---------------------------------------------------------------------------
#define DEFAULT_SERVER_NAME "localhost"

class MinosServer
{
   private:
      static MinosServer *singleton;
      std::string serverName;
   public:
      std::string getServerName()
      {
         return serverName;
      }

      static MinosServer *getMinosServer();
      bool forwardStanza( MinosCommonConnection *, TiXmlElement *pak );
      virtual bool analyseNode( MinosCommonConnection *il, TiXmlElement * pak );
      virtual void dispatchStanza( MinosCommonConnection *il, RPCRequest *a );
      virtual void dispatchStanza( MinosCommonConnection *il, RPCResponse *a );
      //      virtual void dispatchStanza( MinosCommonConnection *il, RPCEvent *a );

      MinosServer();
      ~MinosServer();
};
#endif
