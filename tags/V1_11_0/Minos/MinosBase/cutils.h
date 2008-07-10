/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef CutilsH
#define CutilsH 
//----------------------------------------------------------------------------
extern const double pi /* = (double )3.141592653 */;  /* pi */
extern const double dr /* = pi/180.0*/;      			  // degree to radian conversion factor

extern bool checkFileOK( std::ifstream &istr, const std::string &fname, const std::string &fmess );

//extern char *lbuff;
extern void clearBuffer( void );
extern void strtobuf( const std::string &str );
extern void strtobuf( const MinosItem<std::string> &str );
extern void strtobuf();
std::string strupr( const std::string &s );
extern int stricmp( const std::string &s1, const std::string &s2 );
extern int strnicmp( const std::string &s1, const std::string &s2, unsigned int len );
//extern char *trimr(char *);
extern void opyn( bool b );
void opyn( const MinosItem<bool> &b );
extern void buftostr( std::string &str );
extern void buftostr( MinosItem<std::string> &str );
extern bool inyn();
extern bool wildComp( const std::string &e, const std::string &s );
char *mystrncpy( char *s1, const char *s2, int maxlen );
//extern void *mymemcpy(void *dest, const void *src, size_t n);
extern int strcpysp( char *s1, const std::string &s2, int maxlen = 255 );
extern int strcpysp( std::string &s1, const std::string &s2, int maxlen = 255 );
extern int strcmpsp( const std::string &s1, const std::string &s2 );
extern int stricmpsp( const std::string &s1, const std::string &s2 );
extern int placestr( char *buff, const std::string &str, int start, int len );
int parseLine( char *buff, char sep, char **a, int count, char sep2, bool &sep2seen );

extern std::string trimr( const std::string & );
extern std::string trim( const std::string & );

extern std::string makeADIFField( const std::string &fieldname, const std::string &content );
extern std::string makeADIFField( const std::string &fieldname, int content );
extern String TDTToCanonical( TDateTime d );
extern TDateTime CanonicalToTDT( String cdtg );

const DWORD bsize = 256;
extern char diskBuffer[ bsize + 1 ];

extern int buffpt;

class writer
{
      HANDLE expfd;
      //      char *lbuff;

   public:
      //void lwrite( void );
      void lwrite( const char * );
      void lwriteLine( void );
      void lwriteNl( void );
      void lwriteFf( void );
      writer( HANDLE f );
      ~writer();
};

#endif
