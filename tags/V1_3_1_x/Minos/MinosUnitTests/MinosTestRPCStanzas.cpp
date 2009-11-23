/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#pragma hdrstop

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cppunit/config/SourcePrefix.h>

#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include "boost\shared_ptr.hpp"

#include "tinyxml.h"

#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"

#include "Dispatcher.h"
#include "MinosTestRPCStanzas.h" 
// Need this to satisfy the linker
HANDLE DaemonConnectEvent = 0;
HANDLE DaemonTerminateEvent = 0;
bool connected = false;

//---------------------------------------------------------------------------
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( StanzaTestActions );


void
StanzaTestActions::setUp()
{
   //    CPPUNIT_NS::stdCOut() << "s";
}


void
StanzaTestActions::tearDown()
{
   //    CPPUNIT_NS::stdCOut() << "t";
}


// Here we REALLY need to find a way of using DaemonThread and its descendants
// so that we can do a REAL test!

void StanzaTestActions::dispatchResponse( XStanza *xs )
{
   ( this->*dispatchCallback ) ( xs ); // What a horrid syntax for calling through a member pointer!
}

//---------------------------------------------------------------------------

void
StanzaTestActions::testRequest()
{
   std::string toId;
   RPCRequest *xm = new RPCRequest( toId, "testRequest" );

   std::string s1( "BarPosition" );
   std::string s2( "HOVenue" );
   int i1 = 1;

   xm->addParam( s1 );
   xm->addParam( s2 );
   xm->addParam( i1 );

   std::string UTF8XML = xm->getActionMessage();

   delete xm;

   dispatchCallback = &StanzaTestActions::analyseRequestTest;

   bool ret = analyseNode( this, UTF8XML );
   CPPUNIT_ASSERT_EQUAL( true, ret );
}
void StanzaTestActions::analyseRequestTest( XStanza *xs )
{
   RPCRequest * req = dynamic_cast<RPCRequest *>( xs );
   CPPUNIT_ASSERT_EQUAL( true, req != 0 );

   // and now analyse req
   CPPUNIT_ASSERT_EQUAL( true, req->methodName == "testRequest" );

   std::string s;
   bool pOK = req->getStringArg( 0, s );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( true, s == "BarPosition" );

   pOK = req->getStringArg( 1, s );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( true, s == "HOVenue" );

   int i;
   pOK = req->getIntArg( 2, i );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( true, i == 1 );
}
//---------------------------------------------------------------------------

void
StanzaTestActions::testResponse()
{
   std::string toId = "256";
   std::string to = "toMe";
   RPCResponse *xm = new RPCResponse( to, toId, "testResponse" );

   std::string s1( "BarPosition" );
   std::string s2( "HOVenue" );
   int i1 = 1;

   xm->addParam( s1 );
   xm->addParam( s2 );
   xm->addParam( i1 );

   std::string UTF8XML = xm->getActionMessage();

   delete xm;

   dispatchCallback = &StanzaTestActions::analyseResponseTest;

   bool ret = analyseNode( this, UTF8XML );

   CPPUNIT_ASSERT_EQUAL( true, ret );

}
void StanzaTestActions::analyseResponseTest( XStanza *xs )
{
   RPCResponse * req = dynamic_cast<RPCResponse *>( xs );
   CPPUNIT_ASSERT_EQUAL( true, req != 0 );
   CPPUNIT_ASSERT_EQUAL( true, req->getId() == "256" );

   // and now analyse req
   CPPUNIT_ASSERT_EQUAL( true, req->methodName == "testResponse" );

   std::string s;
   bool pOK = req->getStringArg( 0, s );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( true, s == "BarPosition" );

   pOK = req->getStringArg( 1, s );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( true, s == "HOVenue" );

   int i;
   pOK = req->getIntArg( 2, i );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( true, i == 1 );
}

