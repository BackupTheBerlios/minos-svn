/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosTestTimesH
#define MinosTestTimesH

#include <cppunit/extensions/HelperMacros.h>

class TimerTest : public CPPUNIT_NS::TestFixture
{
      CPPUNIT_TEST_SUITE( TimerTest );
      CPPUNIT_TEST( testTimes );
      CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();
      void tearDown();

      void testTimes();
};
#endif
