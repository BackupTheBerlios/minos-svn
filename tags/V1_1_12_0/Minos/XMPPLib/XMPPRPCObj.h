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

#ifndef XMPPRPCObjH
#define XMPPRPCObjH 
//---------------------------------------------------------------------------
#include <string>
#include <map> 
//---------------------------------------------------------------------------

// We need a factory for MinosRPCObj descendents, so that we can register those we
// are interested in, and when a call is received those we know are manufactured
// but those we don't know are generically rejected

class MinosRPCObj;

// abstract base class
class TRPCFunctor
{
   public:

      // two possible functions to call member function. virtual cause derived
      // classes will use a pointer to an object and a pointer to a member function
      // to make the function call
      virtual void call( bool err, MinosRPCObj *mro, const std::string &from ) = 0;        // call using function
};


// derived template class
template <class TClass>
class TRPCCallback : public TRPCFunctor
{
   private:
      void ( TClass::*fpt ) ( bool err, MinosRPCObj *mro, const std::string &from );   // pointer to member function
      TClass* pt2Object;                  // pointer to object

   public:

      // constructor - takes pointer to an object and pointer to a member and stores
      // them in two private variables
      TRPCCallback( TClass* _pt2Object, void( TClass::*_fpt ) ( bool err, MinosRPCObj *mro, const std::string &from ) )
      {
         pt2Object = _pt2Object;
         fpt = _fpt;
      };

      // override function "Call"
      virtual void call( bool err, MinosRPCObj *mro, const std::string &from )
      {
         ( *pt2Object.*fpt ) ( err, mro, from );
      }
      ;             // execute member function
};


class MinosRPCClient;
class MinosRPCServer;
class MinosRPCObj
{
      // Base object for Minos RPC calls. It encapsulates both ends of
      // the call
   private:
      //      static std::map <std::string, MinosRPCServer *> serverMethodMap;
      //      static std::map <std::string, MinosRPCClient *> clientMethodMap;

   protected:
      std::string methodName;
      RPCArgs callArgs;

   public:
      RPCArgs *getCallArgs()
      {
         return & callArgs;
      }
      void setCallArgs( RPCArgs &r )
      {
         callArgs = r;
      }
      void clearCallArgs();

      std::string id;
      TRPCFunctor *callback;

      MinosRPCObj( const std::string &methodName, TRPCFunctor *cb );
      virtual ~MinosRPCObj();
      static void clearRPCObjects();

      virtual MinosRPCObj *makeObj() = 0;

      static MinosRPCClient *makeClientObj( std::string call );
      static MinosRPCServer *makeServerObj( std::string call );

      static void addObj( MinosRPCClient *mro );
      static void addObj( MinosRPCServer *mro );

      virtual void queueCall( std::string to )
      {}

      // Invalid call in some way; queue an errror response
      virtual void queueErrorResponse( std::string to )
      {}

      // Valid response; queue the response
      virtual void queueResponse( std::string to )
      {}
}
;

// I think we need master and slave ends...  how do we do this?
// Do we go for mixins, one for each type?

class MinosRPCClient: public MinosRPCObj
{
   private:
      MinosRPCClient();            // don't allow default constructor
   public:
      MinosRPCClient( const std::string &methodName, TRPCFunctor *cb ) : MinosRPCObj( methodName, cb )
      {}
      virtual ~MinosRPCClient()
      {}

      virtual MinosRPCClient *makeObj() = 0;

      // call queueCall to make initial call;
      // This will construct a BlockingRequest to allow the eventual callback
      // actually, NOT a Blocking Request, but something similar
      // we don't wait on it, but we do queue it, and on response arriving
      // we queue the desired response

      virtual void queueCall( std::string to );
};
class MinosRPCServer: public MinosRPCObj
{
   private:
      MinosRPCServer();            // don't allow default constructor
   public:
      MinosRPCServer( const std::string &methodName, TRPCFunctor *cb ) : MinosRPCObj( methodName, cb )
      {}
      virtual ~MinosRPCServer()
      {}

      virtual MinosRPCServer *makeObj() = 0;

      // Invalid call in some way; queue an errror response
      virtual void queueErrorResponse( std::string to );

      // Valid response; queue the response
      virtual void queueResponse( std::string to );
};

#endif
