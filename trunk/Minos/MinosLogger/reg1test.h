/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef Reg1testH
#define Reg1testH
enum reg1test_order;

class reg1testLine
{
   public:
      std::string prefix;
      std::string data;

      reg1testLine( const std::string &, const std::string &dat );
      reg1testLine( void );
      ~reg1testLine();

      reg1testLine & operator = ( const reg1testLine & );
};

class reg1test
{
      LoggerContestLog * ct;
      HANDLE regContestFile;
      int nextBlock;
      std::vector <std::string> remarks;
      bool parseHeader( String line );
      bool parseRemark( String line );
      bool parseQSO( String line );
      bool importReg1test();
   public:

      reg1test( LoggerContestLog * c, HANDLE hFile );
      reg1test( LoggerContestLog * const ct );

      ~reg1test();
      bool exportTest( HANDLE expfd );
      static bool doImportReg1test( LoggerContestLog * c, HANDLE hFile );

};
#endif
