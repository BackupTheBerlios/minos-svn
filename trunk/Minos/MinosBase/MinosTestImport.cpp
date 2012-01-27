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

MinosTestImport::MinosTestImport( BaseContestLog * const ct ) : ct( ct ),
      stanzaCount( ct->getStanzaCount() ), curfpos( 0 )
{}
MinosTestImport::MinosTestImport( ) : ct( 0 ), stanzaCount( 0 ), curfpos( 0 )
{}
MinosTestImport::~MinosTestImport()
{}
bool MinosTestImport::getStructArgMemberValueDTG( boost::shared_ptr<RPCParam>rpm, const std::string &name, std::string &val )
{
   boost::shared_ptr<RPCParam> res;
   if ( rpm && rpm->getMember( name, res ) )
   {
      std::string psval;
      if ( res->getDtg( psval ) )
      {
         val = psval;
         return true;
      }
   }
   return false;
}
bool MinosTestImport::getStructArgMemberValueDTG( boost::shared_ptr<RPCParam>rpm, const std::string &name, MinosItem<std::string> &val )
{
   std::string temp;
   bool ret = getStructArgMemberValueDTG( rpm, name, temp );
   if ( ret )
   {
      val.setInitialValue( temp );
   }
   return ret;
}
bool MinosTestImport::getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, std::string &val )
{
   boost::shared_ptr<RPCParam> res;
   if ( rpm && rpm->getMember( name, res ) )
   {
      std::string psval;
      if ( res->getString( psval ) )
      {
         val = psval;
         return true;
      }
   }
   return false;
}
bool MinosTestImport::getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, MinosItem<std::string> &val )
{
   std::string temp;
   bool ret = getStructArgMemberValue( rpm, name, temp );
   if ( ret )
   {
      val.setInitialValue( temp );
   }
   return ret;
}
bool MinosTestImport::getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, int &val )
{
   boost::shared_ptr<RPCParam> res;
   if ( rpm && rpm->getMember( name, res ) )
   {
      int psval;
      if ( res->getInt( psval ) )
      {
         val = psval;
         return true;
      }
   }
   return false;
}
bool MinosTestImport::getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, MinosItem<int> &val )
{
   int temp;
   bool ret = getStructArgMemberValue( rpm, name, temp );
   if ( ret )
   {
      val.setInitialValue( temp );
   }
   return ret;
}
bool MinosTestImport::getStructArgMemberValue( boost::shared_ptr<RPCParam>rpm, const std::string &name, bool &val )
{
   boost::shared_ptr<RPCParam> res;
   if ( rpm && rpm->getMember( name, res ) )
   {
      bool psval;
      if ( res->getBoolean( psval ) )
      {
         val = psval;
         return true;
      }
   }
   return false;
}
bool MinosTestImport::getStructArgMemberValue( boost::shared_ptr<RPCParam> rpm, const std::string &name, MinosItem<bool> &val )
{
   bool temp;
   bool ret = getStructArgMemberValue( rpm, name, temp );
   if ( ret )
   {
      val.setInitialValue( temp );
   }
   return ret;
}
bool MinosTestImport::getStructArgMemberValueDTG( const std::string &name, std::string &val )
{
   return getStructArgMemberValueDTG( body, name, val );
}
bool MinosTestImport::getStructArgMemberValueDTG( const std::string &name, MinosItem<std::string> &val )
{
   return getStructArgMemberValueDTG( body, name, val );
}
bool MinosTestImport::getStructArgMemberValue( const std::string &name, std::string &val )
{
   return getStructArgMemberValue( body, name, val );
}
bool MinosTestImport::getStructArgMemberValue( const std::string &name, MinosItem<std::string> &val )
{
   return getStructArgMemberValue( body, name, val );
}
bool MinosTestImport::getStructArgMemberValue( const std::string &name, int &val )
{
   return getStructArgMemberValue( body, name, val );
}
bool MinosTestImport::getStructArgMemberValue( const std::string &name, MinosItem<int> &val )
{
   return getStructArgMemberValue( body, name, val );
}
bool MinosTestImport::getStructArgMemberValue( const std::string &name, bool &val )
{
   return getStructArgMemberValue( body, name, val );
}
bool MinosTestImport::getStructArgMemberValue( const std::string &name, MinosItem<bool> &val )
{
   return getStructArgMemberValue( body, name, val );
}
void MinosTestImport::setBit( unsigned short &cf, unsigned short bit, bool set
                               )
{
   cf &= ~bit;
   if ( set
         )
      {
         cf |= bit;
      }
}
void MinosTestImport::processMinosStanza( RPCRequest *rq )
{

   // This needs to be "inverted" and passed to the contest object to get the bits it
   // understands

   stanzaCount++;

   //   fpos is a COUNT not a position - so why do we end up 1 short?

   body = rq->args[ 0 ];

   ct->setStanza( stanzaCount, curfpos );
   ct->processMinosStanza( rq->methodName, this );

}

/*
   void processMinosStanza(RPCRequest *rq, long fpos, void *user )
   {
      MinosTestImport *mti = reinterpret_cast<MinosTestImport *> (user);
      mti->processMinosStanza(rq, fpos);
   }
*/

//==============================================================================
// These are used for log monitoring
void MinosTestImport::startImportTest()
{}
int MinosTestImport::importTestBuffer( const std::string &buffer )
{
   // called from monitoredLog when we get a "random" stanza in

   DispatchCallback oldcall = dispatchCallback;
   dispatchCallback = &MinosTestImport::analyseImportTest;

   analyseNode( this, buffer );

   dispatchCallback = oldcall;

   return stanzaCount;
}
void MinosTestImport::endImportTest()
{}
//==============================================================================
char *header1 = "<?xml version='1.0'?><stream:stream xmlns:stream='http://minos.goodey.org.uk/streams' xmlns='minos:client' version='1.0'>";
char *header2 = "<?xml version='1.0'?><stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' version='1.0'>";
char *stubHeader = "<stream:stream>";
void MinosTestImport::dispatchResponse( XStanza *xs )
{
   ( this->*dispatchCallback ) ( xs ); // What a horrid syntax for calling through a member pointer!
}
#define IO_BUF_SIZE 4096
int MinosTestImport::readTestFile( HANDLE ctfile )
{
   // read the stream as a sequence of Minos stanzas
   char rdbuffer[ IO_BUF_SIZE + 1 ];
   bool fileComplete = false;

   // NB - old versions might not have a proper header
   bool firstRead = true;
   std::string buffer;
   while ( !fileComplete )
   {
      DWORD chRead = 0;
      DWORD chToRead = IO_BUF_SIZE;
      bool ReadOK = ReadFile ( ctfile, rdbuffer, chToRead, &chRead, 0 );
      if ( ReadOK )
      {
         if ( chRead > 0 )
         {
            rdbuffer[ chRead ] = '\0';

            buffer += rdbuffer;
            if ( firstRead )
            {
               firstRead = false;
               iqOffset = 0;
               if ( strncmp( rdbuffer, header1, strlen( header1 ) ) == 0 )
               {
                  buffer = buffer.substr( strlen( header1 ), buffer.size() - strlen( header1 ) );  // overwriting what is there already
                  iqOffset = strlen( header1 );
               }
               else
                  if ( strncmp( rdbuffer, header2, strlen( header2 ) ) == 0 )
                  {
                     buffer = buffer.substr( strlen( header2 ), buffer.size() - strlen( header2 ) );  // overwriting what is there already
                     iqOffset = strlen( header2 );
                  }
               buffer = std::string( stubHeader ) + buffer;
            }
         }
         else
         {
            fileComplete = true;
         }

      }
      else
      {
         fileComplete = true;
      }
   }
   // and now we parse the document, and grind through the root
   buffer += "</stream:stream>";

   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument xdoc;
   xdoc.Parse( buffer.c_str(), 0 );
   TiXmlElement *tix = xdoc.RootElement();
   if ( !tix || !checkElementName( tix, "stream:stream" ) )
   {
      return 0;
   }
   int stanzas = 0;
   for ( TiXmlElement * e = tix->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      stanzas++;

      // Tiny gives us a "cursor" of row/column on the parsed data, but this
      // doesn't actually translate at all easily to a file position

      // I've added the code to get a data offset (DataPos)

      // as we get a position of the START rather than the END of a stanza,
      // we don't always get a good length, so we may need to do a "large" read in thos cases

      curfpos = iqOffset + e->DataPos() - 1 - strlen( stubHeader );
      analyseNode( this, e );
   }
   return stanzas;
}
//=============================================================================
int MinosTestImport::importTest( HANDLE ctfile )
{
   stanzaCount = 0;

   dispatchCallback = &MinosTestImport::analyseImportTest;

   if ( readTestFile( ctfile ) )
      return stanzaCount;

   return 0;
}
void MinosTestImport::analyseImportTest( XStanza *xs )
{
   RPCRequest * req = dynamic_cast<RPCRequest *>( xs );
   if ( req )
   {
      processMinosStanza( req );
   }
}
//=============================================================================
bool MinosTestImport::analyseTest( HANDLE ctfile )
{
   dispatchCallback = &MinosTestImport::analyseImportLog;
   return readTestFile( ctfile );
}
void MinosTestImport::analyseImportLog( XStanza *xs )
{
   RPCRequest * req = dynamic_cast<RPCRequest *>( xs );
   if ( req )
   {
      processLogMinosStanza( req );
   }
}
void MinosTestImport::processLogMinosStanza( RPCRequest *rq )
{
   // We need to go through and log the tags and values in this node

   std::string log = rq->analyse();

   #warning here is where we need to analyse the sequence number - or should we do it in the analyse with a callback
   // to here with the name?
   trace( rq->methodName + " : " + log );
}
//==============================================================================

