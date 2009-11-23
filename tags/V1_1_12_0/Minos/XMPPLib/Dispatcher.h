/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef DispatcherH
#define DispatcherH

class TiXmlElement;
class XStanza;
class RPCDispatcher
{
   public:
      RPCDispatcher()
      {}
      virtual ~RPCDispatcher()
      {}
      virtual void dispatchResponse( XStanza *a ) = 0;

};
extern bool analyseNode( RPCDispatcher *user_data, TiXmlElement *e );
extern bool analyseNode( RPCDispatcher *user_data, std::string UTF8XML );
TiXmlElement *findNode( TiXmlElement *node, const std::string &name );
std::string getNodeValue( TiXmlElement *node, const std::string &name );
std::string getAttribute( TiXmlElement *tix, const char *attname );
#endif
