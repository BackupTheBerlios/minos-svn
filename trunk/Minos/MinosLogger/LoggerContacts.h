/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef LoggerContactsH
#define LoggerContactsH 
//----------------------------------------------------------------------------
// This header covers the structure of contests, logs, etc


class LoggerContestLog;

class ContestContact: public DisplayContestContact
{
   private:
      ContestContact( const ContestContact & );
      ContestContact& operator =( const ContestContact & );
   public:
      virtual void clearDirty();
      virtual void setDirty();

      virtual bool setField( int ACol, const std::string Value );

      ContestContact( LoggerContestLog *contest, bool time_now );
      ~ContestContact();
      void getPrintFileText( std::string &, short );
      void addReg1TestComment( std::vector <std::string> &remarks );
      void getReg1TestText( std::string & );
      std::string getADIFLine();
      bool commonSave( );
      bool minosSave( );
      bool GJVsave( GJVParams & );
      virtual bool GJVload( int diskBlock );
      virtual void processMinosStanza( const std::string &methodName, MinosTestImport * const mt );
};


#endif
