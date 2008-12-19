/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ControlRPCObjH
#define ControlRPCObjH 
//---------------------------------------------------------------------------
class RPCControlClient: public MinosRPCClient
{
   public:
      RPCControlClient( TRPCFunctor *cb ) : MinosRPCClient( "Minos:Control", cb )
      {}
      ~RPCControlClient()
      {}
      virtual RPCControlClient *makeObj()
      {
         return new RPCControlClient( callback );
      }
};
class RPCControlServer: public MinosRPCServer
{
   public:
      RPCControlServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:Control", cb )
      {}
      ~RPCControlServer()
      {}
      virtual RPCControlServer *makeObj()
      {
         return new RPCControlServer( callback );
      }

};
//---------------------------------------------------------------------------
#endif
