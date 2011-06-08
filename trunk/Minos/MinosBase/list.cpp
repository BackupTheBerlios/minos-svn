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
#include<boost/tokenizer.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

ContactList::ContactList() : slotno( -1 ), cslFile( false ), errMessShown(false)
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

   if ( stricmp( ext.c_str(), ".csl" ) == 0 )
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
   name = fn.c_str();

   std::string sbuff;

   while ( getline( istr, sbuff ) )
   {
      sbuff = trim(sbuff);
      if (sbuff.size() == 0 || trim(sbuff)[0] == '#')
      {
         continue;
      }
      std::vector<std::string> parts;

      try
      {
         typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer ;
         tokenizer toker(sbuff);

         for( tokenizer::iterator beg=toker.begin(); beg!=toker.end();++beg)
         {
            parts.push_back((*beg));
         }
         while (parts.size() < 6)
         {
            parts.push_back("");
         }

         if ( ++nextBlock == 1 && parts[0].size() == 0 && parts[1].size() == 0 )
         {
            name = parts[ 2 ];              // first line of file gives the list name
         }
         else
         {
            rct = new ListContact();

            // a1, a2, a3 will all be set - but may point to null terminator!

            rct->cs.fullCall.setValue( strupr( parts[0] ) );

            rct->loc.loc.setValue( strupr( parts[ 1 ] ) );
            rct->loc.valRes = LOC_NOT_VALIDATED;

            rct->extraText = parts[ 2 ];
            rct->comments = parts[ 3 ];

            ctList.push_back( rct );
         }
      }
      catch (boost::escaped_list_error &err)
      {
         if (!errMessShown)
         {
            errMessShown = true;
            std::string err = "Errors in " + cfileName ;
            trace(err);
            err += (std::string("; see ") + getTraceFileName() + " for details.");
            MinosParameters::getMinosParameters() ->mshowMessage(err.c_str());
         }
         trace("Error in " + sbuff + " : " + err.what());
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

