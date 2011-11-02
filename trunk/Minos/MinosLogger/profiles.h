/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef ProfilesH
#define ProfilesH 
//----------------------------------------------------------------------------
extern const std::string noneBundle;
class SettingsBundle;
class ProfileEntry
{
      enum peType{petString, petInteger, petBool} pt;
   public:
      int id;
      std::string name;
      std::string dispname;

      std::string sdefaultval;
      int idefaultval;
      bool bdefaultval;

      std::string hint;
      bool RO;

      void createEntry( SettingsBundle * );
      ProfileEntry( int id, const char *n, const char *d, const char * dname, const char *h, bool RO );
      ProfileEntry( int id, const char *n, int d, const char * dname, const char *h, bool RO );
      ProfileEntry( int id, const char *n, bool d, const char * dname, const char *h, bool RO );
};
class INIFile;

class BundleFile
{
   private:
      std::string bundleName;
   public:
      boost::shared_ptr<INIFile> iniFile;
      std::vector<ProfileEntry> entries;

      static boost::shared_ptr<BundleFile>bundleFiles[ epSTATIONPROFILE + 1 ];
      BundleFile( PROFILES p );
      ~BundleFile();
      static boost::shared_ptr<BundleFile> getBundleFile( PROFILES p );
      bool openProfile( const std::string &pname, const std::string &bname );
      std::string getBundle()
      {
         return bundleName;
      }
      ProfileEntry &GetKey( int p );
};
class SettingsBundle
{
   private:
      std::string currsection;
   public:
      boost::shared_ptr<BundleFile> bundleFile;

      SettingsBundle( );
      ~SettingsBundle();
      std::string getBundle();

      void setProfile( boost::shared_ptr<BundleFile> b );

      void openSection( const std::string &psect );
      std::string getSection();
      bool dupSection( const std::string &sname );
      bool newSection( const std::string &sname );

      void closeProfile( void );

      bool checkKeyExists( const char *key );
      std::string displayNameOf( int enumKey );
      bool isReadOnly( int enumKey );
      void getBoolProfile( const char *key, bool &value, bool def );
      void getBoolProfile( const char *key, MinosItem<bool> &value, bool def );
      void setBoolProfile( const char *key, bool value );
      void getBoolProfile( int enumkey, bool &value );
      void getBoolProfile( int enumkey, MinosItem<bool> &value );
      void setBoolProfile( int enumkey, bool value );

      void getStringProfile( const char *key, std::string &value, const char *def );
      void getStringProfile( const char *key, MinosItem<std::string> &value, const char *def );
      void setStringProfile( const char *key, const char *value );
      void getStringProfile( int enumkey, std::string &value );
      void getStringProfile( int enumkey, MinosItem<std::string> &value );
      void setStringProfile( int enumkey, const char *value );

      void getIntProfile( const char *key, int &value, int def );
      void getIntProfile( const char *key, MinosItem<int> &value, int def );
      void setIntProfile( const char *key, int value );
      void getIntProfile( int enumkey, int &value );
      void getIntProfile( int enumkey, MinosItem<int> &value );
      void setIntProfile( int enumkey, int value );

      void flushProfile( void );
      void clearProfileSection( );

      std::vector<std::string> getProfileEntries( );
      std::vector<int> getBundleEntries( );
      std::vector<std::string> getBundleHints( );
      std::vector<std::string> getSections( );
};
/*
extern ProfileEntry LoggerEntries[];
extern ProfileEntry PreloadEntries[];
extern ProfileEntry DisplayEntries[];
extern ProfileEntry OperatorEntries[];
extern ProfileEntry EntryEntries[];
extern ProfileEntry ContestEntries[];
extern ProfileEntry QTHEntries[];
extern ProfileEntry StationEntries[];
*/
#endif
