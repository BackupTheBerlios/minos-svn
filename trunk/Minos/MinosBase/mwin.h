/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef MwinH
#define MwinH 
//----------------------------------------------------------------------------

// GJVVERSION is definition for the .GJV file format

#define GJVVERSION "0.41"
#define VERLENGTH (4)

#define DATELENGTH 8
#define TIMELENGTH 5
#define CALLSIGNLENGTH 15
#define LOCLENGTH 8
#define COMMENTLENGTH 27
#define EXTRALENGTH 50
//#define RSTLENGTH 3
#define SERIALLENGTH 4

//#define INISECTLENGTH 40

#define CS_OK 0
#define CS_NOT_VALIDATED -1
#define ERR_NOCS 21
#define ERR_CSDIGIT 22
#define ERR_INVCS 23
#define ERR_DUPCS 24

#define LOC_NOT_VALIDATED -1
#define LOC_OK 0
#define LOC_SHORT 1
#define LOC_PARTIAL 2
#define ERR_NOLOC 30
#define ERR_LOC_RANGE 31

enum ExportType {EREG1TEST, EADIF, EG0GJV, EMINOS, EKML, EPRINTFILE };

#define TEMPBUFF(a, b) char a[(b)]

// control block for repeated actions on contest contacts
class GJVParams
{
   public:
      int diskBlock;
      HANDLE fd;
      int count;

      GJVParams( HANDLE f );
      ~GJVParams();
};
//===================================================================================

// templated class for items in contest and contact
class RPCParamStruct;

template < class itemtype >
class MinosItem
{
   private:
      bool dirty;
      itemtype val;
      itemtype operator = ( const itemtype& ); // make it inaccessible

      std::string miTrim(const std::string & s)
      {
         return trim(s);
      }
      int miTrim(const int & s)
      {
         return s;
      }
      bool miTrim(const bool & s)
      {
         return s;
      }

   public:

      itemtype getValue() const
      {
         return val;
      }
      itemtype getValue( bool &d ) const
      {
         d |= dirty;
         return val;
      }
      void setValue( itemtype t )
      {
         if ( val != t )        // so all item classes need != operator
         {
            dirty = true;
            val = miTrim(t);
         }
      }
      void setValue( MinosItem<itemtype> t )
      {
         if ( val != t.getValue() )        // so all item classes need != operator
         {
            dirty = true;
            val = t.getValue();
         }
      }
      void setInitialValue( itemtype t )
      {
         dirty = false;
         val = miTrim(t);
      }
      bool isDirty() const
      {
         return dirty;
      }
      void clearDirty()
      {
         dirty = false;
      }
      void setDirty()
      {
         dirty = true;
      }
      MinosItem()
      {}
      MinosItem( itemtype d ) : val( d ), dirty( false )
      {}
      virtual ~MinosItem()
      {}

      void addIfDirty( RPCParamStruct *st, const std::string &stName, bool &d ) const
      {
         d |= isDirty();
         if ( isDirty() )
         {
            st->addMember( getValue(), stName );
         }
      }
};
//---------------------------------------------------------------------------
enum DTG {DTGLOG, DTGDISP, DTGReg1Test, DTGFULL, DTGPRINT};
class dtg
{
   private:
      MinosItem<std::string> sdate;
      MinosItem<std::string> stime;
   public:
      static const double daySecs;

      static TDateTime getRawUTC( );
      static TDateTime getCorrectedUTC( );

      std::string getDate( DTG, bool &d ) const;
      std::string getDate( DTG ) const;
      std::string getTime( DTG, bool &d ) const;
      std::string getTime( DTG ) const;
      bool getDtg( time_t &, bool &d ) const;
      bool getDtg( time_t & ) const;
      void setDate( const std::string &, DTG );
      void setTime( const std::string &, DTG );
      void setDtg( time_t );

      std::string getIsoDTG( bool &d ) const;
      std::string getIsoDTG( ) const;
      void setIsoDTG( const std::string & );

      void setValue( const dtg &d )
      {
         sdate.setValue( d.sdate.getValue() );
         stime.setValue( d.stime.getValue() );
      }
      int notEntered( void );  // returns the "entered" state

      bool isDirty() const
      {
         return ( sdate.isDirty() | stime.isDirty() );
      }
      void setDirty()
      {
         sdate.setDirty();
         stime.setDirty();
      }
      void clearDirty()
      {
         sdate.clearDirty();
         stime.clearDirty();
      }

      dtg( bool now );
      //    dtg(const dtg&);
      //    dtg& operator =(const dtg&);
      ~dtg();
};
//---------------------------------------------------------------------------
class callsign
{
   #define BITLENGTH 4
	#define NUMBITLENGTH 4
	#define TRAILBITLENGTH 3
   public:
      MinosItem<std::string> fullCall; // full call
      // eg for <pe/f0ctt/mm> (g0gjv/p) [F6CTT/RVI/P]
      std::string prefix; // <pe> (g) [/RVI]country of location
      std::string prefix2;  // <f> (g) [F]country of issue
      std::string number; // <0> (0) [6]numeric part
      std::string body;  // <ctt> (gjv) [CTT]main body
      std::string suffix; // <mm> (p) [P]trailer

      char valRes;   // current validation result

      callsign( );
      callsign( const std::string &orig );
      ~callsign();

      char validate( );
      bool isUK() const;
};
//---------------------------------------------------------------------------
class locator
{
   public:
      MinosItem<std::string> loc;	// allowing for Microwave locs

      mutable char valRes;

      locator();
      //    locator(const locator&);
      //    locator& operator =(const locator&);
      ~locator();
      char validate( void );
      char validate( double &lon, double &lat );
};


#endif
