/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#pragma hdrstop

#include <cppunit/config/SourcePrefix.h>
#include "MinosTestRPCParams.h"

#include <string>
#include <vector>
#include <stdlib.h>
#include "boost\shared_ptr.hpp"

#include "XMPPRPCParams.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( RPCTestActions );


void
RPCTestActions::setUp()
{
   //    CPPUNIT_NS::stdCOut() << "s";
}


void
RPCTestActions::tearDown()
{
   //    CPPUNIT_NS::stdCOut() << "t";
}


void RPCTestActions::testInt()
{
   int i1 = 1;

   RPCArgs * xms = new RPCArgs();
   xms->addParam( i1 );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   int ires;

   bool pOK = xm.getIntArg( 0, ires );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( i1, ires );
}

void RPCTestActions::testBool()
{
   bool b1 = true;

   RPCArgs * xms = new RPCArgs();
   xms->addParam( b1 );
   xms->addParam( !b1 );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   bool bres;

   bool pOK = xm.getBooleanArg( 0, bres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( b1, bres );

   pOK = xm.getBooleanArg( 1, bres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( !b1, bres );
}

void RPCTestActions::testDouble()
{
   double d1 = 3.14159;

   RPCArgs * xms = new RPCArgs();
   xms->addParam( d1 );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   double dres;

   bool pOK = xm.getDoubleArg( 0, dres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( d1, dres );
}

void RPCTestActions::testString()
{
   std::string s1( "This is a longish string which we are using for testing" );
   std::string s2( "Try it <with some XML \"escape   characters\"> & some that aren't" );

   RPCArgs * xms = new RPCArgs();
   xms->addParam( s1 );
   xms->addParam( s2 );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   std::string sres;

   bool pOK = xm.getStringArg( 0, sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( s1, sres );

   pOK = xm.getStringArg( 1, sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( s2, sres );
}

void RPCTestActions::testBase64()
{
   std::string s1( "This is a longish string which we are using for testing" );

   RPCArgs * xms = new RPCArgs();
   xms->addBase64Param( s1 );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   std::string sres;

   bool pOK = xm.getBase64Arg( 0, sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( s1, sres );
}

void RPCTestActions::testDtg()
{
   std::string s1( "This is a longish string which we are using for testing" );

   RPCArgs * xms = new RPCArgs();
   xms->addDtgParam( s1 );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   std::string sres;

   bool pOK = xm.getDtgArg( 0, sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( s1, sres );
}

void RPCTestActions::testStruct()
{
   RPCArgs * xms = new RPCArgs();
   boost::shared_ptr<RPCParam>ps(new RPCParamStruct);
   ps->addMember( std::string( "name value" ), "fred" );
   ps->addMember( std::string( "value value" ), "joe" );
   xms->addParam( ps );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   boost::shared_ptr<RPCParam> pres;
   std::string sres;
   int ires;

   bool pOK = xm.getStructArgMember( 0, "fred", pres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   pOK = pres->getString( sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( std::string( "name value" ), sres );

   pOK = pres->getInt( ires );
   CPPUNIT_ASSERT_EQUAL( false, pOK );

   pOK = xm.getStructArgMember( 0, "test", pres );
   CPPUNIT_ASSERT_EQUAL( false, pOK );

   pOK = xm.getStructArgMember( 0, 1, pres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   pOK = pres->getString( sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( std::string( "value value" ), sres );

   pOK = xm.getStructArgMember( 0, 2, pres );
   CPPUNIT_ASSERT_EQUAL( false, pOK );
}

RPCParamArray *RPCArrayFromStringVector( const std::vector< std::string> &plist )
{
   RPCParamArray * rpc = new RPCParamArray();
   for ( unsigned int i = 0; i < plist.size(); i++ )
   {
      rpc->addElement( plist[ i ].c_str() );
   }
   return rpc;
}

void RPCTestActions::testArray()
{
   RPCArgs * xms = new RPCArgs();

   std::vector < std::string > testvec;
   testvec.push_back( "string 1" );
   testvec.push_back( "string 2" );
   testvec.push_back( "string 3" );
   testvec.push_back( "string 4" );
   testvec.push_back( "" );


   boost::shared_ptr<RPCParam>rpca(RPCArrayFromStringVector( testvec ));
   xms->addParam( rpca );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   //      bool GetArrayArgElements( unsigned int argno, unsigned int &size );
   //      bool GetArrayArgElement( unsigned int argno, unsigned int eleno, RPCParam *&res );

   unsigned int asize;

   bool pOK = xm.getArrayArgElements( 0, asize );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( ( unsigned int ) testvec.size(), asize );

   for ( unsigned int i = 0; i < testvec.size(); i++ )
   {
      boost::shared_ptr<RPCParam> pres;
      std::string sres;

      bool pOK = xm.getArrayArgElement( 0, i, pres );
      CPPUNIT_ASSERT_EQUAL( true, pOK );
      CPPUNIT_ASSERT_EQUAL( true, pres != 0 );

      pOK = pres->getString( sres );
      CPPUNIT_ASSERT_EQUAL( true, pOK );
      CPPUNIT_ASSERT_EQUAL( testvec[ i ], sres );
   }
}

void
RPCTestActions::testMultiParam()
{
   std::string s1( "BarPosition" );
   std::string s2( "HOVenue" );
   int i1 = 1;

   RPCArgs * xms = new RPCArgs();
   xms->addParam( s1 );
   xms->addParam( s2 );
   xms->addParam( i1 );

   std::string pstr = xms->makeParamsString();

   delete xms;

   RPCArgs xm;
   bool parseOK = xm.parseParams( pstr );
   CPPUNIT_ASSERT_EQUAL( true, parseOK );

   std::string sres;
   int ires;

   bool pOK = xm.getStringArg( 0, sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( s1, sres );

   pOK = xm.getStringArg( 1, sres );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( s2, sres );

   pOK = xm.getIntArg( 2, ires );
   CPPUNIT_ASSERT_EQUAL( true, pOK );
   CPPUNIT_ASSERT_EQUAL( i1, ires );
}

void RPCTestActions::testAddParams()
{
   // This is more complicated, and needs a real iks node
}

