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

#include <time.h>
#include <string>
#include <vector>
#include <stdlib.h>

#include <cppunit/config/SourcePrefix.h>

#include "mwin.h"
#include "MinosTestTimes.h"

//---------------------------------------------------------------------------
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TimerTest );


void
TimerTest::setUp()
{
   //    CPPUNIT_NS::stdCOut() << "s";
}


void
TimerTest::tearDown()
{
   //    CPPUNIT_NS::stdCOut() << "t";
}
void TimerTest::testTimes()
{
/*
   dtg tnow( true );

   time_t cttime;
   tnow.getDtg( cttime);

   time_t t = ::time( 0 );

   time_t tdiff = t - cttime;

   CPPUNIT_ASSERT_EQUAL( true, tdiff == 0 );
   */
}
