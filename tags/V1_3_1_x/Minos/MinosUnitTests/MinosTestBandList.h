/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosTestBandListH
#define MinosTestBandListH

#include <cppunit/extensions/HelperMacros.h>

class BandListTestParams : public CPPUNIT_NS::TestFixture
{
      CPPUNIT_TEST_SUITE( BandListTestParams );
      CPPUNIT_TEST( testParse );
      CPPUNIT_TEST( testBands );
      CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();
      void tearDown();

      void testParse();
      void testBands();
};
#endif
