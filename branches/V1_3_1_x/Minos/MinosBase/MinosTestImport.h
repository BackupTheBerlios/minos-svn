/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MinosTestImportH
#define MinosTestImportH 
//---------------------------------------------------------------------------
class MinosTestImport: public RPCDispatcher
{
   protected:
      typedef void ( MinosTestImport::* DispatchCallback ) ( XStanza *stanza ) ;
      DispatchCallback dispatchCallback;

      virtual void dispatchResponse( XStanza *a );
      void analyseImportLog( XStanza * );
      void analyseImportTest( XStanza * );
   private:
      BaseContestLog *ct;
      int curfpos;
      int iqOffset;

      boost::shared_ptr<RPCParam> body;

      int stanzaCount;

      bool getStructArgMemberValueDTG( boost::shared_ptr<RPCParam> rpm, const std::string &name, std::string &val );
      bool getStructArgMemberValueDTG( boost::shared_ptr<RPCParam>rpm, const std::string &name, MinosItem<std::string> &val );
      bool getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, std::string &val );
      bool getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, MinosItem<std::string> &val );
      bool getStructArgMemberValue(boost::shared_ptr<RPCParam>rpm, const std::string &name, int &val );
      bool getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, MinosItem<int> &val );
      bool getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, bool &val );
      bool getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, MinosItem<bool> &val );
   public:
      void processMinosStanza( RPCRequest *xs );
      void processLogMinosStanza( RPCRequest *xs );
      MinosTestImport( BaseContestLog * const ct );
      MinosTestImport( );
      ~MinosTestImport();

      void startImportTest();
      int importTestBuffer( const std::string &buffer );
      void endImportTest();

      int importTest( HANDLE ctfile );
      bool analyseTest( HANDLE ctfile );
      int readTestFile( HANDLE ctfile );

      bool getStructArgMemberValueDTG( const std::string &name, std::string &val );
      bool getStructArgMemberValueDTG( const std::string &name, MinosItem<std::string> &val );
      bool getStructArgMemberValue( const std::string &name, std::string &val );
      bool getStructArgMemberValue( const std::string &name, MinosItem<std::string> &val );
      bool getStructArgMemberValue( const std::string &name, int &val );
      bool getStructArgMemberValue( const std::string &name, MinosItem<int> &val );
      bool getStructArgMemberValue( const std::string &name, bool &val );
      bool getStructArgMemberValue( const std::string &name, MinosItem<bool> &val );

      void setBit( unsigned short &cf, unsigned short bit, bool set
                    );
};
#endif
