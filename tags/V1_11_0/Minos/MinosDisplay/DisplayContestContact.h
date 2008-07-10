//--------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef DisplayContestContactH
#define DisplayContestContactH 
//----------------------------------------------------------------------------
class DisplayContestContact: public BaseContact
{
      int modificationCount;
      unsigned long logSequence; // sparse sequence, used to provide sort key
   protected:
      DisplayContestContact( const DisplayContestContact & );
      DisplayContestContact& operator =( const DisplayContestContact & );
   public:

      virtual void setLogSequence( unsigned long ul )
      {
         logSequence = ul;
      }
      virtual unsigned long getLogSequence() const
      {
         return logSequence;
      }
      virtual int getModificationCount() const
      {
         return modificationCount;
      }
      virtual void setModificationCount( int c )
      {
         modificationCount = c;
      }

      // Contact items are all in BaseContact


      // end of Contact items

      virtual bool commonSave( )
      {
         return false;
      }
      //      virtual bool GJVload( int diskBlock ){return false;}   // moved to BAseCointact


      DisplayContestContact( BaseContestLog *contest, bool time_now );
      ~DisplayContestContact();
      virtual bool ne( const ScreenContact& ) const;
      virtual void check( );

      virtual void copyFromArg( ScreenContact & );

      virtual std::string getField( int ACol, const BaseContestLog * const curcon ) const;
      virtual void processMinosStanza( const std::string &methodName, MinosTestImport * const mt );

};

#endif
