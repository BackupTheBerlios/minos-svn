/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "MinosVK_pch.h"
#pragma hdrstop

#include "tinyxml.h"
#include "tinyutils.h"
#include "keyers.h"
#include "portcon.h"
#include "keyconf.h"


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
// The keyer factory
commonKeyer *KeyerConfigure::createKeyer( const KeyerConfig &keyer, const PortConfig &port )
{
   commonKeyer * ck = 0;

   ck = new voiceKeyer( keyer, port );
   if ( sblog )
   {
      trace( "keyer created" );
   }
   if ( !ck->initialise( keyer, port ) )
   {
      delete ck;
      ck = 0;
   }

   return ck;
}
//==============================================================================
//==============================================================================
void KeyerConfigure::SetTrace( TiXmlElement *e )
{
   bool tval;
   if ( GetBoolAttribute( e, "enabled", tval, false ) == TIXML_SUCCESS )
   {
      sblog = tval;
   }
}
void KeyerConfigure::SetTune( TiXmlElement *e )
{
   int tval;
   if ( GetIntAttribute( e, "time", tval, 20 ) == TIXML_SUCCESS )
   {
      if ( tval > 0 && tval <= 30 )
         tuneTime = tval;
      else
      {
         trace( "Incorrect argument to attribute `Tune Time' - must be > 0 and <= 30 (secs)" );
      }
   }
   if ( GetIntAttribute( e, "level", tval, 80 ) == TIXML_SUCCESS )
   {
      if ( tval > 0 && tval <= 100 )
         tuneLevel = tval;
      else
      {
         trace( "Incorrect argument to attribute `Tune Level' - must be > 0 and <= 100 (%)" );
      }
   }
}
void KeyerConfigure::SetPorts( TiXmlElement *e )
{
   for ( TiXmlElement * c = e->FirstChildElement(); c; c = c->NextSiblingElement() )
   {
      if ( checkElementName( c, "Port" ) )
      {
         // attribs name, type, port
         std::string n;
         std::string t;
         if ( GetStringAttribute( c, "name", n ) == TIXML_SUCCESS
              && GetStringAttribute( c, "type", t ) == TIXML_SUCCESS )
         {
            // should all this live inside PortConfig?
            PortConfig::PortType pt;
            if ( t == "Windows" )
            {
               pt = PortConfig::eptWindows;
            }
            else
               if ( t == "MinosControl" )
               {
                  pt = PortConfig::eptMinosControl;
               }
               else
               {
                  // error...
                  continue;
               }
            PortConfig p( n, pt );
            portmap[ n ] = p;
         }
      }
   }
}
void KeyerConfigure::SetKeyers( TiXmlElement *e )
{
   /*
      void procKeyerParam( void *self, const std::string &pname, int pvalue )
      {
         lineMonitor * lm = ( lineMonitor * ) self;
         lm->procParam( pname, pvalue );
      }
   */
   for ( TiXmlElement * c = e->FirstChildElement(); c; c = c->NextSiblingElement() )
   {
      if ( checkElementName( c, "Keyer" ) )
      {
         // attribs name PipTone EnablePip StartDelay AutoRepeat PipStartDelay PlayPTTDelay
         std::string name;
         int PipTone;
         int PipLength;
         bool EnablePip;
         int StartDelay;
         int AutoRepeatDelay;
         bool EnableAutoRepeat;
         int PipStartDelay;
         int PlayPTTDelay;
         int voxHangTime;
         int pipVolume;
         int ClipRecord;

         if ( GetStringAttribute( c, "name", name ) == TIXML_SUCCESS )
         {
            GetIntAttribute( c, "pipTone", PipTone, 1000 );
            GetIntAttribute( c, "pipLength", PipLength, 250 );
            GetBoolAttribute( c, "enablePip", EnablePip, false );
            GetIntAttribute( c, "pipStartDelay", PipStartDelay, 0 );
            GetIntAttribute( c, "pipVolume", pipVolume, 80 );
            GetIntAttribute( c, "startDelay", StartDelay, 0 );
            GetIntAttribute( c, "autoRepeat", AutoRepeatDelay, 6 );
            GetBoolAttribute( c, "enableAutoRepeat", EnableAutoRepeat, false );
            GetIntAttribute( c, "playPTTDelay", PlayPTTDelay, 0 );
            GetIntAttribute( c, "voxHangTime", voxHangTime, 0 );
            GetIntAttribute( c, "clipRecord", ClipRecord, 0 );

            KeyerConfig k( name, PipTone, pipVolume, PipLength, EnablePip, StartDelay,
                           AutoRepeatDelay, EnableAutoRepeat, PipStartDelay, PlayPTTDelay, voxHangTime,
                           ClipRecord );
            keyermap[ name ] = k;
         }
      }
   }
}
void KeyerConfigure::SetEnable( TiXmlElement *e )
{
   //attribs Keyer, Port - refer to names of KeyerConfig, PortConfig
   std::string k;
   std::string p;
   if ( GetStringAttribute( e, "keyer", k ) == TIXML_SUCCESS
        && GetStringAttribute( e, "port", p ) == TIXML_SUCCESS )
      createKeyer( keyermap[ k ], portmap[ p ] );
}
void KeyerConfigure::SetCW( TiXmlElement *e )
{
   int tone;
   int speed;
   if ( GetIntAttribute( e, "tone", tone, 1000 ) == TIXML_SUCCESS )
   {
      CWTone = tone;
   }
   if ( GetIntAttribute( e, "speed", speed, 12 ) == TIXML_SUCCESS )
   {
      CWSpeed = speed;
   }
}
void KeyerConfigure::SetMorseCode( TiXmlElement *e )
{
   /*
      int codeinc;
      void procCodeParam(const std::string &pname, const std::string &pvalue )
      {
         MORSELETTER[ codeinc ] = toupper( pname[ 0 ] );
    
         for ( int i = 0; i < 8 && pvalue[ i ] && pvalue[ i ] != ' '; i++ )
         {
            if ( pvalue[ i ] == '.' )
               MORSECODE[ codeinc ][ i ] = 0x40;
            if ( pvalue[ i ] == '-' )
               MORSECODE[ codeinc ][ i ] = 0x80;
         }
         codeinc++;
      }
      void defineCode( const ParamTable &code )
      {
         memset( MORSELETTER, 0, MORSESIZE );
         memset( MORSECODE, 0, MORSESIZE * 8 );
         codeinc = 0;
         code.forEachParam( 0, 0, procCodeParam );
      }
   */
   for ( TiXmlElement * c = e->FirstChildElement(); c; c = c->NextSiblingElement() )
   {
      if ( checkElementName( c, "Key" ) )
      {
         std::string character;
         std::string code;
         if ( GetStringAttribute( c, "character", character ) == TIXML_SUCCESS
              && GetStringAttribute( c, "code", code ) == TIXML_SUCCESS )
         {
            std::string proccode;
            for ( int i = 0; i < 8 && code[ i ] && code[ i ] != ' '; i++ )
            {
               if ( code[ i ] == '.' )
                  proccode += '\x40';
               if ( code[ i ] == '-' )
                  proccode += '\x80';
            }
            MORSECODE[ character[ 0 ] ] = proccode;
         }
      }
   }
}
//==============================================================================
/*
-- Variable strings
 
-- $MyCall, $HisCall, $CorrectedCall, $RST, $Serial, $MyLocator, $MyQTH,
-- $CurrentOperator
 
-- are interpreted at transmission time
 
-- Should the callbacks add a space before or after the characters? Ideally not
-- but will this confuse novices.
 
We need a some kind of separator to allow us to parse things at all!
 
-- all messages will be converted to "UPPER CASE" before sending
 
CWMessages = {
	["1"] = "CQ CQ CQ de $MyCall",
	["2"] = "$MyCall",
	["3"] = "$HisCall",
	["4"] = "$CorrectedCall UR $RST $Serial at $MyLocator $MyQTH",
	}
SetMessages(CWMessages)
 
// WGV codings:
 
$1 mycall
$2 "^"
$3 "&"
$4 serial sent
$5 myloc
$6 file contents
$7 his call (autocorrect; only if not as originally sent)
 
// we will use???
 
*/
MORSEMSG::MORSEMSG() : repeat( false )
{}
MORSEMSG::~MORSEMSG()
{}
void MORSEMSG::setMessage( const std::string &pvalue )
{
   msg = pvalue;
}
void KeyerConfigure::SetCWMessages( TiXmlElement *e )
{
   /*
      void procMessParam(const std::string &pname, const std::string &pvalue )
      {
         // not very good, but may do for now.
         // We should look at how we number the strings (F1, F2,... or 1, 2,..., or what)
         // We also ought to parse the value string for $tokens, and put in the usual
         // tokens instead
         int mno;
         char mbuff[ 120 ];
         mno = atoi( pname.c_str() );
         if ( mno > 0 && mno <= 10 )
         {
            MORSEMSGS[ mno - 1 ].setMessage( pvalue );
            sprintf( mbuff, "Mno %d value %s\r\n", mno, MORSEMSGS[ mno - 1 ].msg.c_str() );
         }
         else
         {
            sprintf( mbuff, "pname %s value %s\r\n", pname.c_str(), pvalue.c_str() );
         }
         trace( mbuff );
      }
      void defineMessages( const ParamTable &mess )
      {
         mess.forEachParam( 0, 0, procMessParam );
      }
   */
   for ( TiXmlElement * c = e->FirstChildElement(); c; c = c->NextSiblingElement() )
   {
      if ( checkElementName( c, "Message" ) )
      {
         // Attrib id, autoRepeat; body is message
         // Create a MORSEMSG, and put it into MORSEMSGS
      }
   }
}
bool KeyerConfigure::configureKeyers()
{

   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument keyerdoc;
   keyerdoc.LoadFile( ".\\Configuration\\keyerConfig.xml" );
   TiXmlElement *root = keyerdoc.RootElement();
   if ( !root )
   {
      //err = "No XML root element (does file exist?)";
      return false;
   }
   if ( !checkElementName( root, "KeyerConfig" ) )
   {
      //err = "Invalid XML root element";
      return false;
   }
   for ( TiXmlElement * e = root->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      if ( checkElementName( e, "Trace" ) )
      {
         SetTrace( e );
      }
      else
         if ( checkElementName( e, "Tune" ) )
         {
            SetTune( e );
         }
         else
            if ( checkElementName( e, "Ports" ) )
            {
               SetPorts( e );
            }
            else
               if ( checkElementName( e, "Keyers" ) )
               {
                  SetKeyers( e );
               }
               else
                  if ( checkElementName( e, "Enable" ) )
                  {
                     SetEnable( e );
                  }
                  else
                     if ( checkElementName( e, "CW" ) )
                     {
                        SetCW( e );
                     }
                     else
                        if ( checkElementName( e, "MorseCode" ) )
                        {
                           SetMorseCode( e );
                        }
                        else
                           if ( checkElementName( e, "CWMessages" ) )
                           {
                              SetCWMessages( e );
                           }
                           else
                           {
                              //         trace("Unknown");
                           }
   }
   return true;
}

//==============================================================================
bool configureKeyers()
{
   KeyerConfigure kc;
   return kc.configureKeyers();
}
//==============================================================================
//==============================================================================

