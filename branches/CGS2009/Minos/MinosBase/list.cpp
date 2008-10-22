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
//---------------------------------------------------------------------------

#pragma package(smart_init)

ContactList::ContactList() : slotno( -1 ), cslFile( false )
{
}
ContactList::~ContactList()
{
   freeAll();
}
bool ContactList::initialise( int sno )
{
   if ( !MinosParameters::getMinosParameters() ->insertList( this, sno ) )
   {
      return false;
   }
   slotno = sno;
   return true;
}

bool ContactList::initialise( const std::string &fn, int slotno )
{

   if ( !initialise( slotno ) )
      return false;
   if ( MinosParameters::getMinosParameters() ->isListOpen( fn ) )
      return false;

   // open the List file

   cfileName = fn;
   String ext = ExtractFileExt( fn.c_str() );

	if ( stricmp( ext.t_str(), ".csl" ) == 0 )
   {
      cslFile = true;
   }
   else
   {
      return false;
   }

   bool loadOK = false;

   if ( cslFile )
   {
      if ( !cslLoad() )     // load the header so that we can display it
         return false;
      loadOK = cslLoadContacts();
   }

   return loadOK;
}
bool ContactList::cslLoad( void )
{
   int nextBlock = 0;

   ListContact *rct = 0;
   TEMPBUFF( buff, 256 );

   ifstream istr( cfileName.c_str() ); // should close when it goes out of scope
   if ( !istr )
   {
      std::string lerr = lastError();
      std::string emess = "Failed to open ContactList file " + cfileName + " : " + lerr;
      MinosParameters::getMinosParameters() ->mshowMessage( emess.c_str() );
      return false;
   }
   String fn = ExtractFileName( cfileName.c_str() );
   name = fn.t_str();

   while ( istr.getline( buff, 255 ) )
   {
      char * a[ 3 ];
      int i = 0;

      buff[ 254 ] = 0;

      int len = strlen( buff );

      for ( i = 0; i < len; i++ )
      {
         if ( !isspace( buff[ i ] ) )
            break;
      }
      if ( i == len )
         continue;				// blank line

      if ( buff[ 0 ] == '#' )
         continue;   // skip comment lines

      i = 0;

      for ( int j = 0; j < 3; j++ )
      {
         if ( j != 0 )
         {
            // terminate the previous entry on a ','
            while ( ( i < len ) && buff[ i ] && ( ( buff[ i ] != ',' ) && ( buff[ i ] != '\t' ) ) )
               i++;

            if ( ( buff[ i ] == ',' ) || ( buff[ i ] == '\t' ) )
               buff[ i++ ] = 0;
         }
         while ( ( i < len ) && buff[ i ] && ( buff[ i ] == ' ' ) )
            i++;

         a[ j ] = &buff[ i ];
      }

      if ( ++nextBlock == 1 && a[ 0 ][ 0 ] == 0 && a[ 1 ][ 0 ] == 0 )
      {
         name = a[ 2 ];              // first line of file gives the list name
      }
      else
      {
         rct = new ListContact();

         // a1, a2, a3 will all be set - but may point to null terminator!

         rct->cs.fullCall.setValue( strupr( a[ 0 ] ) );

         rct->loc.loc.setValue( strupr( a[ 1 ] ) );
         rct->loc.valRes = LOC_NOT_VALIDATED;

         rct->extraText = a[ 2 ];

         ctList.push_back( rct );
      }
   }

   return true;
}
bool ContactList::cslLoadContacts( void )
{
   return true;
}
void ContactList::freeAll()
{
   for ( ListIterator i = ctList.begin(); i != ctList.end(); i++ )
      delete ( *i );
   ctList.clear();
}
void ContactList::getMatchText( ListContact *, std::string &disp, const BaseContestLog *const /*ct*/ ) const
{
   disp = "N/A";
}
void ContactList::getMatchField( ListContact *pct, int col, std::string &disp, const BaseContestLog *const ct ) const
{
   std::string temp;
   if ( pct )
      temp = pct->getField( col, ct );

   disp = trim( temp );
}
ListContact *ContactList::pcontactAt( unsigned int i )
{
   if ( i < ctList.size() )
   {
      return ctList.at( i );
   }
   return 0;
}

