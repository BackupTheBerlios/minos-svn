/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "base_pch.h"
#pragma hdrstop
#include "MultsImpl.h"


//============================================================================
// prefix/mult count
struct DistCount
{
   char prefix[ 3 ];
   char dcount;
};
DistCount distCounts[] =
   {
      {"G", 1},
      {"GD", 1},
      {"GI", 6},
      {"GJ", 1},
      {"GM", 3},
      {"GU", 1},
      {"GW", 1},
      {"", 0}
   };
//============================================================================
GlistEntry::GlistEntry( const std::string &syn, const std::string &dup )
{
   synPrefix = trim( syn );
   dupPrefix = trim( dup );
}
GlistEntry::~GlistEntry()
{}

bool GlistEntry::operator<( const GlistEntry& rhs ) const
{
   // p1 is from list; p2 is the one being searched for

   int res = stricmp( synPrefix, rhs.synPrefix );
   return res < 0;
}
bool GlistEntry::operator==( const GlistEntry& rhs ) const
{
   // p1 is from list; p2 is the one being searched for

   int res = stricmp( synPrefix, rhs.synPrefix );
   return res == 0;
}
bool GlistEntry::operator!=( const GlistEntry& rhs ) const
{
   // p1 is from list; p2 is the one being searched for

   int res = stricmp( synPrefix, rhs.synPrefix );
   return res != 0;
}

GlistList::GlistList( void )
{}
GlistList::~GlistList()
{}
void GlistList::load( void )
{
   loadEntries( ".\\Configuration\\prefix.SYN", "prefix synonyms file" );
}
bool GlistList::procLine( char **a )
{
   char * syn = a[ 0 ];
   char *dup = a[ 1 ];

   GlistEntry *gle = new GlistEntry( syn, dup );
   insert( gle );
   return true;
}

//======================================================================
MultEntry::MultEntry( const std::string &name, const std::string &cloc ) : listOffset( -1 )
{
   realName = trim( name );

   // set up central
   central.loc.setValue( trim( cloc ).substr( 0, 6 ) );
}
MultEntry::~MultEntry()
{}
void MultEntry::addSynonyms( std::string &s )
{
   s = "";
}
//======================================================================
DistrictEntry::DistrictEntry( const std::string &cd, const std::string &name, const std::string &prefix, const std::string &prefix2, const std::string &cloc ) :
      MultEntry( name, cloc ), country1( 0 ), country2( 0 )
{
   // set district code to cd

   districtCode = cd;

   // search country list for the prefix

   for ( MultList < CountryEntry * >::iterator i = MultListsImpl::getMultLists() ->ctryList.begin(); i != MultListsImpl::getMultLists() ->ctryList.end(); i++ )
   {
      if ( stricmp( ( *i ) ->basePrefix, prefix2 ) == 0 )
      {
         country2 = *i;
         break;
      }
   }
   for ( MultList < CountryEntry * >::iterator i = MultListsImpl::getMultLists() ->ctryList.begin(); i != MultListsImpl::getMultLists() ->ctryList.end(); i++ )
   {
      if ( stricmp( ( *i ) ->basePrefix, prefix ) == 0 )
      {
         country1 = *i;
         break;
      }
   }
}
DistrictEntry::~DistrictEntry()
{
   // no need to delete any components
}
std::string DistrictEntry::str( bool longdisp )
{
   std::string temp;
   temp = districtCode;
   if ( longdisp )
   {
      temp += "(";
      if ( country1 )
         temp += country1->basePrefix;
      if ( country2 )
      {
         temp += "/" + country2->basePrefix;
      }
      temp += ")";
   }
   return temp;
}
void DistrictEntry::addSynonyms( std::string &s )
{
   s = "";
}
bool DistrictEntry::operator<( const DistrictEntry& rhs ) const
{
   int res = stricmp( districtCode, rhs.districtCode );
   return res < 0;
}
bool DistrictEntry::operator==( const DistrictEntry& rhs ) const
{
   int res = stricmp( districtCode, rhs.districtCode );
   return res == 0;
}
bool DistrictEntry::operator!=( const DistrictEntry& rhs ) const
{
   int res = stricmp( districtCode, rhs.districtCode );
   return res != 0;
}


//======================================================================
DistrictSynonym::DistrictSynonym( const std::string &cd, const std::string &syn ) :
      district( 0 )
{
   synonym = syn;

   // find district entry from cd code

   for ( MultList < DistrictEntry * >::iterator i = MultListsImpl::getMultLists() ->distList.begin(); i != MultListsImpl::getMultLists() ->distList.end(); i++ )
   {
      if ( stricmp( ( *i ) ->districtCode, cd ) == 0 )
      {
         district = *i;
         break;
      }
   }
}
DistrictSynonym::~DistrictSynonym()
{}
bool DistrictSynonym::operator<( const DistrictSynonym& rhs ) const
{
   int res = stricmp( synonym, rhs.synonym );
   return res < 0;
}
bool DistrictSynonym::operator==( const DistrictSynonym& rhs ) const
{
   int res = stricmp( synonym, rhs.synonym );
   return res == 0;
}
bool DistrictSynonym::operator!=( const DistrictSynonym& rhs ) const
{
   int res = stricmp( synonym, rhs.synonym );
   return res != 0;
}
//======================================================================
DistrictList::DistrictList( void )
{}
DistrictList::~DistrictList()
{
   // nothing to delete
}
int DistrictList::slen( bool longver )
{
   return longver ? 10 : 2;
}
void DistrictList::load( void )
{
   loadEntries( ".\\Configuration\\district.CTL", "District Control File" );

   for ( unsigned int i = 0; i < size(); i++ )
   {
      //      reinterpret_cast<MultEntry *>( at( i ) ) ->listOffset = i;
      at( i ) ->listOffset = i;
   }
}
bool DistrictList::procLine( char **a )
{
   char * cd = a[ 0 ];
   char *cname = a[ 1 ];
   char *prefix = a[ 2 ];
   char *prefix2 = a[ 3 ];
   char *cloc = a[ 4 ];

   DistrictEntry *dte = new DistrictEntry( cd, cname, prefix, prefix2, cloc );
   if ( !insert( dte ).second )
      delete dte;
   return true;
}
int DistrictList::getWorked( int item, BaseContestLog *const ct )
{
   if ( ct )
      return ct->getDistrictsWorked( item );
   else
      return 0;
}
//======================================================================
DistrictSynonymList::DistrictSynonymList( void )
{}
DistrictSynonymList::~DistrictSynonymList()
{
   // nothing to delete
}

void DistrictSynonymList::load( void )
{
   loadEntries( ".\\Configuration\\district.SYN", "District Synonym File" );
}
bool DistrictSynonymList::procLine( char **a )
{
   char * cd = a[ 0 ];
   char *cname = a[ 1 ];

   DistrictSynonym *dse = new DistrictSynonym( cd, cname );

   bool added = false;

   if ( dse->district )
      added = insert( dse ).second;

   if ( !added )
      delete dse;
   return true;
}
static bool compdistnames( DistrictEntry *ce, const std::string &syn )
{
   int len = ce->realName.length();
   for ( int i = 0; i < len; i++ )
   {
      if ( ce->realName[ i ] == '(' )
      {
         len = i;
         break;
      }
   }

   if ( strnicmp( ce->realName, syn, len ) == 0 )
      return true;

   return false;
}
static DistrictEntry * searchDistrict( const std::string &syn )
{
   // given a random string, look for an entry or a synonym
   for ( MultList < DistrictEntry * >::iterator i = MultListsImpl::getMultLists() ->distList.begin(); i != MultListsImpl::getMultLists() ->distList.end(); i++ )
   {
      if ( stricmp( ( *i ) ->districtCode, syn ) == 0 )
      {
         return ( *i );
      }
   }

   for ( MultList < DistrictSynonym * >::iterator i = MultListsImpl::getMultLists() ->distSynList.begin(); i != MultListsImpl::getMultLists() ->distSynList.end(); i++ )
   {
      if ( stricmp( ( *i ) ->synonym, syn ) == 0 )
      {
         return ( *i ) ->district;
      }
   }

   for ( MultList < DistrictEntry * >::iterator i = MultListsImpl::getMultLists() ->distList.begin(); i != MultListsImpl::getMultLists() ->distList.end(); i++ )
   {
      if ( compdistnames( ( *i ), syn ) )
      {
         return * i;
      }
   }

   return 0;
}
//======================================================================
CountryEntry::CountryEntry( const std::string &continent, const std::string &prefix,
                            const std::string &name, const std::string &cloc ) :
      MultEntry( name, cloc ), distLimit( -1 ), continent( continent )
{
   basePrefix = trim( prefix );
}
CountryEntry::~CountryEntry()
{}
int CountryEntry::districtLimit()
{
   //	search distCounts
   if ( distLimit >= 0 )
      return distLimit;

   DistCount *dc = &distCounts[ 0 ];
   while ( dc->dcount )
   {
      if ( stricmp( basePrefix, dc->prefix ) == 0 )
      {
         distLimit = dc->dcount;
         return distLimit;
      }
      dc++;
   }
   return 0;
}
bool CountryEntry::hasDistricts()
{
   if ( districtLimit() > 0 )
      return true;
   return false;
}
std::string CountryEntry::str( bool )
{
   return basePrefix;
}
void CountryEntry::addSynonyms( std::string &s )
{
   // add list of synonyms to the display buffer
   s = ":";
   for ( MultList < CountrySynonym * >::iterator i = MultListsImpl::getMultLists() ->ctrySynList.begin(); i != MultListsImpl::getMultLists() ->ctrySynList.end(); i++ )
   {
      //    if ( (*((*i)->country)) == this)
      if ( ( *i ) ->country == this )
      {
         if ( s.length() < 180 )   		// should really use the correct value here!
         {
            s += " ";
            ( *i ) ->synCat( s );
         }
      }
   }
}
bool CountryEntry::operator<( const CountryEntry& rhs ) const
{
   int res = stricmp( basePrefix, rhs.basePrefix );
   return res < 0;
}
bool CountryEntry::operator==( const CountryEntry& rhs ) const
{
   int res = stricmp( basePrefix, rhs.basePrefix );
   return res == 0;
}
bool CountryEntry::operator!=( const CountryEntry& rhs ) const
{
   int res = stricmp( basePrefix, rhs.basePrefix );
   return res != 0;
}
//======================================================================
static CountrySynonym *searchCountrySynonym( const std::string &syn )
{
   CountrySynonym test( syn, "" );

   MultList < CountrySynonym * > ::iterator cs = std::lower_bound( MultListsImpl::getMultLists() ->ctrySynList.begin(),
         MultListsImpl::getMultLists() ->ctrySynList.end(),
         &test, MultCmp < CountrySynonym * >() );

   if ( cs == MultListsImpl::getMultLists() ->ctrySynList.end() || !( *( *cs ) == test ) )
   {
      cs = MultListsImpl::getMultLists() ->ctrySynList.end();
   }
   if ( cs == MultListsImpl::getMultLists() ->ctrySynList.end() )
      return 0;
   else
      return ( *cs );
}
static void makeCountrySynonym( const std::string &ssyn, const std::string &sprefix )
{
   // search country list for the prefix

   std::string syn = trim( ssyn );
   std::string prefix = trim( sprefix );

   if ( strchr( syn.c_str(), '-' ) )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( ( std::string( "Synonym ranges no longer allowed : " ) + ssyn + " for " + sprefix ).c_str() );
      return ;
   }

   CountryEntry *ctry = 0;
   for ( MultList < CountryEntry * >::iterator i = MultListsImpl::getMultLists() ->ctryList.begin(); i != MultListsImpl::getMultLists() ->ctryList.end(); i++ )
   {
      if ( stricmp( ( *i ) ->basePrefix, prefix ) == 0 )
      {
         ctry = ( *i );
         break;
      }
   }
   if ( !ctry )
      return ;		// as it will be unsuccessfull anyway

   CountrySynonym *cts = searchCountrySynonym( syn );
   if ( cts && ( cts->country == ctry ) )
      return ;		// as already there

   cts = new CountrySynonym( syn, prefix );

   bool added = false;
   if ( cts->country )
   {
      added = MultListsImpl::getMultLists() ->ctrySynList.insert( cts ).second;    // must add to the syn list...
   }
   if ( !added )
      delete cts;
}
CountrySynonym::CountrySynonym( const std::string &ssyn, const std::string &sprefix ) :
      country( 0 )
{
   std::string syn = trim( ssyn );
   std::string prefix = trim( sprefix );

   if ( prefix.length() )   		// allow for stack based version to search by
   {
      synPrefix = syn;
      // search country list for the prefix
      for ( MultList < CountryEntry * >::iterator i = MultListsImpl::getMultLists() ->ctryList.begin(); i != MultListsImpl::getMultLists() ->ctryList.end(); i++ )
      {
         if ( stricmp( ( *i ) ->basePrefix, prefix ) == 0 )
            //          country = i;
            country = *i;
      }
   }
   else
   {
      synPrefix = syn;
   }
}
CountrySynonym::~CountrySynonym()
{}
void CountrySynonym::getDupPrefix( std::string &sprefix2 )
{
   std::string prefix2 = trim( sprefix2 );
   //	search Glist
   // dup_prefix_offset was used to speed this up. We may need something similar
   // None found, then don't change prefix2
   for ( MultList < GlistEntry * >::iterator i = MultListsImpl::getMultLists() ->glist.begin(); i != MultListsImpl::getMultLists() ->glist.end(); i++ )
   {
      if ( stricmp( ( *i ) ->synPrefix, prefix2 ) == 0 )
      {
         sprefix2 = ( *i ) ->dupPrefix;
         break;
      }
   }
   return ;
}
bool CountrySynonym::operator<( const CountrySynonym& rhs ) const
{
   int res = compare( rhs );
   return res < 0;
}
bool CountrySynonym::operator==( const CountrySynonym& rhs ) const
{
   int res = compare( rhs );
   return res == 0;
}
bool CountrySynonym::operator!=( const CountrySynonym& rhs ) const
{
   int res = compare( rhs );
   return res != 0;
}
int CountrySynonym::compare( const CountrySynonym &cs ) const
{
   // p1 is from list; p2 is the one being searched for
   int res;

   res = stricmp( synPrefix, cs.synPrefix );
   if ( res < 0 )
      return -1;
   else
      if ( res == 0 )
         return 0;
      else
         return 1;
}
void CountrySynonym::synCat( std::string &add_buff )
{
   add_buff += synPrefix;
}
//======================================================================
CountryList::CountryList( void )
{}
CountryList::~CountryList()
{
   // nothing to delete
}
int CountryList::slen( bool )
{
   return 9;
}
void CountryList::load( void )
{
   // load ignores from the ini file
   /*
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpShowContinentEU, contlist[ 0 ].allow );
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpShowContinentAS, contlist[ 1 ].allow );
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpShowContinentAF, contlist[ 2 ].allow );
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpShowContinentOC, contlist[ 3 ].allow );
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpShowContinentSA, contlist[ 4 ].allow );
   TContestApp::getContestApp() ->displayBundle.getBoolProfile( edpShowContinentNA, contlist[ 5 ].allow );
   */
   loadEntries( ".\\Configuration\\cty.dat", "CT9 Country File" );

   for ( unsigned int i = 0; i < size(); i++ )
      at( i ) ->listOffset = i;
}
bool CountryList::procLine( char ** )
{
   return true;
}
// lat, longi to be in degrees, -ve for W or S
extern int geotoloc( double lat, double longi, char *&gridref );
//==============================================================================

// Parse the CT9 CTY.DAT format
/*
------------------------------------------------------------------------
 
 
      CTY.DAT file format
 
CTY.DAT is a new file format for CT Version 9. It includes more
information than the .CTY files for previous versions of CT. This allows
CT to calculate, in real time, beam heading and sun times. It also means
that a single file can be used for all DX contests. The format is as
follows. Note that the fields are aligned in columns and spaced out for
readability only. It is the ":" at the end of each field that acts as a
delimiter for that field:
 
Column Length Description
1 26 Country Name
27 5 CQ Zone
32 5 ITU Zone
37 5 2-letter continent abbreviation
42 9 Latitude in degrees, + for North
51 10 Longitude in degrees, + for West
61 9 Local time offset from GMT
70 6 Primary DXCC Prefix (A "*" preceding this prefix indicates that the
country is on the DARC WAEDC list, and counts in CQ-sponsored contests,
but not ARRL-sponsored contests).
 
Alias DXCC prefixes (including the primary one) follow on consecutive
lines, separated by ",". If there is more than one line, subsequent
lines begin with the "&" continuation character. A ";" terminates the
last prefix in the list.
 
The following special characters can be applied to an alias prefix:
 
(#) Override CQ Zone
[#] Override ITU Zone
<#/#> Override latitude/longitude
{aa} Override Continent
 
See the examples on page 27 in the CT9 manual.
 
------------------------------------------------------------------------
// Some samples:
Chesterfield Is.:         32:  56:  OC:  -19.90:  -158.30:   -11.0:  TX0:
    TX0;
Benin:                    35:  46:  AF:    6.50:    -2.60:    -1.0:  TY:
    TY;
Mali:                     35:  46:  AF:   12.70:     8.00:     0.0:  TZ:
    TZ;
European Russia:          16:  29:  EU:   55.80:   -37.60:    -3.0:  UA:
    R,U;
Kaliningradsk:            15:  29:  EU:   55.00:   -20.50:    -3.0:  UA2:
    R2,RA2,RB2,RC2,RD2,RE2,RF2,RG2,RH2,RI2,RJ2,RK2,RL2,RM2,RN2,RP2,RQ2,RR2,
    RS2,RT2,RU2,RV2,RW2,RX2,RY2,RZ2,U2,UA2,UB2,UC2,UD2,UF2,UG2,UH2,UI2;
Asiatic Russia:           17:  30:  AS:   55.00:   -83.00:    -7.0:  UA9:
    R0,R3F/9,R7(17),R8,R8T(18)[32],R8V(18)[32],R9,R9I(18),R9M(17),R9S(17),R9W(17),
    RA0,RA7(17),RA8,RA8T(18)[32],RA8V(18)[32],RA9,RA9I(18),RA9M(17),RA9S(17),
    .
    .
    .
    UI9,UI9I(18),UI9M(17),UI9S(17),UI9W(17);
------------------------------------------------------------------------
*/
void CountryList::loadEntries( const std::string &fname, const std::string &fmess )
{
   // load a CT9 formatted list
   TEMPBUFF( countrybuff, 256 );

   std::ifstream istr( fname.c_str() ); // should close when it goes out of scope
   if ( !checkFileOK( istr, fname, fmess ) )
      return ;

   // loop through file, parsing each line
   // ignore comment lines. (# prefix)


   while ( istr.getline( countrybuff, 255 ) )
   {
      char * a[ 9 ]; // allow one extra as separator terminated line

      if ( countrybuff[ 0 ] == '#' )      // only allow # comments
         continue;   // skip comment lines

      bool sep2seen;
      parseLine( countrybuff, ':', a, 9, 0, sep2seen );

      std::string mainPrefix( a[ 7 ] );
      bool skip = ( mainPrefix[ 0 ] == '*' );
      if ( !skip )
      {
         // lat, longi to be in degrees, -ve for W or S

         // name: continent: lat: long: DXCC Id:
         // BUT CT uses + for N, + for W
         double lat = 0.0;
         double longi = 0.0;

         lat = atof( a[ 4 ] );
         longi = atof( a[ 5 ] );

         TEMPBUFF( gridref, 15 );
         char *grid = gridref;
         geotoloc( lat, -longi, grid );	// kill temporary warning

         CountryEntry * cte = new CountryEntry( a[ 3 ], a[ 7 ], a[ 0 ], gridref );
         if ( !insert( cte ).second )
         {
            delete cte;
            cte = 0;
         }
      }
      // now we go through following lines up to a semicolon terminator
      sep2seen = false;
      while ( !sep2seen && istr.getline( countrybuff, 255 ) )
      {
         // elements are comma separated, including the end of line
         // may be white space around
         // elements may be complete callsigns
         // each one wants to be added to the CountrySynonymList
         char * b[ 99 ];

         if ( countrybuff[ 0 ] == '#' )      // only allow # comments
            continue;   // skip comment lines

         parseLine( countrybuff, ',', b, 99, ';', sep2seen );
         int i = 0;
         while ( !skip && i < 99 && b[ i ] && b[ i ][ 0 ] )
         {
            int bracket = strcspn( b[ i ], "({[<" );
            if ( bracket )
               b[ i ][ bracket ] = 0;   // chop off the brackets
            makeCountrySynonym( b[ i ], mainPrefix );
            i++;
         }
      }
   }
}
int CountryList::getWorked( int item, BaseContestLog *const ct )
{
   if ( ct )
      return ct->getCountriesWorked( item );
   else
      return 0;
}

//======================================================================
CountrySynonymList::CountrySynonymList( void )
{}
CountrySynonymList::~CountrySynonymList()
{
   // nothing to delete
}
void CountrySynonymList::load( void )
{
   loadEntries( ".\\Configuration\\cty.SYN", "Country Synonym File" );
}
bool CountrySynonymList::procLine( char **a )
{
   for ( int i = 1; i < 255 && a[ i ] && a[ i ][ 0 ]; i++ )
   {
      makeCountrySynonym( a[ i ], a[ 0 ] );
   }

   return true;
}
//======================================================================
LocList::LocList( void )
{}
LocList::~LocList()
{
   freeAll();
}
void LocList::freeAll()
{
   for ( LocSquareIterator i = llist.begin(); i != llist.end(); i++ )
      delete ( *i );
   llist.clear();
}
//======================================================================
LocSquare::LocSquare( const std::string &locId )
{
   clear();
   loc[ 0 ] = toupper( locId[ 0 ] );
   loc[ 1 ] = toupper( locId[ 1 ] );
   loc[ 2 ] = 0;
}

unsigned char *LocSquare::map( char *num )
{
   if ( !isdigit( num[ 0 ] ) || !isdigit( num[ 1 ] ) )
      return 0;

   return &numbers[ num[ 0 ] - '0' ][ num[ 1 ] - '0' ];
}

unsigned char *LocSquare::map( int num )
{
   if ( ( num < 0 ) || ( num >= 100 ) )
      return 0;
   int dig2 = num % 100;
   int dig1 = ( num - dig2 ) / 100;

   return &numbers[ dig1 ][ dig2 ];
}

void LocSquare::clear( void )
{
   int i, j;
   for ( i = 0; i < 10; i++ )
      for ( j = 0; j < 10; j++ )
         numbers[ i ][ j ] = 0;
}

bool LocSquare::operator<( const LocSquare& rhs ) const
{
   return strncmp( loc, rhs.loc, 2 ) < 0;
}
bool LocSquare::operator==( const LocSquare& rhs ) const
{
   return strncmp( loc, rhs.loc, 2 ) == 0;
}
bool LocSquare::operator!=( const LocSquare& rhs ) const
{
   return strncmp( loc, rhs.loc, 2 ) != 0;
}

//======================================================================
MultListsImpl *MultListsImpl::multLists = 0;

MultListsImpl *MultListsImpl::getMultLists()
{
   static bool firstTime = true;
   if ( !multLists && firstTime )
   {
      firstTime = false;
      multLists = new MultListsImpl();
      multLists->loadMultFiles();
   }
   return multLists;
}
bool MultListsImpl::loadMultFiles( void )
{
   ctryList.load();
   ctrySynList.load();
   distList.load();
   distSynList.load();
   glist.load();
   /*
   std::ofstream os("c:\\temp\\multlist.txt");

   os << "================== country entries ========================" << std::endl;
   for (MultList < CountryEntry * >::iterator i = m->ctryList.begin(); i != m->ctryList.end(); i++)
   {
      os << (*i)->basePrefix + " " + (*i)->realName << std::endl;
   }
   os << (String("================== country synonyms ") + String(m->ctrySynList.size()).c_str() + "========================").c_str() << std::endl;
   for (MultList < CountrySynonym * >::iterator i = m->ctrySynList.begin(); i != m->ctrySynList.end(); i++)
   {
      std::string temp1 = (*i)->synPrefix;
   //      CountryEntry * country = *((*i)->country);
      CountryEntry * country = (*i)->country;
      std::string temp2 = country->basePrefix;
      os << (temp1 + " : " + temp2) << std::endl;
   }
   os << "================== district entries ========================" << std::endl;
   for (MultList < DistrictEntry * >::iterator i = m->distList.begin(); i != m->distList.end(); i++)
   {
      os << (*i)->districtCode << std::endl;
   }
   os << "================== district synonyms ========================" << std::endl;
   for (MultList < DistrictSynonym * >::iterator i = m->distSynList.begin(); i != m->distSynList.end(); i++)
   {
      os << ((*i)->synonym + " : " + ((*i)->district)->districtCode) << std::endl;
   }
   os << "================== Glist ========================" << std::endl;
   for (MultList < GlistEntry * >::iterator i = m->glist.begin(); i != m->glist.end(); i++)
   {
      os << ((*i)->synPrefix + " : " + (*i)->dupPrefix) << std::endl;
   }
   */ 
   return true;
}
MultListsImpl::MultListsImpl()
{
}
MultListsImpl::~MultListsImpl()
{
   ctrySynList.freeAll();
   distSynList.freeAll();
   ctryList.freeAll();
   distList.freeAll();
   multLists = 0;
}

int MultListsImpl::getCtryListSize()
{
   return ctryList.size();
}
int MultListsImpl::getDistListSize()
{
   return distList.size();
}
CountryEntry *MultListsImpl::getCtryForPrefix( const std::string &forcedMult )
{
   CountryEntry * ctryMult = 0;
   for ( MultList < CountryEntry * >::iterator i = MultListsImpl::getMultLists() ->ctryList.begin(); i != MultListsImpl::getMultLists() ->ctryList.end(); i++ )
   {
      if ( stricmp( ( *i ) ->basePrefix, forcedMult ) == 0 )
      {
         ctryMult = ( *i );
         break;
      }
   }
   return ctryMult;
}

//void MultListsImpl::addCountry( bool addsyn );
CountrySynonym *MultListsImpl::searchCountrySynonym( const std::string &syn )
{
   return ::searchCountrySynonym( syn );
}
DistrictEntry *MultListsImpl::searchDistrict( const std::string &syn )
{
   return ::searchDistrict( syn );
}
std::string MultListsImpl::getCtryListText( int item, int Column, BaseContestLog *const ct )
{
   return ctryList.getText( item, Column, ct );
}
std::string MultListsImpl::getDistListText( int item, int Column, BaseContestLog *const ct )
{
   return distList.getText( item, Column, ct );
}
CountryEntry * MultListsImpl::getCtryListAt( int index )
{
   return ctryList.at( index );
}

int MultListsImpl::getCtryListIndexOf( CountryEntry *e )
{
   return ctryList.indexOf( e );
}
int MultListsImpl::getDistListIndexOf( DistrictEntry *e )
{
   return distList.indexOf( e );
}

