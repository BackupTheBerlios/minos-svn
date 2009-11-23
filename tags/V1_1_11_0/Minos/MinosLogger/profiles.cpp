/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop
#include "MLogFile.h"
const std::string noneBundle = "<None>";

// Here we define all the possible profile entries in a section
// we use them for new sections in a bundle, and also to get
// values from a bundle.

// What do we do about variable entries? Do we allow them?
// What about about entries for other Minos programs
// where we won't know what is is relevant?

class INIFile;
class INISection;
class INIEntry //: public NameChain
{
   private:
      bool entryValid;
      std::string entryValue;
      bool entryDirty;

   public:
      std::string name;

      INIEntry( INISection *cb,
                const std::string &name, bool Valid );  // Constructor.

      ~INIEntry();        // Destructor.

      std::string getValue( void );
      void setValue( const std::string &Value );
      bool isValidEntry( void );
      bool isDirty()
      {
         return entryDirty;
      }
      void setClean()
      {
         entryDirty = false;
      }

};
typedef INIEntry *IniEntryPtr;
struct INIEntryCmp
{
   std::string cmpstr;
   INIEntryCmp( const std::string &s ) : cmpstr( s )
   {}

   bool operator() ( IniEntryPtr &s1 ) const
   {
      return stricmp( s1->name.c_str(), cmpstr.c_str() ) == 0;
   }
};
class INISection //: public NameChain
{
   private:
      bool entryValid;
      bool sectDirty;
   public:
      std::string name;

      INISection( INIFile *cb,
                  const std::string &name, bool Valid ); // Constructor.

      ~INISection();   // Destructor.

      bool isValidSection( void );

      std::vector <IniEntryPtr> entries;
      //      NameChain_base EntryList;
      bool isDirty();
      void setClean();

};
typedef INISection *IniSectionPtr;
struct INISectionCmp
{
   std::string cmpstr;
   INISectionCmp( const std::string &s ) : cmpstr( s )
   {}

   bool operator() ( IniSectionPtr &s1 ) const
   {
      return stricmp( s1->name.c_str(), cmpstr.c_str() ) == 0;
   }
};
class INIFile
{
      bool fileDirty;
   public:
      bool loadINIFile( void );
      bool writeINIFile( void );
      bool checkStat( void );
      bool checkKeyExists( const char *Section,
                           const char *Entry
                         );
      int getPrivateProfileString( const char *Section,
                                   const char *Entry,
                                   const char *DefaultValue,
                                   char *Buffer,
                                   int Size );

      int getPrivateProfileInt( const char *Section,
                                const char *Entry,
                                int DefaultValue );

      bool writePrivateProfileString( const char *Section,
                                      const char *Entry,
                                      const char *Buffer );

      bool dupSection( const std::string &oldname, const std::string &newname );
      //      NameChain_base SectionList;

      std::vector <IniSectionPtr> sections;
      bool fileLoaded;
      bool isDirty();
      void setClean();
      //      bool invalid;
      struct stat statbuf;

      std::string loadedFileName;

      INIFile( const std::string &name );
      ~INIFile();
};




//==============================================================================
INISection::INISection( INIFile *cb, const std::string &name, bool valid )
      : name( trim( name ) ), entryValid( valid ), sectDirty( false )
{
   cb->sections.push_back( this );
}

INISection::~INISection()
{
   // delete all entries
   for ( std::vector <IniEntryPtr>::iterator this_ent = entries.begin();
         this_ent != entries.end(); this_ent++ )
   {
      delete ( *this_ent );
   }
   entries.clear();
}

bool INISection::isValidSection( void )
{
   return entryValid;
}
bool INISection::isDirty( void )
{
   if ( sectDirty )
      return true;

   for ( std::vector <IniEntryPtr>::iterator thisEntry = entries.begin();
         thisEntry != entries.end(); thisEntry++ )
   {
      if ( ( *thisEntry ) ->isDirty() )
         return true;
   }
   return false;   // for now, just in case...
}
void INISection::setClean( void )
{
   //walk all sections and set each clean
   for ( std::vector <IniEntryPtr>::iterator thisEntry = entries.begin();
         thisEntry != entries.end(); thisEntry++ )
   {
      ( *thisEntry ) ->setClean();
   }
}
//==============================================================================


INIEntry::INIEntry( INISection *cb, const std::string &name, bool valid )
      : name( trim( name ) ), entryValid( valid ), entryDirty( false )
{
   cb->entries.push_back( this );
}

INIEntry::~INIEntry()
{}

std::string INIEntry::getValue( void )
{
   return entryValue;
}

void INIEntry::setValue( const std::string &value )
{
   if ( value != entryValue )
   {
      entryValue = value;
      entryDirty = true;
   }
}

bool INIEntry::isValidEntry( void )
{
   return entryValid;
}
//==============================================================================

INIFile::INIFile( const std::string &name ) : fileLoaded( false ),   /*invalid( true ),*/ fileDirty( false )
{
   memset( &statbuf, 0, sizeof( struct stat ) );
   loadedFileName = ExpandFileName( trim( name ).c_str() ).c_str() ;
   if ( !FileExists( loadedFileName.c_str() ) )
   {
      ForceDirectories( ExtractFilePath( loadedFileName.c_str() ).c_str() );
      int fh = FileCreate( loadedFileName.c_str() );
      if ( fh != -1 )
         FileClose( fh );
   }
}
INIFile::~INIFile()
{
   writePrivateProfileString( 0, 0, 0 );
   // delete all sections
   for ( std::vector <IniSectionPtr>::iterator thisSect = sections.begin(); thisSect != sections.end(); thisSect++ )
   {
      delete ( *thisSect );
   }
   sections.clear();
   fileLoaded = false;
}
bool INIFile::dupSection( const std::string &oldname, const std::string &newname )
{
   std::vector<IniSectionPtr>::iterator newSect = std::find_if( sections.begin(), sections.end(), INISectionCmp( newname ) );
   if ( newSect == sections.end() )
   {
      std::vector<IniSectionPtr>::iterator thisSect = std::find_if( sections.begin(), sections.end(), INISectionCmp( oldname ) );
      if ( thisSect != sections.end() )
      {
         INISection * oldsect = ( *thisSect );
         INISection *newsect = new INISection( this, newname, true );
         for ( std::vector <IniEntryPtr>::iterator this_ent = oldsect->entries.begin(); this_ent != oldsect->entries.end(); this_ent++ )
         {
            INIEntry *i = ( *this_ent );
            std::string n = i->name;
            std::string v = i->getValue();
            INIEntry *newent = new INIEntry( newsect, n, true );
            newent->setValue( v );
         }
         return true;
      }
      return false;
   }
   return false;  // section already exists
}

bool INIFile::checkStat( void )
{
   struct stat tempstat;
   int statret = stat( loadedFileName.c_str(), &tempstat );
   if ( statret )
      return true;	// failed => different(?)
   if ( statret == 0 && memcmp( &tempstat, &statbuf, sizeof( struct stat ) ) != 0 )
   {
      memcpy( &statbuf, &tempstat, sizeof( struct stat ) );
      return true;
   }
   return false;
}
bool INIFile::isDirty( void )
{
   if ( fileDirty )
      return true;

   for ( std::vector <IniSectionPtr>::iterator thisSect = sections.begin(); thisSect != sections.end(); thisSect++ )
   {
      if ( ( *thisSect ) ->isDirty() )
         return true;
   }
   return false;   // for now, just in case...
}
void INIFile::setClean( void )
{
   //walk all sections and set each clean
   for ( std::vector <IniSectionPtr>::iterator thisSect = sections.begin(); thisSect != sections.end(); thisSect++ )
   {
      ( *thisSect ) ->setClean();
   }
}

bool INIFile::writeINIFile( void )
{
   if ( !isDirty() )
      return true;

   //   if ( invalid )
   //      return false;

   FILE *inf;

   // here we need to stat the file to see if it has changed
   // - but what do we do if it has? We should have loaded
   // it VERY recently

   if ( ( inf = fopen( loadedFileName.c_str(), "wt" ) ) == 0 )
      return false;

   for ( std::vector <IniSectionPtr>::iterator thisSect = sections.begin(); thisSect != sections.end(); thisSect++ )
   {
      const std::string sname = ( *thisSect ) ->name;

      if ( ( *thisSect ) ->isValidSection() )
         fprintf( inf, "[%s]\n", sname.c_str() );

      for ( std::vector <IniEntryPtr>::iterator this_entry = ( *thisSect ) ->entries.begin(); this_entry != ( *thisSect ) ->entries.end(); this_entry++ )
      {
         const std::string name = ( *this_entry ) ->name;
         const std::string val = ( *this_entry ) ->getValue();
         if ( ( *this_entry ) ->isValidEntry() )
            fprintf( inf, "%s=%s\n", name.c_str(), val.c_str() );
         else
            fprintf( inf, "%s\n", val.c_str() ); // treat as annotation
      }
   }
   fclose( inf );

   setClean();
   // now stat the file so we can check for changes
   checkStat();
   return true;
}


bool INIFile::loadINIFile()
{
   FILE * inf;
   TEMPBUFF( buffer, 256 );
   INISection *thisSect;
   IniEntryPtr this_entry;
   bool realSections = false;

   if ( fileLoaded )
   {
      if ( !checkStat() )
         return false;			// no change, so don't re-read
      writePrivateProfileString( 0, 0, 0 );
      for ( std::vector <IniSectionPtr>::iterator thisSect = sections.begin(); thisSect != sections.end(); thisSect++ )
      {
         delete ( *thisSect );
      }
      sections.clear();
      fileLoaded = false;
   }

   fileLoaded = true;

   if ( ( inf = fopen( loadedFileName.c_str(), "rt" ) ) == 0 )
   {
      MinosParameters::getMinosParameters() ->mshowMessage( String( "Initialisation file \"" ) + loadedFileName.c_str() + "\" not found." );
      //      invalid = false;
      return false;
   }

   thisSect = new INISection( this, "?Comments", false );
   // create dummy section for leading comments

   while ( fgets( buffer, 256, inf ) != 0 )
   {
      TEMPBUFF ( Parameter, 256 );
      if ( strchr( buffer, '[' ) && ( sscanf( buffer, " [ %255[^\n]", Parameter ) == 1 ) )
      {
         char * p = strchr( Parameter, ']' );
         if ( p != 0 )
         {
            *p = '\0';
            thisSect = new INISection( this, Parameter, true );
            realSections = true;
            continue;
         }
         // and if no trailing ']' should we be lenient?
      }

      if ( buffer[ strlen( buffer ) - 1 ] == '\n' )
         buffer[ strlen( buffer ) - 1 ] = 0;		// take off trailing new line

      char *a[ 3 ];
      bool sep2seen;
      int scnt = parseLine( buffer, '=', a, 2, 0, sep2seen );
      trimr( a[ 0 ] );
      trimr( a[ 1 ] );

      if ( scnt )
      {
         this_entry = new INIEntry( thisSect, a[ 0 ], true );
         // somewhere we need to cope with quoted parameters
         this_entry->setValue( a[ 1 ] );
         this_entry->setClean();
      }
      else
      {
         // create comment entry
         this_entry = new INIEntry( thisSect, "??", false );
         this_entry->setValue( a[ 0 ] );
         this_entry->setClean();
      }
   }
   fclose( inf );
   // now stat the file so we can check for changes
   checkStat();

   /*
      if ( !realSections )
      {
         mshowMessage( String( "Initialisation file \"" ) + loadedFileName.c_str() + "\" invalid (no sections)." );
         invalid = true;
      }
      else
      {
         invalid = false;
      }
   */ 
   return realSections;
}

bool INIFile::checkKeyExists( const char *Section,
                              const char *Entry
                            )
{
   loadINIFile();

   std::vector<IniSectionPtr>::iterator thisSect = std::find_if( sections.begin(), sections.end(), INISectionCmp( Section ) );
   if ( thisSect != sections.end() )
   {
      if ( Entry == 0 )
         return true;
      std::vector<IniEntryPtr>::iterator this_entry = std::find_if( ( *thisSect ) ->entries.begin(), ( *thisSect ) ->entries.end(), INIEntryCmp( Entry ) );
      if ( this_entry != ( *thisSect ) ->entries.end() )
      {
         return true;
      }
   }
   return false;
}

int INIFile::getPrivateProfileString( const char *Section,
                                      const char *Entry,
                                      const char *DefaultValue,
                                      char *Buffer,
                                      int Size )
{
   int Length = 0;

   loadINIFile();

   std::vector<IniSectionPtr>::iterator thisSect = std::find_if( sections.begin(), sections.end(), INISectionCmp( Section ) );
   if ( thisSect == sections.end() )
   {
      const char * this_value = DefaultValue;
      strncpy( Buffer, this_value , Size - 1 );
      Buffer[ Size - 1 ] = 0;
   }
   else
   {
      if ( Entry == 0 )
      {
         /* build list of entry names in buffer */
         char * ptr = Buffer;
         char *bufend = &Buffer[ Size - 2 ]; // space for string null and end null

         for ( std::vector <IniEntryPtr>::iterator this_entry = ( *thisSect ) ->entries.begin(); this_entry != ( *thisSect ) ->entries.end(); this_entry++ )
         {
            if ( ( *this_entry ) ->isValidEntry() && ( ( int ) ( *this_entry ) ->name.length() >= ( bufend - ptr ) ) )
               break;

            if ( ( *this_entry ) ->isValidEntry() )
            {
               strcpy( ptr, ( *this_entry ) ->name.c_str() );
               ptr += ( *this_entry ) ->name.length() + 1;
            }
         }
         *ptr++ = '\0';
         Length = ( int ) ( ptr - Buffer );
      }
      else
      {
         std::vector<IniEntryPtr>::iterator this_entry = std::find_if( ( *thisSect ) ->entries.begin(), ( *thisSect ) ->entries.end(), INIEntryCmp( Entry ) );
         if ( this_entry == ( *thisSect ) ->entries.end() )
         {
            strncpy( Buffer, DefaultValue, Size - 1 );
            Buffer[ Size - 1 ] = 0;
         }
         else
         {
            std::string this_value = ( *this_entry ) ->getValue();
            strncpy( Buffer, this_value.c_str() , Size - 1 );
            Buffer[ Size - 1 ] = 0;
         }
         Length = strlen( Buffer );
      }
   }
   return Length;
}


int INIFile::getPrivateProfileInt( const char *Section,
                                   const char *Entry,
                                   int DefaultValue )
{
   if ( ( Section == 0 ) || ( Entry == 0 ) )
      return 0;

   loadINIFile();

   std::vector<IniSectionPtr>::iterator thisSect = std::find_if( sections.begin(), sections.end(), INISectionCmp( Section ) );
   if ( thisSect == sections.end() )
   {
      return DefaultValue;
   }

   std::vector<IniEntryPtr>::iterator this_entry = std::find_if( ( *thisSect ) ->entries.begin(), ( *thisSect ) ->entries.end(), INIEntryCmp( Entry ) );
   if ( this_entry == ( *thisSect ) ->entries.end() )
   {
      return DefaultValue;
   }

   return atoi( ( *this_entry ) ->getValue().c_str() );
}



bool INIFile::writePrivateProfileString( const char *Section,
      const char *Entry,
      const char *Buffer )
{
   /*
   lpAppName

   Points to a null-terminated string containing the name of the section to which
   the string will be copied. If the section does not exist, it is created. The name
   of the section is case-independent; the string can be any combination of uppercase
    and lowercase letters.

   lpKeyName

   Points to the null-terminated string containing the name of the key to be associated
    with a string. If the key does not exist in the specified section, it is created.
    If this parameter is 0, the entire section, including all entries within the section, is deleted.

   lpString

   Points to a null-terminated string to be written to the file. If this parameter is 0,
   the key pointed to by the lpKeyName parameter is deleted.
   Windows 95: This platform does not support the use of the TAB (\t) character as part of this parameter.

   lpFileName

   Points to a null-terminated string that names the initialization file.


   */
   if ( ( Section == 0 ) && ( Entry == 0 ) && ( Buffer == 0 ) )
   {
      /*    Windows 95 keeps a cached version of WIN.INI to improve performance.
          If all three parameters are 0, the function flushes the cache. The
            function always returns FALSE after flushing the cache, regardless of
            whether the flush succeeds or fails.
      */
      writeINIFile();
      return false;
   }

   loadINIFile();

   std::vector<IniSectionPtr>::iterator thisSect = std::find_if( sections.begin(), sections.end(), INISectionCmp( Section ) );
   if ( Entry == 0 )
   {
      if ( thisSect != sections.end() )
      {
         fileDirty = true;
         for ( std::vector <IniEntryPtr>::iterator this_ent = ( *thisSect ) ->entries.begin(); this_ent != ( *thisSect ) ->entries.end(); this_ent++ )
         {
            delete ( *this_ent );
         }
         ( *thisSect ) ->entries.clear();

         delete ( *thisSect );
         sections.erase( thisSect );
      }
   }
   else
   {
      if ( thisSect == sections.end() )
      {
         /* create new section */
         new INISection( this, Section, true );
         thisSect = std::find_if( sections.begin(), sections.end(), INISectionCmp( Section ) );
         fileDirty = true;
      }
      std::vector<IniEntryPtr>::iterator this_entry = std::find_if( ( *thisSect ) ->entries.begin(), ( *thisSect ) ->entries.end(), INIEntryCmp( Entry ) );
      if ( Buffer == 0 )
      {
         if ( this_entry != ( *thisSect ) ->entries.end() )
         {
            delete ( *this_entry );
            ( *thisSect ) ->entries.erase( this_entry );
            fileDirty = true;
         }
      }
      else
      {
         if ( this_entry == ( *thisSect ) ->entries.end() )
         {
            /* create new entry */
            new INIEntry( ( *thisSect ), Entry, true );
            this_entry = std::find_if( ( *thisSect ) ->entries.begin(), ( *thisSect ) ->entries.end(), INIEntryCmp( Entry ) );
            fileDirty = true;
         }
         ( *this_entry ) ->setValue( Buffer );
      }
   }

   //      return writeINIFile();
   return true;
}


//=============================================================================
ProfileEntry::ProfileEntry( int id, const char *n, const char *d, const char *h )
      : id( id ), name( n ), sdefaultval( d ), hint( h ), pt( petString )
{}
ProfileEntry::ProfileEntry( int id, const char *n, int d, const char *h )
      : id( id ), name( n ), idefaultval( d ), sdefaultval( makeStr( d ) ), hint( h ), pt( petInteger )
{}
ProfileEntry::ProfileEntry( int id, const char *n, bool d, const char *h )
      : id( id ), name( n ), bdefaultval( d ), sdefaultval( makeStr( d ) ), hint( h ), pt( petBool )
{}
void ProfileEntry::createEntry( SettingsBundle *s )
{
   switch ( pt )
   {
      case petString:
         s->setStringProfile( name.c_str(), sdefaultval.c_str() ) ;
         break;
      case petInteger:
         s->setIntProfile( name.c_str(), idefaultval ) ;
         break;
      case petBool:
         s->setBoolProfile( name.c_str(), bdefaultval ) ;
         break;
   }
}
//=============================================================================
BundleFile::BundleFile( PROFILES p )  //: iniFile( 0 )
{
   switch ( p )
   {
      case epLOGGERPROFILE:

         entries.push_back( ProfileEntry( elpPreloadFile, "PreloadFile", ".\\Configuration\\Preload.ini", "hint" ) );
         entries.push_back( ProfileEntry( elpPreloadSection, "PreloadSection", "Default", "hint" ) );
         entries.push_back( ProfileEntry( elpDisplayFile, "DisplayFile", ".\\Configuration\\Display.ini", "hint" ) );
         entries.push_back( ProfileEntry( elpDisplaySection, "DisplaySection", "Default", "hint" ) );
         entries.push_back( ProfileEntry( elpOperatorFile, "OperatorFile", ".\\Configuration\\Operator.ini", "hint" ) );
         entries.push_back( ProfileEntry( elpOperatorSection, "OperatorSection", "Default", "hint" ) );
         entries.push_back( ProfileEntry( elpListDirectory, "List Directory", ".\\Lists", "hint" ) );
         entries.push_back( ProfileEntry( elpLogDirectory, "Log Directory", ".\\Logs", "hint" ) );

         entries.push_back( ProfileEntry( elpEntryFile, "EntryFile", ".\\Configuration\\Entry.ini", "hint" ) );
         entries.push_back( ProfileEntry( elpContestFile, "ContestsFile", ".\\Configuration\\Contests.ini", "hint" ) );
         entries.push_back( ProfileEntry( elpStationFile, "StationFile", ".\\Configuration\\Station.ini", "hint" ) );
         entries.push_back( ProfileEntry( elpQTHFile, "QTHFile", ".\\Configuration\\QTH.ini", "hint" ) );
         break;
      case epPRELOADPROFILE:
         entries.push_back( ProfileEntry( eppCurrent, "Current", 0, "hint" ) );
         entries.push_back( ProfileEntry( epp1, "1", "", "hint" ) );
         entries.push_back( ProfileEntry( epp2, "2", "", "hint" ) );
         entries.push_back( ProfileEntry( epp3, "3", "", "hint" ) );
         entries.push_back( ProfileEntry( epp4, "4", "", "hint" ) );
         entries.push_back( ProfileEntry( epp5, "5", "", "hint" ) );
         entries.push_back( ProfileEntry( epp6, "6", "", "hint" ) );
         entries.push_back( ProfileEntry( epp7, "7", "", "hint" ) );
         entries.push_back( ProfileEntry( epp8, "8", "", "hint" ) );
         entries.push_back( ProfileEntry( epp9, "9", "", "hint" ) );
         entries.push_back( ProfileEntry( epp10, "10", "", "hint" ) );
         break;
      case epDISPLAYPROFILE:
         //         entries.push_back( ProfileEntry( edpFontName, "FontName", "Verdana", "Font name" ) );
         //         entries.push_back( ProfileEntry( edpFontSize, "FontSize", 8, "Font Size (points)" ) );
         entries.push_back( ProfileEntry( edpTop, "Top", 10, "Top of Logger window" ) );
         entries.push_back( ProfileEntry( edpLeft, "Left", 25, "Left of Logger window" ) );
         entries.push_back( ProfileEntry( edpWidth, "Width", 750, "Width of Logger window" ) );
         entries.push_back( ProfileEntry( edpHeight, "Height", 590, "Height of Logger window" ) );
         entries.push_back( ProfileEntry( edpShowContinentEU, "ShowContinentEU", true, "hint" ) );
         entries.push_back( ProfileEntry( edpShowContinentAS, "ShowContinentAS", false, "hint" ) );
         entries.push_back( ProfileEntry( edpShowContinentAF, "ShowContinentAF", false, "hint" ) );
         entries.push_back( ProfileEntry( edpShowContinentOC, "ShowContinentOC", false, "hint" ) );
         entries.push_back( ProfileEntry( edpShowContinentSA, "ShowContinentSA", false, "hint" ) );
         entries.push_back( ProfileEntry( edpShowContinentNA, "ShowContinentNA", false, "hint" ) );
         entries.push_back( ProfileEntry( edpShowWorked, "ShowWorked", true, "hint" ) );
         entries.push_back( ProfileEntry( edpShowUnworked, "ShowUnworked", true, "hint" ) );
         entries.push_back( ProfileEntry( edpNextContactDetailsOnLeft, "ShowNextContactDetailsOnLeft", false, "hint" ) );
         entries.push_back( ProfileEntry( edpEditor, "Editor", "Notepad.exe", "Default editor" ) );
         entries.push_back( ProfileEntry( edpAutoBandMapTuneAmount, "AutoBandMapTuneAmount", 2000, "Send to band map on tune more than x Hz " ) );
         entries.push_back( ProfileEntry( edpAutoBandMapTune, "AutoBandMapTune", false, "Enable send to band map on tune" ) );
         entries.push_back( ProfileEntry( edpAutoBandMapTimeLapse, "AutoBandMapTimeLapse", 20, "Send to band map on leaving more than x seconds " ) );
         entries.push_back( ProfileEntry( edpAutoBandMapTime, "AutoBandMapTime", false, "Enable send to band map on time lapse" ) );
         entries.push_back( ProfileEntry( edpStatisticsPeriod1, "Statistics Period 1", 10, "Statistics Period 1" ) );
         entries.push_back( ProfileEntry( edpStatisticsPeriod2, "Statistics Period 2", 60, "Statistics Period 2" ) );
         break;
      case epENTRYPROFILE:
         entries.push_back( ProfileEntry( eepCall, "Call", "", "Call sign used" ) );
         entries.push_back( ProfileEntry( eepEntrant, "Entrant", "", "Mame of club/group" ) );
         entries.push_back( ProfileEntry( eepMyName, "MyName", "", "Name of responsible operator" ) );
         entries.push_back( ProfileEntry( eepMyCall, "MyCall", "", "Callsign of responsible operator" ) );

         entries.push_back( ProfileEntry( eepMyAddress1, "MyAddress1", "", "Address line 1 of responsible operator" ) );
         entries.push_back( ProfileEntry( eepMyAddress2, "MyAddress2", "", "Address line 2 of responsible operator" ) );
         entries.push_back( ProfileEntry( eepMyCity, "MyCity", "", "City of responsible operator" ) );
         entries.push_back( ProfileEntry( eepMyCountry, "MyCountry", "", "Country of responsible operator" ) );
         entries.push_back( ProfileEntry( eepMyPostCode, "MyPostCode", "", "Post Code of responsible operator" ) );
         entries.push_back( ProfileEntry( eepMyPhone, "MyPhone", "", "Phone no. of responsible operator" ) );
         entries.push_back( ProfileEntry( eepMyEmail, "MyEmail", "", "eMail address of responsible operator" ) );
         break;
      case epCONTESTPROFILE:
         entries.push_back( ProfileEntry( ecpName, "ContestName", "", "Contest name" ) );
         entries.push_back( ProfileEntry( ecpBands, "Bands", "", "List of Contest Bands" ) );
         entries.push_back( ProfileEntry( ecpSections, "Sections", "", "List of Contest sections" ) );
         entries.push_back( ProfileEntry( ecpScoreKmQSO, "ScoreKms", true, "0 = pt/QSO, 1 = Score pt/Km" ) );
         entries.push_back( ProfileEntry( ecpStart, "Start", "", "Date/time of contest start" ) );
         entries.push_back( ProfileEntry( ecpEnd, "End", "", "Date/time of contest start" ) );
         entries.push_back( ProfileEntry( ecpDistrictMult, "DistrictMult", false, "1 = District Mult" ) );
         entries.push_back( ProfileEntry( ecpDXCCMult, "DXCCMult", false, "1 = DXCC Mult" ) );
         entries.push_back( ProfileEntry( ecpLocMult, "LocMult", false, "1 = LOC Mult" ) );
         entries.push_back( ProfileEntry( ecpQTHReq, "QTHReq", false, "1 = QTH Required" ) );
         entries.push_back( ProfileEntry( ecpAllowLoc4, "allowLoc4", false, "1 = Short (4 char) locators allowed" ) );
         entries.push_back( ProfileEntry( ecpAllowLoc8, "allowLoc8", false, "1 = Long (8 char) locators allowed" ) );

         break;
      case epQTHPROFILE:
         entries.push_back( ProfileEntry( eqpLocator, "Locator", "", "Locator" ) );
         entries.push_back( ProfileEntry( eqpDistrict, "District", "", "District Exchange" ) );
         entries.push_back( ProfileEntry( eqpLocation, "Location", "", "Descriptive Location Exchange" ) );
         entries.push_back( ProfileEntry( eqpStationQTH1, "StationQTH1", "", "Address line 1/2 of station" ) );
         entries.push_back( ProfileEntry( eqpStationQTH2, "StationQTH2", "", "Address line 2/2 of station" ) );
         entries.push_back( ProfileEntry( eqpASL, "ASL", 0, "QTH height ASL (metres)" ) );
         break;
      case epSTATIONPROFILE:
         entries.push_back( ProfileEntry( espPower, "Power", 0, "Transmit Power (Watts)" ) );
         entries.push_back( ProfileEntry( espTransmitter, "Transmitter", "", "Transmit Equipment" ) );
         entries.push_back( ProfileEntry( espReceiver, "Receiver", "", "Receive Equipment" ) );
         entries.push_back( ProfileEntry( espAntenna, "Antenna", "", "Antenna details" ) );
         entries.push_back( ProfileEntry( espAGL, "AGL", 0, "Antenna Height AGL (metres)" ) );
         break;
   }
}
bool BundleFile::openProfile( const std::string &fname, const std::string &bname )
{
   bundleName = bname;
   iniFile = boost::shared_ptr<INIFile>( new INIFile( fname.c_str() ) );

   iniFile->loadINIFile();
   return true;
}
BundleFile::~BundleFile()
{
   //delete iniFile;
}
/*static*/
boost::shared_ptr<BundleFile>BundleFile::bundleFiles[ epSTATIONPROFILE + 1 ]; //  =  {0};
/*static*/
boost::shared_ptr<BundleFile>BundleFile::getBundleFile( PROFILES p )
{
   if ( !bool( bundleFiles[ p ] ) )
   {
      bundleFiles[ p ] = boost::shared_ptr<BundleFile>( new BundleFile( p ) );
   }
   return bundleFiles[ p ];
}
ProfileEntry &BundleFile::GetKey( int p )
{
   static ProfileEntry NullEntry( -1, "", 0, "" );
   for ( unsigned int i = 0; i < entries.size(); i++ )
   {
      if ( entries[ i ].id == p )
         return entries[ i ];
   }
   return NullEntry;
}
//=============================================================================
SettingsBundle::SettingsBundle() //:bundleFile(0)
{}

SettingsBundle::~SettingsBundle()
{
   try
   {
      closeProfile();
   }
   catch ( ... )
   {}
}
std::string SettingsBundle::getBundle()
{
   return bundleFile->getBundle();
}

void SettingsBundle::setProfile( boost::shared_ptr<BundleFile> b )
{
   bundleFile = b;
}
void SettingsBundle::openSection( const std::string &psect )
{
   if ( psect.size() )
      currsection = psect;
   else
      currsection = noneBundle;
}

void SettingsBundle::closeProfile()
{
   flushProfile();
}
std::string SettingsBundle::getSection()
{
   return currsection;
}
bool SettingsBundle::newSection( const std::string &newname )
{
   // Create a new section with no match
   std::vector<IniSectionPtr>::iterator newSect = std::find_if( bundleFile->iniFile->sections.begin(), bundleFile->iniFile->sections.end(), INISectionCmp( newname ) );
   if ( newSect == bundleFile->iniFile->sections.end() )
   {
      currsection = newname;
      for ( std::vector<ProfileEntry>::iterator i = bundleFile->entries.begin(); i != bundleFile->entries.end(); i++ )
      {
         ( *i ).createEntry( this );
      }
      return true;
   }
   return false;
}


bool SettingsBundle::dupSection( const std::string &newname )
{
   if ( currsection == noneBundle )
   {
      return false;
   }
   // Create a new section that matches the old
   return bundleFile->iniFile->dupSection( currsection, newname );
}

bool SettingsBundle::checkKeyExists( const char *key )
{
   if ( !bundleFile || currsection == noneBundle )
   {
      return false;
   }
   return bundleFile->iniFile->checkKeyExists( currsection.c_str(), key );
}
std::string SettingsBundle::nameOf( int enumKey )
{
   if ( bool( bundleFile ) && currsection != noneBundle )
   {
      for ( std::vector<ProfileEntry>::iterator i = bundleFile->entries.begin(); i != bundleFile->entries.end(); i++ )
      {
         if ( ( *i ).id == enumKey )
            return ( *i ).name;
      }
   }
   return "";
}

void SettingsBundle::getBoolProfile( const char *key, bool &value, bool def )
{
   if ( !bundleFile || currsection == noneBundle )
   {
      value = def;
      return ;
   }
   int intval = bundleFile->iniFile->getPrivateProfileInt( currsection.c_str(), key, ( int ) def );
   if ( intval )
      value = true;
   else
      value = false;
}
void SettingsBundle::getBoolProfile( int enumkey, bool &value )
{
   ProfileEntry & p = bundleFile->GetKey( enumkey );
   getBoolProfile( p.name.c_str(), value, p.bdefaultval );
}
void SettingsBundle::getBoolProfile( int enumkey, MinosItem<bool> &value )
{
   bool temp;
   getBoolProfile( enumkey, temp );
   value.setValue( temp );
}
void SettingsBundle::setBoolProfile( const char *key, bool value )
{
   if ( !bundleFile || currsection == noneBundle )
   {
      return ;
   }
   bundleFile->iniFile->writePrivateProfileString( currsection.c_str(), key, value ? "1" : "0" );
}
void SettingsBundle::setBoolProfile( int enumkey, bool value )
{
   ProfileEntry & p = bundleFile->GetKey( enumkey );
   setBoolProfile( p.name.c_str(), value );
}
void SettingsBundle::getStringProfile( const char *key, std::string &value, const char *def )
{
   if ( !bundleFile || currsection == noneBundle )
   {
      value = def;
      return ;
   }
   TEMPBUFF( inbuff, 256 );
   bundleFile->iniFile->getPrivateProfileString( currsection.c_str(), key, def, inbuff, 255 );
   value = inbuff;
}
void SettingsBundle::getStringProfile( int enumkey, std::string &value )
{
   ProfileEntry & p = bundleFile->GetKey( enumkey );
   getStringProfile( p.name.c_str(), value, p.sdefaultval.c_str() );
}

void SettingsBundle::getStringProfile( int enumkey, MinosItem<std::string> &value )
{
   std::string temp;
   getStringProfile( enumkey, temp );
   value.setValue( temp );
}
void SettingsBundle::setStringProfile( const char *key, const char *value )
{
   if ( !bundleFile || currsection == noneBundle )
   {
      return ;
   }
   bundleFile->iniFile->writePrivateProfileString( currsection.c_str(), key, value );
}
void SettingsBundle::setStringProfile( int enumkey, const char *value )
{
   ProfileEntry & p = bundleFile->GetKey( enumkey );
   setStringProfile( p.name.c_str(), value );
}

void SettingsBundle::getIntProfile( const char *key, int &value, int def )
{
   if ( !bundleFile || currsection == noneBundle )
   {
      value = def;
      return ;
   }
   value = bundleFile->iniFile->getPrivateProfileInt( currsection.c_str(), key, def );
}
void SettingsBundle::getIntProfile( int enumkey, int &value )
{
   ProfileEntry & p = bundleFile->GetKey( enumkey );
   getIntProfile( p.name.c_str(), value, p.idefaultval );
}
void SettingsBundle::getIntProfile( int enumkey, MinosItem<int> &value )
{
   int temp;
   getIntProfile( enumkey, temp );
   value.setValue( temp );
}
void SettingsBundle::setIntProfile( const char *key, int value )
{
   if ( !bundleFile || currsection == noneBundle )
   {
      return ;
   }
   std::string sval = ( boost::format( "%d" ) % value ).str();
   bundleFile->iniFile->writePrivateProfileString( currsection.c_str(), key, sval.c_str() );
}
void SettingsBundle::setIntProfile( int enumkey, int value )
{
   ProfileEntry & p = bundleFile->GetKey( enumkey );
   setIntProfile( p.name.c_str(), value );
}

std::vector<std::string> SettingsBundle::getProfileEntries( )
{
   char retbuff[ 1024 ];

   std::vector<std::string> sectlist;
   if ( !bundleFile || currsection == noneBundle )
   {
      return sectlist;
   }

   int mlen = bundleFile->iniFile->getPrivateProfileString( currsection.c_str(), 0, "", retbuff, 1024 );

   char *npt = retbuff;

   while ( *npt && ( npt - retbuff < mlen ) )
   {
      std::string str = npt;
      sectlist.push_back( str );
      npt = npt + strlen( npt ) + 1;
   }
   return sectlist;
}
std::vector<int> SettingsBundle::getBundleEntries( )
{
   std::vector<int> e;
   if ( bundleFile )
   {
      for ( std::vector<ProfileEntry>::iterator i = bundleFile->entries.begin(); i != bundleFile->entries.end(); i++ )
      {
         e.push_back( ( *i ).id );
      }
   }
   return e;

}
std::vector<std::string> SettingsBundle::getBundleHints( )
{
   std::vector<std::string> e;
   if ( bundleFile )
   {
      for ( std::vector<ProfileEntry>::iterator i = bundleFile->entries.begin(); i != bundleFile->entries.end(); i++ )
      {
         e.push_back( ( *i ).hint );
      }
   }
   return e;
}
std::vector<std::string> SettingsBundle::getSections( )
{
   std::vector<std::string> slist;
   if ( !bundleFile )
   {
      return slist;
   }
   slist.push_back( noneBundle );
   for ( std::vector <IniSectionPtr>::iterator thisSect = bundleFile->iniFile->sections.begin(); thisSect != bundleFile->iniFile->sections.end(); thisSect++ )
   {

      if ( ( *thisSect ) ->isValidSection() )
      {
         const std::string sname = ( *thisSect ) ->name;
         slist.push_back( sname );
      }
   }
   return slist;
}
void SettingsBundle::clearProfileSection( )
{
   // clear the content AND the section header
   if ( bundleFile )
      bundleFile->iniFile->writePrivateProfileString( currsection.c_str(), 0, 0 );

}
void SettingsBundle::flushProfile( void )
{
   if ( !bundleFile || !bundleFile->iniFile )
   {
      return ;
   }
   bundleFile->iniFile->writePrivateProfileString( 0, 0, 0 );
}

