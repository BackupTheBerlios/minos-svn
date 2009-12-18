/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PrintFileH
#define PrintFileH
//---------------------------------------------------------------------------
enum PrintFile_order;
class PrintFileLine
{
   public:
      std::string prefix;
      std::string data;

      PrintFileLine( const std::string &, const std::string &dat );
      PrintFileLine( void );
      ~PrintFileLine();

      PrintFileLine & operator = ( const PrintFileLine & );
};

class PrintFile
{
      LoggerContestLog * ct;
   public:


      PrintFile( LoggerContestLog * const ct );

      ~PrintFile();
      bool exportTest( HANDLE expfd );
};
#endif
