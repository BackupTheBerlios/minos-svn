/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#define NO_WIN32_LEAN_AND_MEAN
#include "MinosControl_pch.h"
#pragma hdrstop

#include "tinyxml.h"
#include "tinyutils.h"
#include "CommonMonitor.h"
#include "controlport.h"
#include "portconf.h"


static int GetStringAttribute( TiXmlElement *e, const std::string &name, std::string &s )
{
   const std::string * a = e->Attribute( name );
   if ( !a )
   {
      return TIXML_NO_ATTRIBUTE;
   }
   s = *a;

   return TIXML_SUCCESS;
}
static int GetStringAttribute( TiXmlElement *e, const std::string &name, std::string &s, const std::string &def )
{
   int ret = GetStringAttribute( e, name, s );
   if ( ret == TIXML_NO_ATTRIBUTE )
   {
      s = def;
   }
   return ret;
}
static int GetIntAttribute( TiXmlElement *e, const std::string &name, int &i )
{
   const std::string * a = e->Attribute( name );
   if ( !a )
   {
      i = 0;
      return TIXML_NO_ATTRIBUTE;
   }

   char *endpt = 0;
   unsigned long l = strtoul( ( *a ).c_str(), &endpt, 0 );

   i = l;

   if ( endpt == ( *a ).c_str() )
      return TIXML_NO_ATTRIBUTE;

   return TIXML_SUCCESS;
}
static int GetIntAttribute( TiXmlElement *e, const std::string &name, int &i, int def )
{
   int ret = GetIntAttribute( e, name, i );
   if ( ret == TIXML_NO_ATTRIBUTE )
   {
      i = def;
   }
   return ret;
}
static int GetBoolAttribute( TiXmlElement *e, const std::string &name, bool &b )
{
   std::string val;
   int ret = GetStringAttribute( e, name, val );
   if ( ret == TIXML_SUCCESS )
   {
      if ( val[ 0 ] == 't' || val[ 0 ] == 'T' || val[ 0 ] == 'y' || val[ 0 ] == 'Y' || val[ 0 ] == '1' )
      {
         b = true;
      }
      else
      {
         b = false;
      }
   }
   return ret;
}
static int GetBoolAttribute( TiXmlElement *e, const std::string &name, bool &b, bool def )
{
   int ret = GetBoolAttribute( e, name, b );
   if ( ret == TIXML_NO_ATTRIBUTE )
   {
      b = def;
   }
   return ret;
}
//==============================================================================
//==============================================================================
void PortConfigure::SetPorts( TiXmlElement *e )
{
   for ( TiXmlElement * c = e->FirstChildElement(); c; c = c->NextSiblingElement() )
   {
      if ( checkElementName( c, "Port" ) )
      {
         // attribs name, type, port
         std::string n;
         std::string t;
         std::string cn;
         if ( GetStringAttribute( c, "name", n ) == TIXML_SUCCESS
              && GetStringAttribute( c, "type", t ) == TIXML_SUCCESS
              && GetStringAttribute( c, "connection", cn ) == TIXML_SUCCESS )
         {
            // should all this live inside PortConfig?
            PortConfig::PortType pt;
            if ( t == "Windows" )
            {
               pt = PortConfig::eptWindows;
            }
            else
               if ( t == "Serial" )
               {
                  pt = PortConfig::eptSerial;
               }
               else
                  if ( t == "K8055" )
                  {
                     pt = PortConfig::eptK8055;
                  }
                  else
                     if ( t == "UBW" )
                     {
                        pt = PortConfig::eptUBW;
                     }
                     else
                     {
                        // error...
                        continue;
                     }
            PortConfig p( n, pt, cn );
            portmap[ n ] = p;
         }
      }
   }
}
void PortConfigure::SetLines( TiXmlElement *e )
{
   for ( TiXmlElement * c = e->FirstChildElement(); c; c = c->NextSiblingElement() )
   {
      if ( checkElementName( c, "Line" ) )
      {
         //  	<Line name="L2" direction = "in" port="K8055" line="2" />
         std::string name;
         std::string inLine;
         std::string portName;
         std::string portLineName;

         if ( GetStringAttribute( c, "name", name ) == TIXML_SUCCESS )
         {
            GetStringAttribute( c, "direction", inLine, "" );
            GetStringAttribute( c, "port", portName, "" );
            GetStringAttribute( c, "line", portLineName, "" );

            LineConfig k( name, inLine, portName, portLineName );
            linemap[ name ] = k;
         }
      }
   }
}
void PortConfigure::SetEnable( TiXmlElement *e, commonController &monitor )
{
   std::string pname;
   if ( GetStringAttribute( e, "port", pname ) == TIXML_SUCCESS )
   {
      PortConfig & pc = portmap[ pname ];

      // now create the port
      commonPort *p = monitor.createPort( pc );
      // and loop through the lines adding the relevant lineConfig lines to it
      for ( std::map <std::string, LineConfig>::iterator i = linemap.begin(); i != linemap.end(); i++ )
      {
         if ( pname == ( *i ).second.portName )
         {
            p->addLine( ( *i ).second );
         }
      }
      if ( !p->initialise( pc, monitor ) )
      {
         delete p;
         return ;
      }

   }
}
bool PortConfigure::configurePorts( commonController &monitor )
{

   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument keyerdoc;
   keyerdoc.LoadFile( ".\\Configuration\\portConfig.xml" );
   TiXmlElement *root = keyerdoc.RootElement();
   if ( !root )
   {
      //err = "No XML root element (does file exist?)";
      return false;
   }
   if ( !checkElementName( root, "PortConfig" ) )
   {
      //err = "Invalid XML root element";
      return false;
   }
   for ( TiXmlElement * e = root->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      if ( checkElementName( e, "Ports" ) )
      {
         SetPorts( e );
      }
      else
         if ( checkElementName( e, "Lines" ) )
         {
            SetLines( e );
         }
         else
            if ( checkElementName( e, "Enable" ) )
            {
               SetEnable( e, monitor );
            }
            else
            {
               //         trace("Unknown");
            }
   }

   return true;
}

//==============================================================================
bool configurePorts( commonController &monitor )
{
   PortConfigure kc;
   return kc.configurePorts( monitor );
}
//==============================================================================

