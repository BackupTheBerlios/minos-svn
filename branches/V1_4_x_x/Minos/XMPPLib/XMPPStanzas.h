/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef XMPPStanzasH
#define XMPPStanzasH 
//---------------------------------------------------------------------------
// Basic classes
// base of actions to be passed to XMPP


class XStanza
{
      std::string id;
   public:

      XStanza();
      virtual ~XStanza();

      virtual void setId( const char * tid )
      {
         if ( tid )
            id = tid;
      }
      virtual std::string getId()
      {
         return id;
      }
      void setNextId();
      static std::string getNextId();

      virtual std::string getTo() = 0;
      virtual std::string getFrom() = 0;

      // Build up the DOM tree for the action, and send it
      virtual std::string getActionMessage() = 0;

      virtual std::string print() = 0;
      virtual std::string analyse() = 0;
};
//---------------------------------------------------------------------------
// RPC actions. NB that calls, events and responses can come from either end!

// RPC base action
class RPCAction: public XStanza, public RPCArgs
{
   protected:
      void parseParams( TiXmlElement *paramsNode );     // parse from the node to args
      std::string to;
      std::string from;
   public:

      std::string getTo()
      {
         return to;
      }
      std::string getFrom()
      {
         return from;
      }

      RPCAction();
      RPCAction( const std::string &to, const std::string &from );
      virtual ~RPCAction();
      // Build up the DOM tree for the action, and send it
      virtual std::string getActionMessage() = 0;
      virtual std::string print() = 0;
      virtual std::string analyse()
      {
         return "";
      }
};
// Do an RPC action (normally the receiver!)
class RPCRequest: public RPCAction
{
   public:
      std::string methodName;
      RPCRequest( const std::string &to, const std::string &mname );
      RPCRequest( const std::string &to, const std::string &from, const std::string &mname );
      RPCRequest( const std::string &from, TiXmlElement *node );
      virtual ~RPCRequest();
      // Build up the DOM tree for the action, and send it
      virtual std::string getActionMessage( );
      virtual std::string print();
      virtual std::string analyse();
};

// Do an RPC action (normally the receiver!)
class RPCResponse: public RPCAction
{
   public:
      std::string methodName;
      boost::shared_ptr<RPCParam> fault;

      RPCResponse( const std::string &to, const std::string &id, const std::string &mname );
      RPCResponse( const std::string &to, const std::string &from, const std::string &id, const std::string &mname );
      RPCResponse( const std::string &from, TiXmlElement *node );
      RPCResponse();

      void addFault( boost::shared_ptr<RPCParam> );
      void addFault( TiXmlElement & );
      virtual ~RPCResponse();
      // Build up the DOM tree for the action, and send it
      virtual std::string getActionMessage( );

      virtual std::string print();
      virtual std::string analyse()
      {
         return "";
      }

};
//---------------------------------------------------------------------------
#endif
