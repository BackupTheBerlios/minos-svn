/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosTestCalendarH
#define MinosTestCalendarH

#include <cppunit/extensions/HelperMacros.h>

class CalendarTestParams : public CPPUNIT_NS::TestFixture
{
      CPPUNIT_TEST_SUITE( CalendarTestParams );
      CPPUNIT_TEST( testDates );
      CPPUNIT_TEST( testParse );
      CPPUNIT_TEST( testContests );
      CPPUNIT_TEST( testDates2009 );
      CPPUNIT_TEST( testParse2009 );
      CPPUNIT_TEST( testContests2009 );
      CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();
      void tearDown();

      void testParse();
      void testContests();
      void testDates();
      void testParse2009();
      void testContests2009();
      void testDates2009();
};
#endif
