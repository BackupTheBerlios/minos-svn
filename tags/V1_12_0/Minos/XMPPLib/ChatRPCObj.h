//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ChatRPCObjH
#define ChatRPCObjH 
//---------------------------------------------------------------------------
class RPCChatControlClient: public MinosRPCClient
{
   public:
      RPCChatControlClient( TRPCFunctor *cb ) : MinosRPCClient( "Minos:Chat", cb )
      {}
      ~RPCChatControlClient()
      {}
      virtual RPCChatControlClient *makeObj()
      {
         return new RPCChatControlClient( callback );
      }
};
class RPCChatControlServer: public MinosRPCServer
{
   public:
      RPCChatControlServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:Chat", cb )
      {}
      ~RPCChatControlServer()
      {}
      virtual RPCChatControlServer *makeObj()
      {
         return new RPCChatControlServer( callback );
      }

};
#endif
