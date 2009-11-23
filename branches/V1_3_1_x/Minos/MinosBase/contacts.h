/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef ContactsH
#define ContactsH 
//----------------------------------------------------------------------------
// This header covers the structure of contests, logs, etc

namespace ContactBuffs
{
   extern char mult_chars[];

   extern char scorebuff[ 20 ];
   extern char brgbuff[ 6 ];
   extern char buff2[ 120 ];
   extern char qthbuff[ 120 ];
   extern char srbuff[ 6 ];
   extern char ssbuff[ 6 ];
   extern char buff[ 1024 ];
}

class MatchContact;

class TMatchCollection;
class dupsheet;

template < class itemtype >
class MultList;
class DistrictEntry;
class DistrictSynonym;
class DistrictList;
class CountrySynonymList;
class CountryEntry;
class CountrySynonym;
class CountryList;
class ScreenContact;

CountryEntry *findCtryPrefix( const callsign &cs );

// This is a contact, either in the log or the current screen contact
// Various parts may be missing to allow "comment" records in the log.

// NB we still have the FF00 flags available!
// And these are in the GJV file, so don't change the values!

const short LOCAL_COMMENT = 0x0800;		// Local comment only
const short VALID_DISTRICT = 0x0400;		// Treat district as valid when checking
const short COUNTRY_FORCED = 0x0200;		// take the country forced in
const short UNKNOWN_COUNTRY = 0x0100;		// country not in control files
const short NON_SCORING = 0x0080;     // general non scoring
const short COMMENT_ONLY = 0x0040;     // only a (long) comment
const short MANUAL_SCORE = 0x0020;     // scored manually
const short DONT_PRINT = 0x0010;     // Not to be printed (deleted)
const short VALID_DUPLICATE = 0x0008;		// e.g. backpacker, second time
const short TO_BE_ENTERED = 0x0004;		// auto spaced, to be gone back to later
const short XBAND = 0x0002;		// Cross band - half score
const short FORCE_LOG = 0x0001;		// Force logged into log

class BaseContact
{
      std::vector < BaseContact > history;
   protected:
      BaseContestLog *contest;
   public:
      BaseContact( const BaseContact & );
      BaseContact( BaseContestLog *contest, bool time_now );
      BaseContact& operator =( const BaseContact & );
      virtual void setLogSequence( unsigned long ul )
      {}
      virtual unsigned long getLogSequence() const
      {
         return 0;
      }
      virtual int getModificationCount() const
      {
         return 0;
      }
      virtual std::vector < BaseContact > &getHistory()
      {
         return history;
      }
      // These CONTAIN minositems

      dtg updtime;      //CONTAIN MinosItem

      callsign cs;   //CONTAIN MinosItem
      locator loc;   //CONTAIN MinosItem
      dtg time;      //CONTAIN MinosItem

      MinosItem<std::string> extraText;
      MinosItem<std::string> mode;
      MinosItem<std::string> reps;
      MinosItem<std::string> serials;
      MinosItem<std::string> repr;
      MinosItem<std::string> serialr;
      MinosItem<std::string> comments;
      MinosItem<unsigned short> contactFlags;
      MinosItem<std::string> forcedMult;

      MinosItem<std::string> op1;         // current main op - derived from contacts
      MinosItem<std::string> op2;         // current second op - derived from contacts

      MinosItem<int> contactScore;

      virtual void clearDirty();
      virtual void setDirty();

      int bearing;

      bool QSOValid;
      bool newDistrict;
      bool newCtry;
      bool newLoc;

      DistrictEntry * districtMult;
      CountryEntry * ctryMult;
      virtual void makestrings( bool serialFields ) const;
      virtual void getText( std::string &dest, const BaseContestLog * const curcon ) const;
      char multCount;

      // silly implementations that are needed to keep things isolated
      virtual std::string getField( int ACol, const BaseContestLog * const curcon ) const
      {
         return "no field";
      }
      virtual bool ne( const ScreenContact&, bool checkDTG ) const
      {
         return false;
      }
      virtual bool commonSave( )
      {
         return false;
      }
      virtual void processMinosStanza( const std::string &methodName, MinosTestImport * const mt )
      {}
      virtual void check( )
      {}
      virtual void copyFromArg( ScreenContact & )
      {}

}
;

#endif
