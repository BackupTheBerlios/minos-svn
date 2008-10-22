/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef LoggerRPCObjH
#define LoggerRPCObjH

class RPCLogSubscribeClient: public MinosRPCClient
{
   public:
      RPCLogSubscribeClient( TRPCFunctor *cb ) : MinosRPCClient( "Minos:Logger:GetStanza", cb )
      {}
      ~RPCLogSubscribeClient()
      {}
      virtual RPCLogSubscribeClient *makeObj()
      {
         return new RPCLogSubscribeClient( callback );
      }
};
class RPCLogSubscribeServer: public MinosRPCServer
{
   public:
      RPCLogSubscribeServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:Logger:GetStanza", cb )
      {}
      ~RPCLogSubscribeServer()
      {}
      virtual RPCLogSubscribeServer *makeObj()
      {
         return new RPCLogSubscribeServer( callback );
      }

};
#endif
