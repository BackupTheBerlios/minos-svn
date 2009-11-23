/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MatchThreadH
#define MatchThreadH 
//---------------------------------------------------------------------------
#include <Classes.hpp> 
//---------------------------------------------------------------------------
#define SET_CHANGED 1
#define SET_NOT_GREATER 2
class matchElement
{
   public:
      bool match;
      bool empty;
      //      bool trimmed;
      std::string mstr;
      std::string rawstr;

      matchElement( void );
      unsigned char set
         ( const std::string & );
      unsigned char checkGreater( const std::string & );
      bool checkMatch( const std::string & );
};
struct LtMatch
{
   bool operator() ( const MatchContact* s1, const MatchContact* s2 ) const
   {
      return * s1 < *s2;
   }
};
typedef codeproject::sorted_vector < MatchContact *, true, LtMatch > MatchList;
typedef MatchList::iterator MatchIterator;

class TMatchCollection
{
   public:
      MatchList matchList;
      void freeAll()
      {
         for ( MatchIterator i = matchList.begin(); i != matchList.end(); i++ )
         {
            delete ( *i );
            ( *i ) = 0;
         }
         matchList.clear();
      }
      TMatchCollection( void );
      ~TMatchCollection();
      int getContactCount( void );
      MatchContact *pcontactAt( int );
};
class Matcher
{
      bool matchRequired;
   protected:
      enum MatchPhase {Exact, NoSuffix, NoLoc, Body, Country};
      enum ContestPhase {Starting, MainContest, Rest};

      MatchPhase mp;
      bool matchStarted;
      ContestPhase firstMatch;
      int tickct;

      int contestIndex;
      int contactIndex;

      matchElement matchcs;
      matchElement matchloc;
      matchElement matchqth;

      CountryEntry *ce;

      int thisContestMatched;
      virtual void matchDistrict( const std::string &extraText ) = 0;
      virtual void matchCountry( const std::string &cs ) = 0;
      virtual void replaceList( TMatchCollection *matchCollection ) = 0;
   public:

      Matcher( );
      virtual ~Matcher();
      TMatchCollection *matchCollection;

      void startMatch( CountryEntry *ce = 0 );
      void initMatch( void );
      void clearmatchall();
      virtual bool idleMatch( int limit ) = 0;

      void setMatchRequired( bool b )
      {
         matchRequired = b;
      }
      bool getMatchRequired()
      {
         return matchRequired;
      }

};
class LogMatcher: public Matcher
{
      virtual void matchDistrict( const std::string &extraText );
      virtual void matchCountry( const std::string &cs );
      virtual void replaceList( TMatchCollection *matchCollection );
   public:
      LogMatcher( );
      virtual ~LogMatcher();

      virtual bool idleMatch( int limit );
      void addMatch( BaseContact *, BaseContestLog * );
};
class ListMatcher: public Matcher
{
      virtual void matchDistrict( const std::string &extraText );
      virtual void matchCountry( const std::string &cs );
      virtual void replaceList( TMatchCollection *matchCollection );
   public:
      ListMatcher( );
      ~ListMatcher();

      virtual bool idleMatch( int limit );
      void addMatch( ListContact *, ContactList * );
};
//---------------------------------------------------------------------------
class TMatchThread : public TThread
{
      //   friend class Matcher;
      //   friend class LogMatcher;
      //   friend class ListMatcher;
   private:
      static TMatchThread *matchThread;
      __fastcall TMatchThread();

      LogMatcher *logMatch;
      ListMatcher *listMatch;

      TMatchCollection *myMatches;        // used to pass the match list out
      TMatchCollection *myListMatches;    // used to pass the match list out

      std::string ctrymatch;
      std::string distmatch;

      std::string matchStatus;

      void __fastcall doReplaceContestList( void );
      void __fastcall doReplaceListList( void );
      void __fastcall doMatchCountry( void );
      void __fastcall doMatchDistrict( void );

   protected:
      virtual void __fastcall Execute();    // TThread method
   public:
      void __fastcall ShowMatchStatus( std::string mess );
      void __fastcall replaceContestList( TMatchCollection *matchCollection );
      void __fastcall replaceListList( TMatchCollection *matchCollection );
      void __fastcall matchCountry( std::string cs );
      void __fastcall matchDistrict( std::string dist );

      static void startMatch( CountryEntry *ce = 0 );
      static std::string getMatchStatus( );
      static void FinishMatchThread();
      static TMatchThread *getMatchThread()
      {
         return matchThread;
      }
};
//---------------------------------------------------------------------------
#endif
