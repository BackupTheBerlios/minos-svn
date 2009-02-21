/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ListContactH
#define ListContactH 
//---------------------------------------------------------------------------
class ListContact
{
   public:
      BaseContact& operator =( const BaseContact & );

      callsign cs;   //CONTAIN MinosItem
      locator loc;   //CONTAIN MinosItem
      std::string extraText;
      std::string comments;
      virtual std::string getField( int ACol, const BaseContestLog * const curcon ) const;
};
#endif
