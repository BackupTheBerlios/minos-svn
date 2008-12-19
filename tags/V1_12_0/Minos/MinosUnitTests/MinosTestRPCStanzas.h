/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosTestRPCStanzasH
#define MinosTestRPCStanzasH 
//---------------------------------------------------------------------------

#include <cppunit/extensions/HelperMacros.h>

class XStanza;

class StanzaTestActions : public RPCDispatcher, public CPPUNIT_NS::TestFixture
{
      CPPUNIT_TEST_SUITE( StanzaTestActions );
      CPPUNIT_TEST( testRequest );
      CPPUNIT_TEST( testResponse );
      CPPUNIT_TEST_SUITE_END();

      void analyseRequestTest( XStanza * );
      void analyseResponseTest( XStanza * );

   public:
      //   protected:
      //      typedef  void (ResponseDispatcher::* DispatchCallback)(XStanza *stanza) ;
      //      DispatchCallback dispatchCallback;

      typedef void ( StanzaTestActions::* DispatchCallback ) ( XStanza *stanza ) ;
      DispatchCallback dispatchCallback;

      virtual void dispatchResponse( XStanza *a );

      void setUp();
      void tearDown();
      void testRequest();
      void testResponse();

};
#endif
