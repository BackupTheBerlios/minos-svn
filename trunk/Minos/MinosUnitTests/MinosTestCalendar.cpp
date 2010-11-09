/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <string>
#include <vector>
#include <stdlib.h>

#include <cppunit/config/SourcePrefix.h>

#include "MinosTestCalendar.h"
#include "VHFList.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CalendarTestParams );


void
CalendarTestParams::setUp()
{
   //    CPPUNIT_NS::stdCOut() << "s";
}


void
CalendarTestParams::tearDown()
{
   //    CPPUNIT_NS::stdCOut() << "t";
}
void CalendarTestParams::testDates()
{
   setYear( 2008 );

   int m = getMonth( "february" );
   CPPUNIT_ASSERT_EQUAL( 2, m );
   m = getMonth( "rubbish" );
   CPPUNIT_ASSERT_EQUAL( 0, m );

   int d = getDate( /*month*/1,  /*day*/1,  /*week*/1 );   // first Monday is 6th
   CPPUNIT_ASSERT_EQUAL( 7, d );  // Tuesday 1st, first Monday is 7th

   d = getDate( /*month*/5,  /*day*/6,  /*week*/3 );   // May 144
   CPPUNIT_ASSERT_EQUAL( 17, d );  // Tuesday 1st, first Monday is 7th

   d = getDate( /*month*/6,  /*day*/7,  /*week*/2 );
   CPPUNIT_ASSERT_EQUAL( 8, d );  // 2nd sunday in june

   d = getDate( /*month*/6,  /*day*/2,  /*week*/5 );
   CPPUNIT_ASSERT_EQUAL( 0, d );  // 5th Tuesday in june

   TDateTime t = TDateTime( 2008, 5, 1 );
   t += TDateTime( 0, 0, 0, 0 );

   TDateTime t2 = localToUTC( t );
   CPPUNIT_ASSERT_EQUAL( 3600, ( int ) ( ( double( t - t2 ) + 0.5 / 86400 ) * 86400 ) );

   t = TDateTime( 2008, 10, 25 );
   t += TDateTime( 0, 0, 0, 0 );

   t2 = localToUTC( t );

   CPPUNIT_ASSERT_EQUAL( 3600, ( int ) ( ( double( t - t2 ) + 0.5 / 86400 ) * 86400 ) );

   t = TDateTime( 2008, 10, 26 );
   t += TDateTime( 0, 0, 0, 0 );

   t2 = localToUTC( t );
   CPPUNIT_ASSERT_EQUAL( 0, ( int ) ( ( double( t - t2 ) + 0.5 / 86400 ) * 86400 ) );
}
void CalendarTestParams::testParse()
{
   Calendar vhf(2008);
   bool loaded = vhf.parseFile( ".\\configuration\\vhfContests.xml" );
   CPPUNIT_ASSERT_EQUAL( true, loaded );
}
void CalendarTestParams::testContests()
{
   Calendar vhf(2008);
   bool loaded = vhf.parseFile( ".\\configuration\\vhfContests.xml" );
   CPPUNIT_ASSERT_EQUAL( true, loaded );

   int nc = vhf.contests.size();
   CPPUNIT_ASSERT_EQUAL( 33, nc );

   std::string lp144( "lp144" );
   Contest &qrp144 = vhf.contests[ lp144 ];
   CPPUNIT_ASSERT_EQUAL( lp144, qrp144.name );

}
void CalendarTestParams::testDates2009()
{
   setYear( 2009 );

   int m = getMonth( "february" );
   CPPUNIT_ASSERT_EQUAL( 2, m );
   m = getMonth( "rubbish" );
   CPPUNIT_ASSERT_EQUAL( 0, m );

   int d = getDate( /*month*/1,  /*day*/1,  /*week*/1 );   // first Monday is 5th
   CPPUNIT_ASSERT_EQUAL( 5, d );

   d = getDate( /*month*/5,  /*day*/6,  /*week*/3 );   // May 144 - Sat 16th
   CPPUNIT_ASSERT_EQUAL( 16, d );

   d = getDate( /*month*/6,  /*day*/7,  /*week*/2 );
   CPPUNIT_ASSERT_EQUAL( 14, d );  // 2nd sunday in june

   d = getDate( /*month*/6,  /*day*/2,  /*week*/5 );
   CPPUNIT_ASSERT_EQUAL( 30, d );  // 5th Tuesday in june

   TDateTime t = TDateTime( 2009, 5, 1 );
   t += TDateTime( 0, 0, 0, 0 );

   TDateTime t2 = localToUTC( t );
   CPPUNIT_ASSERT_EQUAL( 3600, ( int ) ( ( double( t - t2 ) + 0.5 / 86400 ) * 86400 ) );

   t = TDateTime( 2009, 10, 24 );
   t += TDateTime( 0, 0, 0, 0 );

   t2 = localToUTC( t );

   CPPUNIT_ASSERT_EQUAL( 3600, ( int ) ( ( double( t - t2 ) + 0.5 / 86400 ) * 86400 ) );

   t = TDateTime( 2009, 10, 25 );
   t += TDateTime( 0, 0, 0, 0 );

   t2 = localToUTC( t );
   CPPUNIT_ASSERT_EQUAL( 0, ( int ) ( ( double( t - t2 ) + 0.5 / 86400 ) * 86400 ) );
}
void CalendarTestParams::testParse2009()
{
   Calendar vhf(2009);
   bool loaded = vhf.parseFile( ".\\configuration\\vhfContests09.xml" );
   CPPUNIT_ASSERT_EQUAL( true, loaded );
}
void CalendarTestParams::testContests2009()
{
   Calendar vhf(2009);
   bool loaded = vhf.parseFile( ".\\configuration\\vhfContests09.xml" );
   CPPUNIT_ASSERT_EQUAL( true, loaded );

   int nc = vhf.contests.size();
   CPPUNIT_ASSERT_EQUAL( 33, nc );

   std::string lp144( "lp144" );
   Contest &qrp144 = vhf.contests[ lp144 ];
   CPPUNIT_ASSERT_EQUAL( lp144, qrp144.name );

}
int fsun(int month)
{
   int fSunday = getDate(month, 7, 1);    // month, day, week
   return fSunday;
}
void CalendarTestParams::testBST()
{
   for (int i = 0; i < 40; i++)
   {
      setYear(2005 + i);
      {
         int fs = fsun(4);
         int lsMarch = fs + 31 - 7;
         TDateTime t(2005 + i, 3, lsMarch);
         int dow = getDayOfWeek(t);
         CPPUNIT_ASSERT_EQUAL( 7, dow );
         CPPUNIT_ASSERT_EQUAL( true, dow >= 1 && dow <= 7);
      }
      {
         int fs = fsun(11);
         int lsOct = fs + 31 - 7;
         TDateTime t(2005 + i, 10, lsOct);
         int dow = getDayOfWeek(t);
         CPPUNIT_ASSERT_EQUAL( 7, dow );
         CPPUNIT_ASSERT_EQUAL( true, dow >= 1 && dow <= 7);
      }
   }

}
