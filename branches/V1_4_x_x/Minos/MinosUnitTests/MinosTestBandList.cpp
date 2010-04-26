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

#include "BandList.h"
#include "MinosTestBandList.h"
#pragma package(smart_init)

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BandListTestParams );

void BandListTestParams::setUp()
{

}
void BandListTestParams::tearDown()
{

}
void BandListTestParams::testParse()
{
   BandList blist;
   bool loaded = blist.parseFile( ".\\configuration\\bandlist.xml" );
   CPPUNIT_ASSERT_EQUAL( true, loaded );
}
void BandListTestParams::testBands()
{
   BandList blist;
   bool loaded = blist.parseFile( ".\\configuration\\bandlist.xml" );
   CPPUNIT_ASSERT_EQUAL( true, loaded );

   BandInfo b;
   loaded = blist.findBand(144250000.0, b);
   CPPUNIT_ASSERT_EQUAL( true, loaded );
   CPPUNIT_ASSERT_EQUAL( std::string("VHF"), b.type );
   int cnt = blist.bandList.size();
   CPPUNIT_ASSERT_EQUAL( 21, cnt );

}

