/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef ScreenContactH
#define ScreenContactH 
//----------------------------------------------------------------------------
class ScreenContact
{
      unsigned long logSequence; // sparse sequence, used to provide
   protected:
      BaseContestLog *contest;
   public:
      ScreenContact();
      virtual ~ScreenContact();
      virtual void copyFromArg( BaseContact & );   // this MIGHT just get used for dummy, for ops
      virtual void copyFromArg( ScreenContact & );    // used for partialSave
      void check( );
      virtual bool isNextContact( void ) const;
      virtual void setLogSequence( unsigned long ul )
      {
         logSequence = ul;
      }
      virtual unsigned long getLogSequence() const
      {
         return logSequence;
      }

      void initialise( BaseContestLog *ct );


      callsign cs;   //CONTAIN MinosItem
      locator loc;   //CONTAIN MinosItem
      dtg time;      //CONTAIN MinosItem

      std::string mode;
      std::string reps;
      std::string serials;
      std::string repr;
      std::string serialr;
      std::string extraText;
      std::string comments;
      unsigned short contactFlags;
      std::string forcedMult;

      MinosItem<std::string> op1;
      MinosItem<std::string> op2;
      //------------------

      bool valid;
      bool newCtry;
      bool newDistrict;
      bool newLoc;

      DistrictEntry * districtMult;
      CountryEntry * ctryMult;


      int contactScore;
      int bearing;
      char multCount;
};

#endif
