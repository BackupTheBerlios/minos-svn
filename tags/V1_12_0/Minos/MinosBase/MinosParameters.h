/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#ifndef MinosParametersH
#define MinosParametersH 
//---------------------------------------------------------------------------
class BaseContestLog;
class ContactList;
class MinosParameters
{
   private:
      static MinosParameters *mp;
   protected:
      static void setMinosParameters( MinosParameters *m )
      {
         mp = m;
      }
   public:
      MinosParameters()
      {
         mp = this;
      }
      virtual ~MinosParameters()
      {
         mp = 0;
      }

      static MinosParameters *getMinosParameters();
      virtual bool insertContest( BaseContestLog *p, unsigned int sno ) = 0;
      virtual int getMagneticVariation() = 0;
      virtual void getDisplayColumnWidth( const std::string &key, int &val, int def ) = 0;
      virtual void setDisplayColumnWidth( const std::string &key, int val ) = 0;
      virtual void getBoolDisplayProfile( int enumkey, bool &value ) = 0;
      virtual void setBoolDisplayProfile( int enumkey, bool value ) = 0;
      virtual void flushDisplayProfile( void ) = 0;
      virtual double getBigClockCorrection() = 0;
      virtual int getStatsPeriod1() = 0;
      virtual int getStatsPeriod2() = 0;
      virtual void setStatsPeriod1( int ) = 0;
      virtual void setStatsPeriod2( int ) = 0;
      virtual void addOperator( const std::string &curop ) = 0;
      virtual void setError( int err ) = 0;
      virtual bool getAllowLoc4() = 0;
      virtual bool getAllowLoc8() = 0;
      virtual bool yesNoMessage( TComponent* Owner, String mess ) = 0;
      virtual void mshowMessage( String mess, TComponent* Owner = 0 ) = 0;
      virtual void showErrorList( ) = 0;
      virtual void showContestScore( const std::string &score ) = 0;
      virtual BaseContestLog *getCurrentContest() = 0;
      virtual bool insertList( ContactList *p, unsigned int sno ) = 0;
      virtual bool isContestOpen( const std::string fn ) = 0;
      virtual bool isListOpen( const std::string fn ) = 0;
      virtual ErrorList &getErrorList() = 0;
      virtual void valtrace( int mess_no, bool flag ) = 0;
      virtual bool isErrSet( int mess_no ) = 0;
      virtual void clearErrorList() = 0;
};
class MinosParametersAdapter : public MinosParameters
{
      ErrorList errs;
   public:
      MinosParametersAdapter()
      {}
      ~MinosParametersAdapter()
      {}
      virtual bool insertContest( BaseContestLog *p, unsigned int sno );
      virtual int getMagneticVariation();
      virtual void getDisplayColumnWidth( const std::string &key, int &val, int def );
      virtual void setDisplayColumnWidth( const std::string &key, int val );
      virtual void getBoolDisplayProfile( int enumkey, bool &value );
      virtual void setBoolDisplayProfile( int enumkey, bool value );
      virtual void flushDisplayProfile( void );
      virtual double getBigClockCorrection();
      virtual int getStatsPeriod1();
      virtual int getStatsPeriod2();
      virtual void setStatsPeriod1( int );
      virtual void setStatsPeriod2( int );
      virtual void addOperator( const std::string &curop );
      virtual void setError( int err );
      virtual bool getAllowLoc4();
      virtual bool getAllowLoc8();
      virtual bool yesNoMessage( TComponent* Owner, String mess );
      virtual void mshowMessage( String mess, TComponent* Owner = 0 );
      virtual void showErrorList( );
      virtual void showContestScore( const std::string &score );
      virtual BaseContestLog *getCurrentContest();
      virtual bool insertList( ContactList *p, unsigned int sno );
      virtual bool isContestOpen( const std::string fn );
      virtual bool isListOpen( const std::string fn );
      virtual ErrorList &getErrorList();
      virtual void valtrace( int mess_no, bool flag );
      virtual bool isErrSet( int mess_no );
      virtual void clearErrorList();
};
#endif
