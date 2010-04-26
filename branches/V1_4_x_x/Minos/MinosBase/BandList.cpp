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
#include "BandList.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
BandList::BandList()
{

}
BandList::~BandList()
{

}
/*static*/ BandList &BandList::getBandList()
{
   static BandList blist;
   static bool loaded;
   if (!loaded)
   {
      blist.parseFile( ".\\configuration\\bandlist.xml" );
      loaded = true;
   }
   return blist;
}
//---------------------------------------------------------------------------
bool BandList::parseFile( const std::string &fname )
{
#warning we need to check for file exists ands complain if not
// but we cannot do it from here - maybe publish an event?

   HANDLE bandFile = CreateFile( fname.c_str(),
                                GENERIC_READ,
                                FILE_SHARE_READ,
                                0,                  // security
                                OPEN_EXISTING,
                                FILE_FLAG_WRITE_THROUGH,
                                0 );               // template handle
   if ( bandFile == INVALID_HANDLE_VALUE )
   {
      return false;
   }
   bool fileComplete = false;

   std::string buffer;
   while ( !fileComplete )
   {
      char rdbuffer[ 4096 + 1 ];
      DWORD chRead = 0;
      DWORD chToRead = 4096;
      bool ReadOK = ReadFile ( bandFile, rdbuffer, chToRead, &chRead, 0 );
      if ( ReadOK )
      {
         if ( chRead > 0 )
         {
            rdbuffer[ chRead ] = '\0';

            buffer += rdbuffer;
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
   CloseHandle( bandFile );

   std::string buffer2;

   size_t dtdPos = buffer.find( "<!DOCTYPE" );
   if ( dtdPos != std::string::npos )
   {
      buffer2 = buffer.substr( 0, dtdPos );

      size_t dtdEndPos = buffer.find( "]>" );
      if ( dtdEndPos == std::string::npos )
      {
         return false;
      }
      buffer2 += buffer.substr( dtdEndPos + 2, buffer.size() - dtdEndPos - 2 );
   }
   else
   {
      buffer2 = buffer;
   }

   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument xdoc;
   xdoc.Parse( buffer2.c_str() );
   TiXmlElement *tix = xdoc.RootElement();
   if ( !tix || !checkElementName( tix, "Bandlist" ) )
   {
      return false;
   }
   for ( TiXmlElement * e = tix->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      if ( checkElementName( e, "Band" ) )
      {
         if ( !parseBand( e ) )     // at the moment it always returns true
         {
            return false;
         }
      }
   }
   return true;
}
/*
      std::string type;
      double flow;
      double fhigh;
      std::string wlen;
      std::string cabrillo;
      std::string adif;
      std::string reg1test;
*/
bool BandList::parseBand(TiXmlElement * e)
{
   // we know we are on a band; get the attributes we want
   BandInfo band;

   band.type = getAttribute( e, "type" );

   std::string unit = getAttribute( e, "unit" );
   std::string temp = getAttribute( e, "flow" );
   band.flow = atoi(temp.c_str());
   temp = getAttribute( e, "fhigh" );
   band.fhigh = atoi(temp.c_str());
   if (unit == "K")
   {
      band.flow  *= 1000.0;
      band.fhigh *= 1000.0;
   }
   else if (unit == "M")
   {
      band.flow  *= 1000000.0;
      band.fhigh *= 1000000.0;
   }
   else if (unit == "G")
   {
      band.flow  *= 1000000000.0;
      band.fhigh *= 1000000000.0;
   }

   band.wlen = getAttribute( e, "wlen" );
   band.uk = getAttribute( e, "UK" );
   band.reg1test = getAttribute( e, "Reg1Test" );
   band.adif = getAttribute( e, "ADIF" );
   band.cabrillo = getAttribute( e, "Cabrillo" );

   bandList.push_back(band);

   return true;
}
bool BandList::findBand(int freq, BandInfo &bi)
{
   for (std::vector<BandInfo>::iterator i = bandList.begin(); i != bandList.end(); i++)
   {
      if ((*i).flow <= freq && (*i).fhigh >= freq)
      {
         bi = (*i);
         return true;
      }
   }
   return false;
}

bool BandList::findBand(const std::string &cb, BandInfo &bi)
{
   for (std::vector<BandInfo>::iterator i = bandList.begin(); i != bandList.end(); i++)
   {
      if (stricmp(cb, (*i).uk) == 0
            || stricmp(cb, (*i).wlen) == 0
            || stricmp(cb, (*i).adif) == 0
            || stricmp(cb, (*i).cabrillo) == 0
            || stricmp(cb, (*i).reg1test) == 0)
      {
         bi = (*i);
         return true;
      }
   }
   int freq = atoi(cb.c_str());
   bool res = findBand(freq, bi);

   return res;
}

