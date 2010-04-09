/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		VHF Contest Adjudication
//
// COPYRIGHT         (c) M. J. Goodey G0GJV, 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MLogFileH
#define MLogFileH 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class MLogFile
{
   private:
      std::string fLogFileName;
   public:

      MLogFile()
      { }
      // CreateLogFile is called by the boot form at startup
      std::ostream & createLogFile( const std::string &prefix, int keepDays );

      // Log and LogT are used to log data without and with a timestamp;
      std::ostream & log( void );
      std::ostream & logT( void );
      // Log logs a string with a time prefix hh:nn:ss
      std::ostream & log( const std::string &s );
      void close( void );

      // Generates a suffix of the form yyyyddmmhhmmss
      static AnsiString generateLogFileName( const std::string &prefix );
      static void tidyFiles( const std::string &prefix, int keepDays );
      std::string getTraceFileName()
      {
         return fLogFileName;
      }
};
//---------------------------------------------------------------------------
#endif
