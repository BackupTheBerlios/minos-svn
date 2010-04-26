/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include <vcl.h>
#pragma hdrstop
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <stdexcept>
#include <fstream>
#include <conio.h>

extern void testRequest();
int main( int argc, char* argv[] )
{
   // Retreive test path from command line first argument. Default to "" which resolve
   // to the top level suite.
   std::string testPath = ( argc > 1 ) ? std::string( argv[ 1 ] ) : std::string( "" );

   // Create the event manager and test controller
   CPPUNIT_NS::TestResult controller;

   // Add a listener that colllects test result
   CPPUNIT_NS::TestResultCollector result;
   controller.addListener( &result );

   // Add a listener that print dots as test run.
#ifdef WIN32

   CPPUNIT_NS::TextTestProgressListener progress;
#else

   CPPUNIT_NS::BriefTestProgressListener progress;
#endif

   controller.addListener( &progress );


   // Get the top level suite from the registry
   CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

   // Adds the test to the list of test to run
   CPPUNIT_NS::TestSuite runner;
   runner.addTest( suite );

   try
   {
      CPPUNIT_NS::stdCOut() << "Running " << testPath;

      // Run the test.
      runner.run( &controller );

      CPPUNIT_NS::stdCOut() << "\n";

      // Print test in a compiler compatible format.
      CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
      outputter.write();

      // Uncomment this for XML output
      std::ofstream file( "tests.xml" );
      CPPUNIT_NS::XmlOutputter xml( &result, file );
      //xml.setStyleSheet( "report.xsl" );
      xml.write();
      file.close();
   }
   catch ( std::invalid_argument & e )   // Test path not resolved
   {
      CPPUNIT_NS::stdCOut() << "\n"
      << "ERROR: " << e.what()
      << "\n";
      return 0;
   }
   getch();
   return result.wasSuccessful() ? 0 : 1;
}


