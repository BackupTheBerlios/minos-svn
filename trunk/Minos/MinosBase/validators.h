/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef validatorsH
#define validatorsH 
//---------------------------------------------------------------------------
#define ERR_0 0
#define ERR_1 1
#define ERR_2 2
#define ERR_3 3
#define ERR_4 4
#define ERR_5 5
#define ERR_6 6
#define ERR_7 7
#define ERR_8 8
#define ERR_9 9
#define ERR_10 10
#define ERR_11 11
#define ERR_12 12
#define ERR_13 13
#define ERR_14 14
#define ERR_15 15
#define ERR_16 16
#define ERR_17 17
#define ERR_18 18
#define ERR_19 19
#define ERR_20 20
#define ERR_21 21
#define ERR_22 22
#define ERR_23 23
#define ERR_24 24
#define ERR_25 25
#define ERR_26 26
#define ERR_27 27
#define ERR_28 28
#define ERR_29 29

#define MINSHOWPRIORITY 10
class ErrEntry
{
   public:
      ErrEntry()
            : priority( 0 ), errStr( "" )
      {}
      ErrEntry( unsigned int c, const char *s )
            : priority( c ), errStr( s )
      {}

      ErrEntry( const ErrEntry &e )
      {
         priority = e.priority;
         errStr = e.errStr;
      }
      unsigned int priority;
      /*const*/
      std::string errStr;

      bool operator==( const ErrEntry& e2 ) const
      {
         if ( ( priority == e2.priority ) && strcmp( errStr.c_str(), e2.errStr.c_str() ) == 0 )
            return true;
         return false;
      }
      bool operator!=( const ErrEntry& e2 ) const
      {
         if ( ( priority != e2.priority ) || strcmp( errStr.c_str(), e2.errStr.c_str() ) != 0 )
            return true;
         return false;
      }
      bool operator<( const ErrEntry& e2 ) const
      {
         if ( priority < e2.priority )
            return true;

         if ( priority == e2.priority )
            return ( strcmp( errStr.c_str(), e2.errStr.c_str() ) < 0 );

         return false;
      }

};

extern ErrEntry errDefs[];
extern bool allSpaces( const char *S );

typedef codeproject::sorted_vector < ErrEntry *, true > ErrorList;
typedef ErrorList::iterator ErrorIterator;

enum validTypes { /*cmNone, cmCancel, cmValid,*/ cmCheckValid, cmValidStatus /*, cmReleasedFocus*/};
enum validatorTypes {vtNone, vtNotEmpty, vtNumeric, vtDate, vtTime, vtCallsign,
                     vtSN, vtRST, vtLoc, vtQTH, vtComments};

class Validator
{
      static bool validNumber( const std::string &S, bool trailingAlphaAllowed = false);
      static bool allSpaces( const std::string &S );
      bool status;
      validatorTypes vt;
   public:
      Validator( validatorTypes vt );
      bool validate( String );
      static bool validateRST( String t );
      bool getStatus()
      {
         return status;
      }
};

class ValidatedControl
{
   public:
      TLabeledEdit *wc;
      Validator validator;
      bool tIfValid;
      ValidatedControl( TLabeledEdit *c, validatorTypes vt );
      virtual bool valid( validTypes cmd );
};
//---------------------------------------------------------------------------
#endif
