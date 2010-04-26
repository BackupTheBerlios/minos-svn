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

void dispatchResponse( RPCDispatcher *RPCDisp, XStanza *xs )
{
   if ( RPCDisp )
   {
      RPCDisp->dispatchResponse( xs );
   }
}
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

/*
<?xml version='1.0'?>
<stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' to='localhost' version='1.0'>*/ 
/*
<iq type='set' id='1'>
   <query xmlns='minos:iq:rpc'>
      <methodCall>
         <methodName>SetFromId</methodName>
         <params><param><value><string>Logger@localhost</string></value></param></params>
      </methodCall>
   </query></iq>
*/

/*
<iq type='set' to='Logger@dev-Station' id='4' from='dev-Station'>
   <query xmlns='minos:iq:rpc'>
      <methodCall>
         <methodName>Minos:PubSub:ClientNotify</methodName>
         <params><param><value><struct><member><name>Server</name><value><string/></value></member><member><name>Category</name><value><string>Keyer</string></value></member><member><name>Key</name><value><string>Report</string></value></member><member><name>Value</name><value><string>emsReplayPip : Pip : [P]</string></value></member></struct></value></param>
         </params>
      </methodCall>
   </query>
</iq>
*/ 
/*
<iq type='result' to='Logger@dev-Station' id='2' from='dev-Station'>
   <query xmlns='minos:iq:rpc'>
      <methodResponse>
         <methodName>Minos:PubSub:Publish</methodName>
         <params><param><value><struct><member><name>PublishResult</name><value><boolean>1</boolean></value></member></struct></value></param></params>
      </methodResponse>
   </query>
</iq>
*/
bool analyseNode( RPCDispatcher *RPCDisp, TiXmlElement *tix )
{
   if ( !tix || !checkElementName( tix, "iq" ) )
   {
      return false;
   }

   std::string from = getAttribute( tix, "from" );
   std::string to = getAttribute( tix, "to" );
   std::string id = getAttribute( tix, "id" );
   std::string subtype = getAttribute( tix, "type" );

   TiXmlElement *query = findNode( tix, "query" );
   if ( !query )
   {
      return false;
   }
   std::string ns = getAttribute( query, "xmlns" );
   if ( ns != "minos:iq:rpc" )
   {
      return false;
   }

   TiXmlElement *mcall = findNode( query, "methodCall" );
   TiXmlElement *mresp = findNode( query, "methodResponse" );

   if ( subtype == "set" && mcall )
   {
      RPCRequest * xs = new RPCRequest( from, mcall );
      xs->setId( id.c_str() );
      dispatchResponse( RPCDisp, xs );
      delete xs;
      return true;
   }
   else
      if ( subtype == "result" && mresp )
      {
         RPCResponse * rr = new RPCResponse( from, mresp );
         rr->setId( id.c_str() );
         dispatchResponse( RPCDisp, rr );
         delete rr;
         return true;
      }
   return false;
}
bool analyseNode( RPCDispatcher *RPCDisp, std::string UTF8XML )
{
   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument xdoc;
   xdoc.Parse( UTF8XML.c_str(), 0 );
   TiXmlElement *tix = xdoc.RootElement();
   bool ret = analyseNode( RPCDisp, tix );
   return ret;

}

