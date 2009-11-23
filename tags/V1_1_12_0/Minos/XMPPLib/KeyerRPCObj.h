/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef KeyerRPCObjH
#define KeyerRPCObjH 
//---------------------------------------------------------------------------
class RPCKeyerControlClient: public MinosRPCClient
{
   public:
      RPCKeyerControlClient( TRPCFunctor *cb ) : MinosRPCClient( "Minos:KeyerControl", cb )
      {}
      ~RPCKeyerControlClient()
      {}
      virtual RPCKeyerControlClient *makeObj()
      {
         return new RPCKeyerControlClient( callback );
      }
};
class RPCKeyerControlServer: public MinosRPCServer
{
   public:
      RPCKeyerControlServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:KeyerControl", cb )
      {}
      ~RPCKeyerControlServer()
      {}
      virtual RPCKeyerControlServer *makeObj()
      {
         return new RPCKeyerControlServer( callback );
      }

};
#endif
