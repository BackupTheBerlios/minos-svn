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
// Basic classes
// base of actions to be passed to XMPP

XStanza::XStanza()
{}
XStanza::~XStanza()
{}
void XStanza::setNextId()
{
   if ( id.size() == 0 )
   {
      id = getNextId();
   }
}
/*static*/ std::string XStanza::getNextId()
{
   static int nextId = 1;
   static char buff[ 40 ] = {0};

   GJV_scoped_lock lck;

   sprintf( buff, "%d", nextId );
   nextId++;

   return buff;
}
//---------------------------------------------------------------------------
// RPC actions. NB that calls, events and responses can come from either end!
//---------------------------------------------------------------------------

// RPC base action
RPCAction::RPCAction()
{}
RPCAction::RPCAction( const std::string &to, const std::string &from ) :
      to( to ), from( from )
{}
RPCAction::~RPCAction()
{
   args.clear();
}
std::string RPCAction::print()
{
   std::string s = "From: " + getFrom() + " To: " + getTo() + "\r\n";
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = args.begin(); i != args.end(); i++ )
   {
      s += ( *i ) ->print();
   }
   return s;
}
void RPCAction::parseParams( TiXmlElement *paramsNode )      // parse from the node to args
{
   std::string s;
   s << *paramsNode ;
   RPCArgs::parseParams( s );
}

//---------------------------------------------------------------------------
// Do an RPC action (normally the receiver!)
TiXmlElement *findNode( TiXmlElement *node, const std::string &name )
{
   for ( TiXmlElement * e = node->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      // declaration, destination lines
      if ( checkElementName( e, name ) )
      {
         return e;
      }
   }
   return 0;
}
std::string getNodeValue( TiXmlElement *node, const std::string &name )
{
   std::string res;
   for ( TiXmlElement * e = node->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      // declaration, destination lines
      if ( checkElementName( e, name ) )
      {
         const char * val = e->GetText();
         if ( val )
         {
            res = val;
         }
         break;
      }
   }
   return res;
}
RPCRequest::RPCRequest( const std::string &to, const std::string &mname ) : RPCAction( to, "" ), methodName( mname )
{}
RPCRequest::RPCRequest( const std::string &to, const std::string &from, const std::string &mname ) : RPCAction( to, from ), methodName( mname )
{}
RPCRequest::RPCRequest( const std::string &from, TiXmlElement *node ) : RPCAction( "", from )
{
   std::string mname = getNodeValue( node, "methodName" );
   methodName = mname;
   TiXmlElement *p = findNode( node, "params" );
   if ( p )
   {
      parseParams( p );
   }
}
RPCRequest::~RPCRequest()
{}

TiXmlElement *makeIq ( const std::string &type, const std::string &xmlns )
{
   TiXmlElement * x = new TiXmlElement( "iq" );
   x->SetAttribute( "type", type );
   TiXmlElement qNode( "query" );
   qNode.SetAttribute( "xmlns", xmlns );

   x->InsertEndChild( qNode );
   return x;
}

// Build up the DOM tree for the action, and send it
std::string RPCRequest::getActionMessage( )
{
   TiXmlElement * x = makeIq( "set", "minos:iq:rpc" );
   if ( to.length() )
      x->SetAttribute( "to", to );
   if ( from.length() )
      x->SetAttribute( "from", from );
   if ( getId().length() )
      x->SetAttribute( "id", getId() );

   TiXmlElement *q = findNode( x, "query" );

   TiXmlElement mcNode( "methodCall" );
   TiXmlElement mNode( "methodName" );

   TiXmlText tNode( methodName );
   mNode.InsertEndChild( tNode );

   mcNode.InsertEndChild( mNode );
   addParams( mcNode );
   q->InsertEndChild( mcNode );

   std::string s;
   s << *x ;
   delete x;
   return s;
}
std::string RPCRequest::print()
{
   std::string s = "Request\r\n";
   s += RPCAction::print();
   return s;
}
std::string RPCRequest::analyse()
{
   std::string s;
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = args.begin(); i != args.end(); i++ )
   {
      s += ( *i ) ->analyse();
   }
   return s;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Do an RPC action (normally the receiver!)
RPCResponse::RPCResponse( const std::string &to, const std::string &pid , const std::string &mname ) :
      RPCAction( to, "" ), methodName( mname )
{
   setId( pid.c_str() );
}
RPCResponse::RPCResponse( const std::string &to, const std::string &from, const std::string &pid , const std::string &mname ) :
      RPCAction( to, from ), methodName( mname )
{
   setId( pid.c_str() );
}
RPCResponse::RPCResponse( const std::string &from, TiXmlElement *node ) :
      RPCAction( "", from )
{
   // node points to methodResponse
   methodName = getNodeValue( node, "methodName" );

   TiXmlElement * p = findNode( node, "params" );
   if ( p )
   {
      parseParams( p );
   }
   else
   {
      TiXmlElement *mf = findNode( node, "fault" );
      if ( mf )
      {
         TiXmlElement * mv = findNode( mf, "value" );
         if ( mv )
         {
            TiXmlElement * ms = findNode( mv, "struct" );
            if ( ms )
            {
               fault = boost::shared_ptr<RPCParam>(new RPCParamStruct( *ms ));
            }
         }
      }
   }
}
RPCResponse::RPCResponse()
{}
RPCResponse::~RPCResponse()
{
}
void RPCResponse::addFault( boost::shared_ptr<RPCParam>p )
{
   fault = p;
}
void RPCResponse::addFault( TiXmlElement &node )
{
   if ( fault )
   {
      TiXmlElement pNode( "fault" );
      fault->addValue( pNode );
      RPCArgs xm;
      xm.addParam( fault );
      TiXmlElement *pnode = xm.makeParamsNode();
      node.InsertEndChild( *pnode );
      delete pnode;
   }
}
// Build up the DOM tree for the action, and send it
std::string RPCResponse::getActionMessage( )
{
   TiXmlElement * x = makeIq( "result", "minos:iq:rpc" );
   if ( to.length() )
      x->SetAttribute( "to", to );
   if ( from.length() )
      x->SetAttribute( "from", from );
   if ( getId().length() )
      x->SetAttribute( "id", getId() );

   TiXmlElement *q = findNode( x, "query" );

   TiXmlElement mrNode( "methodResponse" );
   TiXmlElement mNode( "methodName" );
   TiXmlText tNode( methodName );
   mNode.InsertEndChild( tNode );
   mrNode.InsertEndChild( mNode );

   if ( fault )
      addFault( mrNode );
   else
      addParams( mrNode );
   q->InsertEndChild( mrNode );

   std::string s;
   s << *x;
   delete x;
   return s;
}
std::string RPCResponse::print()
{
   std::string s = "Response\r\n";
   if ( fault )
      s += fault->print();

   s += RPCAction::print();
   return s;
}
//---------------------------------------------------------------------------


