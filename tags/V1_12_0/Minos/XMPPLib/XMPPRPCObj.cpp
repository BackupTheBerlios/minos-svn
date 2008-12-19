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

extern void sendAction( XStanza *a );      // which might not be the one in XMPPThread

//---------------------------------------------------------------------------

#pragma package(smart_init)
std::map <std::string, MinosRPCServer *> &getServerMethodMap()
{
   static std::map <std::string, MinosRPCServer *> serverMethodMap;
   return serverMethodMap;
}
std::map <std::string, MinosRPCClient *> &getClientMethodMap()
{
   static std::map <std::string, MinosRPCClient *> clientMethodMap;
   return clientMethodMap;
}
//==============================================================================
MinosRPCObj::MinosRPCObj( const std::string &methodName, TRPCFunctor *cb )
      : methodName( methodName ), callback( cb )
{}
MinosRPCObj::~MinosRPCObj()
{
   //delete callArgs;
}
void MinosRPCObj::clearCallArgs()
{
   callArgs.args.clear();  // clears shared pointers
}
/*static*/ void MinosRPCObj::clearRPCObjects()
{
   for ( std::map <std::string, MinosRPCServer *>::iterator i = getServerMethodMap().begin(); i != getServerMethodMap().end(); i++ )
   {
      delete ( *i ).second->callback;
      delete ( *i ).second;
   }
   getServerMethodMap().clear();
   for ( std::map <std::string, MinosRPCClient *>::iterator i = getClientMethodMap().begin(); i != getClientMethodMap().end(); i++ )
   {
      delete ( *i ).second->callback;
      delete ( *i ).second;
   }
   getClientMethodMap().clear();
}

/*static*/ void MinosRPCObj::addObj(  MinosRPCClient *mro )
{
   getClientMethodMap().insert( std::pair <std::string, MinosRPCClient *>( mro->methodName, mro ) );
}
/*static*/ void MinosRPCObj::addObj(  MinosRPCServer *mro )
{
   getServerMethodMap().insert( std::pair <std::string, MinosRPCServer *>( mro->methodName, mro ) );
}

/*static*/ MinosRPCClient *MinosRPCObj::makeClientObj(  std::string call )
{
   std::map <std::string, MinosRPCClient *>::iterator mo = getClientMethodMap().find( call );
   if ( mo != getClientMethodMap().end() )
      return ( *mo ).second->makeObj();
   return 0;
}

/*static*/ MinosRPCServer *MinosRPCObj::makeServerObj(  std::string call )
{
   std::map <std::string, MinosRPCServer *>::iterator mo = getServerMethodMap().find( call );
   if ( mo != getServerMethodMap().end() )
      return ( *mo ).second->makeObj();
   return 0;
}
//==============================================================================
void MinosRPCClient::queueCall( std::string to )
{
   RPCRequest * m = new RPCRequest( to, methodName );
   m->setNextId();      // only happens if no Id already
   m->args = callArgs.args;     // copy vector of pointers
   sendAction( m );
   delete m;
}
//==============================================================================
// server...
void MinosRPCServer::queueResponse( std::string to )
{
   RPCResponse * m = new RPCResponse( to, id, methodName );
   m->args = callArgs.args;     // copy vector of pointers
   sendAction( m );
   delete m;
}
void MinosRPCServer::queueErrorResponse( std::string to )
{
   RPCResponse * m = new RPCResponse( to, id, methodName );
   sendAction( m );
   delete m;
}
//==============================================================================

