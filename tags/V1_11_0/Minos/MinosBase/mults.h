/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef MultsH
#define MultsH 
//----------------------------------------------------------------------------
class DistrictEntry;
class DistrictSynonym;
class DistrictList;
class CountrySynonymList;
class CountryEntry;
class CountrySynonym;					 //
class CountryList;
class MultEntry;
class BaseContestLog;

const UKREGIONS = 10;

enum eMultGridCols {ectCall, ectWorked, ectLocator, ectBearing, ectName, ectOtherCalls,
                    ectMultMaxCol
                   };

#define GLIST_PREFIX_LEN 5
class GlistEntry
{
   public:
      GlistEntry( const std::string &cd, const std::string &syn );
      virtual ~GlistEntry();

      bool operator<( const GlistEntry& rhs ) const;
      bool operator==( const GlistEntry& rhs ) const;
      bool operator!=( const GlistEntry& rhs ) const;

      std::string synPrefix;
      std::string dupPrefix;
};
class MultEntry
{
   public:
      int listOffset;
      locator central;	// central point to take bearings to
      std::string realName;

      MultEntry( const std::string &name, const std::string &cloc );
      virtual ~MultEntry();

      virtual std::string str( bool ) = 0;

      virtual void addSynonyms( std::string & );
};
#define DISTRICT_CODE_LENGTH 2
class DistrictEntry : public MultEntry
{
   public:

      std::string districtCode; // RSGB code
      CountryEntry * country1; // country containing district
      CountryEntry * country2; // country containing district

      DistrictEntry( const std::string &cd, const std::string &name, const std::string &prefix, const std::string &prefix2, const std::string &cloc );
      virtual ~DistrictEntry();
      bool operator<( const DistrictEntry& rhs ) const;
      bool operator==( const DistrictEntry& rhs ) const;
      bool operator!=( const DistrictEntry& rhs ) const;

      virtual std::string str( bool );
      virtual void addSynonyms( std::string & );
};

class DistrictSynonym
{
   public:
      DistrictSynonym( const std::string &cd, const std::string &syn );
      virtual ~DistrictSynonym();

      std::string synonym;
      DistrictEntry *district;
      bool operator<( const DistrictSynonym& rhs ) const;
      bool operator==( const DistrictSynonym& rhs ) const;
      bool operator!=( const DistrictSynonym& rhs ) const;
};

class CountryEntry : public MultEntry
{
      char distLimit;
   public:
      std::string basePrefix;
      std::string continent;

      int districtLimit( void );
      bool hasDistricts( void );

      CountryEntry( const std::string &continent, const std::string &prefix, const std::string &name, const std::string &cloc );
      virtual ~CountryEntry();
      virtual std::string str( bool );
      virtual void addSynonyms( std::string & );
      bool operator<( const CountryEntry& rhs ) const;
      bool operator==( const CountryEntry& rhs ) const;
      bool operator!=( const CountryEntry& rhs ) const;
};

class CountrySynonym
{
   public:
      std::string synPrefix;
      CountryEntry * country;

      CountrySynonym( const std::string &syn, const std::string &prefix );
      virtual ~CountrySynonym();

      void getDupPrefix( std::string & );
      virtual void synCat( std::string &add_buff );

      virtual int compare( const CountrySynonym & ) const;

      bool operator<( const CountrySynonym& rhs ) const;
      bool operator==( const CountrySynonym& rhs ) const;
      bool operator!=( const CountrySynonym& rhs ) const;
};

class LocSquare
{
   public:
      LocSquare( const std::string &loc );
      unsigned char *map( int num ); // give count char for loc num
      unsigned char *map( char *num ); // give count char for loc num
      void clear( void );

      char loc[ 3 ]; // two letter main square

      unsigned char numbers[ 10 ][ 10 ]; // map of parts worked
      bool operator<( const LocSquare& rhs ) const;
      bool operator==( const LocSquare& rhs ) const;
      bool operator!=( const LocSquare& rhs ) const;

};

struct LocSqCmp
{
   bool operator() ( const LocSquare *s1, const LocSquare *s2 ) const
   {
      return * s1 < *s2;
   }
};

typedef codeproject::sorted_vector < LocSquare *, true, LocSqCmp > LocSquareList;
typedef LocSquareList::iterator LocSquareIterator;
class LocList
{
   public:
      LocSquareList llist;
      LocList( void );
      virtual ~LocList();
      void freeAll();
};

class MultLists
{
   public:
      static MultLists *getMultLists();
      MultLists();
      virtual ~MultLists();

      //      void addCountry( bool addsyn ) = 0;
      virtual CountrySynonym *searchCountrySynonym( const std::string &syn ) = 0;
      virtual DistrictEntry *searchDistrict( const std::string &syn ) = 0;

      virtual int getCtryListSize() = 0;
      virtual int getDistListSize() = 0;
      virtual CountryEntry *getCtryForPrefix( const std::string &forcedMult ) = 0;
      virtual std::string getCtryListText( int item, int Column, BaseContestLog *const ct ) = 0;
      virtual std::string getDistListText( int item, int Column, BaseContestLog *const ct ) = 0;
      virtual CountryEntry * getCtryListAt( int index ) = 0;
      virtual int getCtryListIndexOf( CountryEntry * ) = 0;
      virtual int getDistListIndexOf( DistrictEntry * ) = 0;
};
#endif
