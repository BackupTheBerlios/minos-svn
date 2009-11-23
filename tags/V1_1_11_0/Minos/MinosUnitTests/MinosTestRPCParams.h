/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosTestRPCParamsH
#define MinosTestRPCParamsH

#include <cppunit/extensions/HelperMacros.h>

class RPCTestActions : public CPPUNIT_NS::TestFixture
{
      CPPUNIT_TEST_SUITE( RPCTestActions );
      CPPUNIT_TEST( testInt );
      CPPUNIT_TEST( testBool );
      CPPUNIT_TEST( testDouble );
      CPPUNIT_TEST( testString );
      CPPUNIT_TEST( testBase64 );
      CPPUNIT_TEST( testDtg );
      CPPUNIT_TEST( testStruct );
      CPPUNIT_TEST( testArray );
      CPPUNIT_TEST( testMultiParam );
      CPPUNIT_TEST( testAddParams );
      CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();
      void tearDown();

      void testInt();
      void testBool();
      void testDouble();
      void testString();
      void testBase64();
      void testDtg();
      void testStruct();
      void testArray();
      void testMultiParam();
      void testAddParams();
};

#endif
