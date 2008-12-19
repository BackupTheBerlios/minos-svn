/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "XMPP_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void makeXMPPEvent( XStanza *xs )
{
   // analyse the stanza and produce the correct event
   if ( RPCRequest * rq = dynamic_cast<RPCRequest *>( xs ) )
   {
      // external RPC request - decode and action
      MinosRPCServer * mro = MinosRPCObj::makeServerObj( rq->methodName );
      if ( mro )
      {
         mro->setCallArgs( *rq );
         mro->id = xs->getId();

         // we should be able to queue response/error response
         // on mro (request) - how?
         if ( mro->callback )
         {
            mro->callback->call( false, mro, rq->getFrom() );
         }
         delete mro;
      }
   }
   else
   {
      if ( RPCResponse * rp = dynamic_cast<RPCResponse *>( xs ) )
      {
         // RPC response where call was not blocking
         MinosRPCClient * mro = MinosRPCObj::makeClientObj( rp->methodName );
         if ( mro )
         {
            mro->id = xs->getId();
            mro->setCallArgs( *rp );
            if ( mro->callback )
            {
               mro->callback->call( false, mro, rp->getFrom() );
            }
            delete mro;
         }
      }
   }
}

