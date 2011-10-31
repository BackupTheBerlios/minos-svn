/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef ContestH
#define ContestH 
//----------------------------------------------------------------------------
// This header covers the structure of contests, logs, etc

//class DisplayContestContact;
class BaseContact;
class ContestContact;
class ScreenContact;
class DisplayContestContact;
class ContestScore;
class DupContact
{
   public:
      BaseContact *dct;
      ScreenContact *sct;
      bool operator<( const DupContact& rhs ) const;
      bool operator==( const DupContact& rhs ) const;
      bool operator!=( const DupContact& rhs ) const;

      DupContact( BaseContact *c );
      DupContact( ScreenContact *c );
      DupContact();
      ~DupContact();
};
struct LtDup
{
   bool operator() ( const DupContact* s1, const DupContact* s2 ) const;
};

typedef codeproject::sorted_vector < DupContact *, true, LtDup > DupList;
typedef DupList::iterator DupIterator;
class dupsheet
{
      // a dupsheet is a sorted collection of (full LoggerContestLog) ContestContact records,
      // sorted by the callsign and logSequence number

   private:
      DupList ctList;
      DupContact *curdup; // points into dupsheet

   public:
      bool checkCurDup( BaseContact *nct, BaseContact *valp, bool insert );
      bool checkCurDup( ScreenContact *nct, BaseContact *valp, bool insert );
      bool isCurDup( BaseContact *nct ) const;
      bool isCurDup( ScreenContact *nct ) const;
      void clearCurDup();
      void clear();
      BaseContact *getCurDup();
      dupsheet();
      ~dupsheet();
};

enum SCOREMODE {PPKM, PPQSO};

struct LtLogSeq
{
   bool operator() ( const BaseContact* s1, const BaseContact* s2 ) const;
};
typedef codeproject::sorted_vector < BaseContact *, true, LtLogSeq > LogList;
typedef LogList::iterator LogIterator;

struct StrLess
{
   bool operator() ( const std::string &s1, const std::string &s2 ) const
   {
      return stricmp( s1.c_str(), s2.c_str() ) < 0;
   }
};
typedef codeproject::sorted_vector < std::string, true, StrLess > OperatorList;
typedef OperatorList::iterator OperatorIterator;

class BaseContestLog
{
      friend class MinosTestImport;
      friend class MonitoredLog;
      // This is the basis behind all variants - currently we have Logger and Monitor
      // which hold slightly different info, and more importantly handle backing store
      // totally differently

   protected:
      int stanzaCount;
      unsigned long nextBlock;
      bool suppressProtected;
      bool unwriteable;

   public:
      int slotno;
      int unfilledCount;

      BaseContestLog();
      virtual ~BaseContestLog();

      int getStanzaCount()
      {
         return stanzaCount;
      }

      // The contest details

      // "Real" basic contest data that needs monitoring
      // and provide the "front sheet" data

      MinosItem<bool> protectedContest;

      MinosItem<std::string> name;         // name of contest
      MinosItem<std::string> location;

      callsign mycall;  // CONTAINS MinosItem
      locator myloc;  // CONTAINS MinosItem

      MinosItem<bool> allowLoc4;
      MinosItem<bool> allowLoc8;

      MinosItem<bool> RSTField;
      MinosItem<bool> serialField;
      MinosItem<bool> locatorField;
      MinosItem<bool> QTHField;

      MinosItem<std::string> power;
      MinosItem<std::string> mode;

      MinosItem<std::string> band;
      MinosItem<bool> otherExchange;
      MinosItem<bool> countryMult;
      MinosItem<bool> locMult;
      MinosItem<bool> GLocMult;
      MinosItem<bool> districtMult;
      MinosItem<bool> powerWatts;
      MinosItem<SCOREMODE> scoreMode;
      MinosItem<std::string> DTGStart;
      MinosItem<std::string> DTGEnd;

	  MinosItem<int> bearingOffset;

      // dirty info is only relevant when it is being editted
      // but needs to stay with the data

      virtual void clearDirty();
      virtual void setDirty();
      bool isReadOnly( void )
      {
         return (protectedContest.getValue() && ! suppressProtected) || unwriteable;
      }
      bool isProtected( void )
      {
         return protectedContest.getValue();
      }
      void setProtected( bool s  )
      {
         if (protectedContest.getValue() && !s)
         {
            suppressProtected = true;
         }
         else if (s)
         {
            suppressProtected = false;
            protectedContest.setValue( true );
         }
      }
      bool isProtectedSuppressed( void )
      {
         return suppressProtected;
      }
      void setProtectedSuppressed( bool s)
      {
         suppressProtected = s;
      }
      void setUnwriteable( bool s )
      {
         unwriteable = s ;
      }
      bool isUnwriteable( )
      {
         return unwriteable;
      }
      // end of contest details

      // The log itself

      LogList ctList;

      // Ancilliary variables

      MinosItem<std::string> currentOp1;         // current main op - derived from contacts
      MinosItem<std::string> currentOp2;         // current second op - derived from contacts

      OperatorList oplist;

      std::string opsQSO1;
      std::string opsQSO2;

      int maxSerial;
      long contestScore;

      bool locValid;
      double ode;
      double odn;
      double cosodn ;            /* cos of odn */
      double sinodn ;            /* sin of odn */

      std::string cfileName;
      std::string publishedName;

      // duplicate sheet

      BaseContact *validationPoint;   // contact from log list to be treated
      dupsheet DupSheet;
      int nextScan;

      // Mult stuff

      int nctry;
      int ndistrict;
      int nlocs;

      int *districtWorked;
      int *countryWorked;
      LocList locs;

      int getDistrictsWorked( int item )
      {
         return districtWorked[ item ];
      }
      int getCountriesWorked( int item )
      {
         return countryWorked[ item ];
      }
      int getValidQSOs();

      // stats data

      int QSO1, QSO2, QSO1p, QSO2p;
      long kms1, kms2, kms1p, kms2p;
      int mults1, mults2, mults1p, mults2p;
      bool updateStat( BaseContact *cct );
      void updateStats();
      char lasttchar;

      // methods

      // common file stuff
      virtual bool commonSave( bool newfile )
      {
         return false;
      }
      virtual void closeFile( void )
      {}
      virtual bool GJVload( void )
      {
         return false;
      }

      virtual bool minosSaveFile( bool newfile )
      {
         return false;
      }
      /*
      virtual bool minosSaveBaseContact( const BaseContact *lct){return false;}
      */
      virtual bool minosSaveContestContact( const ContestContact *lct )
      {
         return false;
      }

      // Log monitoring

      virtual void processMinosStanza( const std::string &methodName, MinosTestImport * const mt );
      virtual bool getStanza( unsigned int stanza, std::string &stanzaData );
      virtual void setStanza( unsigned int stanza, int stanzaStart );

      // general

      void validateLoc( void );
      void getMatchText( BaseContact *, std::string &, const BaseContestLog *const ct ) const;
      void getMatchField( BaseContact *pct, int col, std::string &disp, const BaseContestLog *const ct ) const;
      void scanContest( void );
      void setScore( std::string & );
      bool isCurDup( BaseContact *) const;

      virtual void getScoresTo(ContestScore &cs, TDateTime limit);

      // manipulation of contact list

      int getContactCount( void );
      unsigned int indexOf( BaseContact * item );
      BaseContact *pcontactAtSeq( unsigned long logSequence );
      BaseContact *pcontactAt( unsigned int offset );

      //      virtual void makeContact( bool time_now, DisplayContestContact *&){}
      virtual void makeContact( bool time_now, BaseContact *& );
      BaseContact* findNextUnfilledContact();

      void freeAll();

      // calcs

      void disbear( double lon, double lat, double &dist, int &brg ) const;
      int CalcNearest( const std::string &scalcloc );
      bool getsdist( const char *loc, char *minloc, double &mindist );
      BaseContact *getBestDX( void );
      std::string dateRange( DTG dstyle );
      bool checkTime(const dtg &t);

};
class ContestScore
{
   public:
      char brcc1;
      char brcc2;
      char brcc3;
      char brcc4;
      char brloc1;
      char brloc2;

      std::string name;
      int nqsos;
      int contestScore;
      int nctry;
      int ndistrict;
      int nlocs;
      int nmults;
      int totalScore;

      ContestScore(BaseContestLog *, TDateTime limit);
      std::string disp();
};

#endif
