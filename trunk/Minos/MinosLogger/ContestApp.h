/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef ContestAppH
#define ContestAppH

//---------------------------------------------------------------------------
class TMatchThread;
class ContestSlot
{
   public:
      unsigned int slotno;
      BaseContestLog * slot;
      //      unsigned char colour;

      ContestSlot( void );
      ~ContestSlot();
};
class ListSlot
{
   public:
      unsigned int slotno;
      ContactList * slot;
      //      unsigned char colour;

      ListSlot( void );
      ~ListSlot();
};
struct CmpSlotNo
{
   bool operator() ( const ContestSlot* s1, const ContestSlot* s2 ) const
   {
      return s1->slotno < s2->slotno;
   }
   bool operator() ( const ListSlot* s1, const ListSlot* s2 ) const
   {
      return s1->slotno < s2->slotno;
   }
};

typedef codeproject::sorted_vector < ContestSlot *, true, CmpSlotNo > ContestSlotList;
typedef ContestSlotList::iterator SlotIterator;

typedef codeproject::sorted_vector < ListSlot *, true, CmpSlotNo > ListSlotList;
typedef ListSlotList::iterator ListSlotIterator;

class TContestApp : public MinosParameters
{
   private:

      BaseContestLog * currentContest;
      static TContestApp *contestApp;
      int magneticVariation;
   public:
      static TContestApp *getContestApp();

      // general settings - choose at startup
      SettingsBundle loggerBundle;     // startup defaults - INI files, sections, etc
      SettingsBundle preloadBundle;    // contests to be preloaded
      SettingsBundle displayBundle;    // display settings - or is this an op preference

      TFont *sysfont;
      ContestSlotList contestSlotList;
      ListSlotList listSlotList;
      void freeAll()
      {
         for ( SlotIterator i = contestSlotList.begin(); i != contestSlotList.end(); i++ )
            delete ( *i );
         contestSlotList.clear();
         for ( ListSlotIterator i = listSlotList.begin(); i != listSlotList.end(); i++ )
            delete ( *i );
         listSlotList.clear();
      }


      int period1, period2;

      bool reverseBearing;
      bool dispCountry;
      bool dispBearing;
      bool dispScore;

      virtual int getMagneticVariation()
      {
         return magneticVariation;
      }
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
      virtual bool getAllowLoc4();
      virtual bool getAllowLoc8();
      virtual bool yesNoMessage( TComponent* Owner, String mess );
      virtual void mshowMessage( String mess, TComponent* Owner = 0 );
      virtual BaseContestLog * getCurrentContest();
      virtual bool insertList( ContactList *p, unsigned int sno );
      virtual bool isContestOpen( const std::string fn );
      virtual bool isListOpen( const std::string fn );
      virtual std::vector<BaseContestLog *> getContestList();
      virtual int getFontMultiplier();
      virtual void applyFontMultiplier(TWinControl *);

      TContestApp();
      bool initialise();
      ~TContestApp();
      void close();

      int getContestSlotCount()
      {
         return contestSlotList.size();
      }
      int getListSlotCount()
      {
         return listSlotList.size();
      }
      int getOccupiedListSlotCount();

      bool insertContest( BaseContestLog *p, unsigned int sno );
      BaseContestLog * findFirstContest();
      int findContest( BaseContestLog * p );
      int findList( ContactList * p );
      //      LoggerContestLog * findContest( LoggerContestLog *p );
      void removeContest( BaseContestLog * p );
      void removeList( ContactList * p );
      void writeContestList();
      LoggerContestLog * openFile( const std::string &fn, bool newFile, int slotno );
      ContactList * openListFile( const std::string &fn, int slotno );
      void closeFile( BaseContestLog *contest );
      void closeListFile( ContactList *list );

      void setCurrentContest( BaseContestLog * );

};
extern double bigClockCorr;

extern bool isOpen( ContestSlot *cs, const std::string &fn );
extern bool isOpen( ListSlot *cs, const std::string &fn );
extern void closeContestApp( void );
extern bool initialiseContestApp( void );
extern bool contestAppLoadFiles( void );


#endif
