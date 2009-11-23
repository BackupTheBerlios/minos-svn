/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef BandMapRPCObjH
#define BandMapRPCObjH 
//---------------------------------------------------------------------------
class RPCBandMapClient: public MinosRPCClient
{
   public:
      RPCBandMapClient( TRPCFunctor *cb ) : MinosRPCClient( "Minos:BandMap", cb )
      {}
      ~RPCBandMapClient()
      {}
      virtual RPCBandMapClient *makeObj()
      {
         return new RPCBandMapClient( callback );
      }
};
class RPCBandMapServer: public MinosRPCServer
{
   public:
      RPCBandMapServer( TRPCFunctor *cb ) : MinosRPCServer( "Minos:BandMap", cb )
      {}
      ~RPCBandMapServer()
      {}
      virtual RPCBandMapServer *makeObj()
      {
         return new RPCBandMapServer( callback );
      }

};
#endif
