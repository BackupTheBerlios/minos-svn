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
#include "mwin.h"
#include "cutils.h"
#include "tinyxml.h"
#include "tinyutils.h"
#include "vkmixer.h"
#ifndef _INC_MMSYSTEM
#error this should already be included... 
// but it is useful to be able to open it easily
#include <mmsystem.h>
#endif

std::vector<boost::shared_ptr<VKMixer> > VKMixer::inputMixers;
std::vector<boost::shared_ptr<VKMixer> > VKMixer::outputMixers;
VKMixer * VKMixer::inputMixer = 0;
std::string VKMixer::inputLine;
VKMixer * VKMixer::outputMixer = 0;
std::string VKMixer::outputLine;

//==============================================================================
std::string boolVal( DWORD d )
{
   return std::string( d ? "1" : "0" );
}
std::string intVal( DWORD d )
{
   char buff[ 20 ];
   sprintf( buff, "%d", d );
   return std::string( buff );
}
std::string uintVal( DWORD d )
{
   char buff[ 20 ];
   sprintf( buff, "%u", d );
   return std::string( buff );
}
std::string hexVal( DWORD d )
{
   char buff[ 20 ];
   sprintf( buff, "0x%x", d );
   return std::string( buff );
}
//==============================================================================
//---------------------------------------------------------------------------
static int GetIntAttribute( TiXmlElement *e, const char *name, int &i )
{
   const char * a = e->Attribute( name );
   if ( !a )
   {
      i = 0;
      return TIXML_NO_ATTRIBUTE;
   }

   char *endpt = 0;
   unsigned long l = strtoul( a, &endpt, 0 );

   i = l;

   if ( endpt == a )
      return TIXML_NO_ATTRIBUTE;

   return TIXML_SUCCESS;
}
//---------------------------------------------------------------------------
// Volume settings required in different situations.
// To be applied to the relevant MixerSet for the current
// situation
/*
Tabs:
 
Unloaded
PassThrough No PTT
Replay
Replay Pip
PassThrough PTT
PTT Release Pip
Mic Monitor
Replay Monitor
 
enum eMixerSets {emsUnloaded, emsPassThroughNoPTT, emsPassThroughPTT,
                emsReplay, emsReplayPip, emsReplayT1, emsReplayT2,
                emsVoiceRecord,
                emsCWTransmit, emsCWPassThrough,
                emsMaxMixerSet};
*/
eMixerSets CurrMixerSet = emsUnloaded;

DWORD baseMicRec = 0;
DWORD baseMicOut = 0;
DWORD baseWaveOut = 0;
DWORD baseRec = 0;
DWORD baseMaster = 0;

MixerSet MixerSets[ emsMaxMixerSet ] =
   {
      // revise to MicOut mute, Speaker mute as only ones we need to drive
      // and we need to mute or drive to zero if no mute.
      // MixerSet(MicRec, MicOut,  Rec,   Master)
      // MixerSet(bool MicOutMute, bool MasterMute)

      MixerSet( false, false ),         //emsUnloaded
      MixerSet( false, false ),         //emsPassThroughNoPTT
      MixerSet( false, false ),         //emsPassThroughPTT
      MixerSet( true , false ),         //emsReplay
      MixerSet( true , false ),         //emsReplayPip
      MixerSet( true , false ),         //emsReplayT1
      MixerSet( true , false ),         //emsReplayT2
      MixerSet( false, true ),         //emsVoiceRecord
      MixerSet( true , false ),         //emsCWTransmit
      MixerSet( true , false )         //emsCWPassThrough
   };
MixerSet * _DLL_FUNCTION GetMixerSets()
{
   return MixerSets;
}
eMixerSets _DLL_FUNCTION GetCurrentMixerSet()
{
   return CurrMixerSet;
}

//enum controltype {ecSetUsed, ecSetNotUsed, ecNoSet};

void _DLL_FUNCTION SetCurrentMixerSet( eMixerSets cms )
{

   CurrMixerSet = cms;
}

//---------------------------------------------------------------------------

// Code to be removed...

// The most characters that an error message can contain
#define APP_MAX_STRING_ERROR_CHARS      512
#define APP_MAX_STRING_ERROR_BYTES      (APP_MAX_STRING_ERROR_CHARS * sizeof(TCHAR))
const TCHAR	gszMixerErr[] = TEXT( "Error #%lu getting info upon Mixer #%lu!" );
const TCHAR	gszMixerErr2[] = TEXT( "Error #%lu opening Mixer #%lu!" );
//const TCHAR		gszMixerErr3[] = TEXT("Error #%lu closing previously open Mixer!");
//const TCHAR		gszMixerErr4[] = TEXT("Error %lu getting info upon Mixer!");
const TCHAR	gszLineErr2[] = TEXT( "Error %lu getting info on Component %lu!" );
const TCHAR	gszParamErr[] = TEXT( "Error #%u getting info upon parameters!" );
const TCHAR	gszControlErr[] = TEXT( "Error - no data for control!" );
// ************************** getComponentType() **************************
// Gets a string that describes the type of audio line for the specified
// mixer line.
//
// NOTE: This must not return a string longer than MIXER_COMPONENT_NAMELEN.
//
//	ARGS:
//		MIXERLINE *mixerLine:	MIXERLINE struct filled-in with information
//								about the desired line.
//
//	RETURNS:
//		Pointer to the string.

//==============================================================================
const char * VKMixerControls::getComponentType( unsigned int n )
{
   return mixerLineControls.pamxctrl[ n ].szName;
}
const char * VKMixerLine::getComponentType()
{
   return lineinfo.szName;
}
//==============================================================================

// ******************************* doMsgBox() *************************
// Formats/displays a string (using wsprintf()) in a standard message
// box. The resulting string may be no larger than
// APP_MAX_STRING_ERROR_CHARS (defined in MIXAPP.H).
//
// ARGS:
//		HWND hwnd:			Handle to parent window for message box.
//
//		UINT fuStyle:		Style flags for MessageBox().
//
//		PTSTR pszFormat:	Format string used for wsprintf().
//
// RETURNS:
//		The result of MessageBox() function.

int doMsgBox( HWND hwnd, UINT fuStyle, LPCTSTR pszFormat, ... )
{
   va_list	va;
   TCHAR	buffer[ APP_MAX_STRING_ERROR_CHARS ];

   // Format the message
   va_start( va, pszFormat );
   wvsprintf( &buffer[ 0 ], pszFormat, va );
   va_end( va );


   // Display the messagebox and return its result
   return ( MessageBox( hwnd, &buffer[ 0 ], "doMsgBox", fuStyle ) );
}
//---------------------------------------------------------------------------
VKMixer::VKMixer() : inputWaveInID( -1 ), outputWaveOutID( -1 ), mixerID(-1), hMixer( 0 )
{
   memset( &mixercaps, 0, sizeof( MIXERCAPS ) );
}
void VKMixer::Close()
{
   if ( hMixer )
      mixerClose( hMixer );
   for ( unsigned int i = 0; i < destlines.size();i++ )
      delete destlines[ i ];
   destlines.clear();
}
VKMixer::~VKMixer()
{
   Close();
}
/*static*/ VKMixer *VKMixer::GetInputVKMixer()
{
   return inputMixer;
}
/*static*/ int VKMixer::getWaveInID()
{
   return inputMixer->inputWaveInID;
}
/*static*/ VKMixer *VKMixer::GetOutputVKMixer()
{
   return outputMixer;
}
/*static*/ int VKMixer::getWaveOutID()
{
   return outputMixer->outputWaveOutID;
}
/*static*/ void VKMixer::closeMixer()
{
   outputMixer = 0;
   inputMixer = 0;
   for ( std::vector<boost::shared_ptr<VKMixer> >::iterator i = inputMixers.begin(); i != inputMixers.end(); i++ )
   {
	  ( *i ) ->Close();
	  //      delete(*i).get();  // shared pointers - DO NOT delete them!
   }
   inputMixers.clear();
   for ( std::vector<boost::shared_ptr<VKMixer> >::iterator i = outputMixers.begin(); i != outputMixers.end(); i++ )
   {
	  ( *i ) ->Close();
	  //      delete(*i).get();  // shared pointers - DO NOT delete them!
   }
   outputMixers.clear();

}
std::string VKMixer::getMixerName()
{
   return trim( mixercaps.szPname );
}
//---------------------------------------------------------------------------
VKMixerControls::VKMixerControls()
{
   memset ( &mixerLineControls, 0, sizeof( MIXERLINECONTROLS ) );
   mixerLineControls.cbStruct = sizeof( MIXERLINECONTROLS );
   mixerLineControls.cbmxctrl = sizeof( MIXERCONTROL );
}
VKMixerControls::~VKMixerControls()
{
   if ( mixerLineControls.pamxctrl )
      delete [] mixerLineControls.pamxctrl;
   mixerLineControls.pamxctrl = 0;
}
void VKMixerControls::initialise( VKMixerLine *d, HMIXEROBJ h )
{
   line = d;
   hMixer = h;
   mixerLineControls.dwLineID = d->lineinfo.dwLineID;

   unsigned int nControls = d->lineinfo.cControls;
   mixerLineControls.cControls = nControls;

   if ( nControls == 0 )
      return ;

   // Get memory for enough MIXERCONTROL structs to hold info on all parameters
   mixerLineControls.pamxctrl = new MIXERCONTROL [ nControls ];
   memset ( mixerLineControls.pamxctrl, 0, nControls * sizeof( MIXERLINECONTROLS ) );
   // Get info upon all parameters

   MMRESULT err = mixerGetLineControls( hMixer, &mixerLineControls, MIXER_GETLINECONTROLSF_ALL );
   if ( err != MMSYSERR_NOERROR )
   {
      delete [] mixerLineControls.pamxctrl;
      mixerLineControls.pamxctrl = 0;
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszParamErr, err );
   }
}
//---------------------------------------------------------------------------
VKMixerLine::VKMixerLine()
{
   memset ( &lineinfo, 0, sizeof( MIXERLINE ) );
   lineinfo.cbStruct = sizeof( MIXERLINE );
}
VKMixerLine::~VKMixerLine()
{}
//---------------------------------------------------------------------------
VKSourceLine::VKSourceLine()
{}
VKSourceLine::~VKSourceLine()
{}
void VKSourceLine::initialise( VKDestinationLine *dest, HMIXEROBJ hMixer, unsigned int v )
{
   // ...get info upon that source line
   lineinfo.dwDestination = dest->lineinfo.dwDestination;
   lineinfo.dwSource = v;

   MMRESULT err = mixerGetLineInfo( ( HMIXEROBJ ) hMixer, &lineinfo, MIXER_GETLINEINFOF_SOURCE );
   if ( err != MMSYSERR_NOERROR )
   {
      // An error
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszLineErr2, err, v );
      return ;
   }

   // Get all the controls

   controls.initialise( this, hMixer );

}
//---------------------------------------------------------------------------
VKDestinationLine::VKDestinationLine()
{}
VKDestinationLine::~VKDestinationLine()
{
   for ( unsigned int i = 0; i < sourcelines.size();i++ )
      delete sourcelines[ i ];
   sourcelines.clear();
}
void VKDestinationLine::initialise( HMIXEROBJ hMixer, unsigned int u )
{
   // ...get info on that destination line
   lineinfo.dwDestination = u;

   MMRESULT err = mixerGetLineInfo( ( HMIXEROBJ ) hMixer, &lineinfo, MIXER_GETLINEINFOF_DESTINATION );
   if ( err != MMSYSERR_NOERROR )
   {
      // An error. Notify user and ask him if he wants to continue !!!
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszLineErr2, err, u );
      return ;
   }
   // Get all the controls

   controls.initialise( this, hMixer );

   // For each source line that can be connected to this destination...
   unsigned int cConnections = ( UINT ) lineinfo.cConnections;
   for ( unsigned int v = 0; v < cConnections; v++ )
   {
      sourcelines.push_back( new VKSourceLine );
      sourcelines[ v ] ->initialise( this, hMixer, v );
   }
}
//---------------------------------------------------------------------------
bool VKMixer::inputOpen( unsigned int mId, unsigned int waveinid, HWND callbackWnd )
{
   mixerID = mId;
   inputWaveInID = waveinid;
   return Open(callbackWnd);
}
bool VKMixer::outputOpen( unsigned int mId, unsigned int waveoutid, HWND callbackWnd )
{
   mixerID = mId;
   outputWaveOutID = waveoutid;
   return Open(callbackWnd);
}
bool VKMixer::Open( HWND callbackWnd )
{

   // Get info about requested mixer device
   MMRESULT	err = mixerGetDevCaps( mixerID, &mixercaps, sizeof( MIXERCAPS ) );
   if ( err != MMSYSERR_NOERROR )
   {
      // Can't query the new mixer device
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszMixerErr, err, mixerID );

      // Return handle to previously opened mixer without closing it
      return false;
   }

   err = mixerOpen( &hMixer, mixerID, ( DWORD ) callbackWnd, 0L, callbackWnd ? CALLBACK_WINDOW : 0 );
   if ( err != MMSYSERR_NOERROR )
   {
      // Can't open the new mixer device
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszMixerErr2, err, mixerID );
      return false;
   }

   // For each destination line...
   for ( unsigned int u = 0; u < mixercaps.cDestinations; u++ )
   {
      destlines.push_back( new VKDestinationLine );
      destlines[ u ] ->initialise( hMixer, u );
   }
   return true;
}
//---------------------------------------------------------------------------
const char * VKMixer::getComponentType()
{
   return mixercaps.szPname;
}
//---------------------------------------------------------------------------
bool VKMixerControls::Find( unsigned int controltype, DWORD &id )
{
   if ( mixerLineControls.pamxctrl )
   {
      for ( unsigned int i = 0; i < mixerLineControls.cControls; i++ )
      {
         if ( mixerLineControls.pamxctrl[ i ].dwControlType == controltype )
         {
            id = mixerLineControls.pamxctrl[ i ].dwControlID;
            return true;
         }
      }
   }
   return false;
}
bool VKMixerLine::Find( unsigned int controltype, DWORD &id )
{
   return controls.Find( controltype, id );
}
bool VKDestinationLine::Find( unsigned int srclinetype, unsigned int controltype, DWORD &id )
{
   if ( srclinetype == 0 )
      return controls.Find( controltype, id );

   for ( unsigned int i = 0; i < sourcelines.size(); i++ )
   {
      if ( sourcelines[ i ] ->lineinfo.dwComponentType == srclinetype )
         return sourcelines[ i ] ->Find( controltype, id );
   }
   return false;
}
bool VKDestinationLine::FindLineControl( DWORD srclineid, unsigned int controltype, DWORD &id )
{
   for ( unsigned int i = 0; i < sourcelines.size(); i++ )
   {
      if ( sourcelines[ i ] ->lineinfo.dwLineID == srclineid )
         return sourcelines[ i ] ->Find( controltype, id );
   }
   return false;
}

bool VKMixer::Find( unsigned int destlinetype, unsigned int srclinetype, unsigned int controltype, DWORD &id )
{
   // BUT we can probably use the target type of WaveIn/WaveOut
   // dwComponentType gives MIXERLINE_COMPONENTTYPE_DST_* for destination type

   // then MIXERLINE_COMPONENTTYPE_SRC_* for the source line types
   // (see getComponentType above)

   // Then the controls dwControlType is a mixture of
   // MIXERCONTROL_CT_CLASS_, MIXERCONTROL_CT_SC_ (subclass), and MIXERCONTROL_CT_UNITS_*
   // ored together, giving a MIXERCONTROL_CONTROLTYPE_*

   for ( unsigned int i = 0; i < destlines.size(); i++ )
   {
      if ( destlines[ i ] ->lineinfo.dwComponentType == destlinetype )
      {
         return destlines[ i ] ->Find( srclinetype, controltype, id );
      }
   }

   return false;
}
bool VKMixer::FindLineControl( unsigned int destlinetype, DWORD srclineid, unsigned int controltype, DWORD &id )
{
   for ( unsigned int i = 0; i < destlines.size(); i++ )
   {
      if ( destlines[ i ] ->lineinfo.dwComponentType == destlinetype )
      {
         return destlines[ i ] ->FindLineControl( srclineid, controltype, id );
      }
   }

   return false;
}

std::vector<std::string> VKMixer::getOutputLines()
{
   std::vector<std::string> lines;
   // dest = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,src = 0
   for ( unsigned int i = 0; i < destlines.size(); i++ )
   {
      if ( destlines[ i ] ->lineinfo.dwComponentType == MIXERLINE_COMPONENTTYPE_DST_SPEAKERS )
      {
         lines.push_back( destlines[ i ] ->getComponentType() );
      }
   }
   return lines;
}
std::vector<std::string> VKMixer::getInputLines()
{
   //   if ( Find( MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
   //              MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,
   //              MIXERCONTROL_CONTROLTYPE_VOLUME, mic ) )
   std::vector<std::string> lines;
   for ( unsigned int i = 0; i < destlines.size(); i++ )
   {
      if ( destlines[ i ] ->lineinfo.dwComponentType == MIXERLINE_COMPONENTTYPE_DST_WAVEIN )
      {
         unsigned int slsize = destlines[ i ] ->sourcelines.size();
         for ( unsigned int j = 0; j < slsize; j++ )
         {
            VKSourceLine *s = destlines[ i ] ->sourcelines[ j ];
            if ( s && s ->lineinfo.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE )
               lines.push_back( s ->getComponentType() );
         }
      }
   }
   return lines;

}
//---------------------------------------------------------------------------
MIXERCONTROL * VKMixerControls::FindId( unsigned int controlid )
{
   if ( mixerLineControls.pamxctrl )
   {
      for ( unsigned int i = 0; i < mixerLineControls.cControls; i++ )
      {
         if ( mixerLineControls.pamxctrl[ i ].dwControlID == controlid )
         {
            return & mixerLineControls.pamxctrl[ i ];
         }
      }
   }
   return 0;
}
MIXERCONTROL * VKMixerLine::FindId( unsigned int controlid )
{
   return controls.FindId( controlid );
}
MIXERCONTROL * VKDestinationLine::FindId( unsigned int sourceid, unsigned int controlid )
{
   if ( sourceid == ( unsigned int ) - 1 )
      return VKMixerLine::FindId( controlid );

   for ( unsigned int i = 0; i < sourcelines.size(); i++ )
   {
      if ( sourcelines[ i ] ->lineinfo.dwSource == sourceid )
      {
         return sourcelines[ i ] ->FindId( controlid );
      }
   }
   return 0;
}
MIXERCONTROL * VKMixer::FindId( unsigned int destinationid, unsigned int sourceid, unsigned int controlid )
{
   for ( unsigned int i = 0; i < destlines.size(); i++ )
   {
      if ( destlines[ i ] ->lineinfo.dwDestination == destinationid )
      {
         return destlines[ i ] ->FindId( sourceid, controlid );
      }
   }
   return 0;
}
//---------------------------------------------------------------------------
bool VKMixer::GetVolume( unsigned int controlID, DWORD &vol )
{
   if ( !hMixer )
      return false;

   MIXERCONTROLDETAILS mcd;
   MIXERCONTROLDETAILS_UNSIGNED mcdu;

   mcd.cbStruct = sizeof( mcd );
   mcd.dwControlID = controlID;
   mcd.cChannels = 1;
   mcd.cMultipleItems = 0;
   mcd.cbDetails = sizeof( mcdu );
   mcd.paDetails = &mcdu;

   MMRESULT res = mixerGetControlDetails( hMixer, &mcd, MIXER_GETCONTROLDETAILSF_VALUE );
   if ( res == MMSYSERR_NOERROR )
   {
      vol = ( ( MIXERCONTROLDETAILS_UNSIGNED * ) ( mcd.paDetails ) ) ->dwValue;
      return true;
   }
   return false;
}
void VKMixer::SetVolume( unsigned int controlID, DWORD vol )      // use zero to drive mute if present
{
   if ( !hMixer )
      return ;

   MIXERCONTROLDETAILS mcd;
   MIXERCONTROLDETAILS_UNSIGNED mcdu;

   mcd.cbStruct = sizeof( mcd );
   mcd.dwControlID = controlID;
   mcd.cChannels = 1;
   mcd.cMultipleItems = 0;
   mcd.cbDetails = sizeof( mcdu );
   mcd.paDetails = &mcdu;
   ( ( MIXERCONTROLDETAILS_UNSIGNED * ) ( mcd.paDetails ) ) ->dwValue = vol;

   mixerSetControlDetails( hMixer, &mcd, MIXER_GETCONTROLDETAILSF_VALUE );
}
void VKMixer::SetBool( unsigned int controlID, bool setting )
{
   if ( !hMixer )
      return ;

   MIXERCONTROLDETAILS mcd;
   MIXERCONTROLDETAILS_BOOLEAN mcdu;

   mcd.cbStruct = sizeof( mcd );
   mcd.dwControlID = controlID;
   mcd.cChannels = 1;
   mcd.cMultipleItems = 0;
   mcd.cbDetails = sizeof( mcdu );
   mcd.paDetails = &mcdu;
   ( ( MIXERCONTROLDETAILS_BOOLEAN * ) ( mcd.paDetails ) ) ->fValue = setting;

   mixerSetControlDetails( hMixer, &mcd, MIXER_GETCONTROLDETAILSF_VALUE );
}
bool VKMixer::GetBool( unsigned int controlID )
{
   if ( !hMixer )
      return 0;

   MIXERCONTROLDETAILS mcd;
   MIXERCONTROLDETAILS_BOOLEAN mcdu;

   mcd.cbStruct = sizeof( mcd );
   mcd.dwControlID = controlID;
   mcd.cChannels = 1;
   mcd.cMultipleItems = 0;
   mcd.cbDetails = sizeof( mcdu );
   mcd.paDetails = &mcdu;

   MMRESULT res = mixerGetControlDetails( hMixer, &mcd, MIXER_GETCONTROLDETAILSF_VALUE );
   if ( res == MMSYSERR_NOERROR )
      return ( ( MIXERCONTROLDETAILS_BOOLEAN * ) ( mcd.paDetails ) ) ->fValue;
   return false;
}
//---------------------------------------------------------------------------
#ifndef MUXEXAMPLE
// return the current mux input number and the name string
bool VKMixer::GetMuxInput( DWORD &muxId, std::string &sInputName )
{
   HMIXER mix = hMixer;
   bool retval = false;
   MMRESULT error;
   MIXERLINE mxl;
   mxl.cbStruct = sizeof( mxl );
   MIXERLINECONTROLS controls;
   MIXERCONTROL * control;
   MIXERCONTROLDETAILS cd;
   HRESULT hr;

   memset( &mxl, 0, sizeof( mxl ) );
   mxl.cbStruct = sizeof( mxl );

   mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
   hr = mixerGetLineInfo( ( HMIXEROBJ ) mix, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE );
   hr = mixerGetLineInfo( ( HMIXEROBJ ) mix, &mxl, MIXER_GETLINEINFOF_DESTINATION );

   if ( hr == MMSYSERR_NOERROR )
   {
      // should always be the first, but should iterate to make sure
      //for (int dest=0; dest<mxl.dwDestination; dest++)
      {
         controls.cbStruct = sizeof( MIXERLINECONTROLS );
         controls.dwLineID = mxl.dwLineID;  // from MIXERLINE
         controls.cControls = mxl.cControls;
         controls.cbmxctrl = sizeof( MIXERCONTROL );

         control = ( MIXERCONTROL * ) LocalAlloc( LPTR, sizeof( MIXERCONTROL ) * controls.cControls );
         controls.pamxctrl = control;

         controls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUX;

         error = mixerGetLineControls( ( HMIXEROBJ ) mix, &controls, MIXER_GETLINECONTROLSF_ONEBYTYPE );
         if ( MMSYSERR_NOERROR == error )
         {
            for ( unsigned int c = 0; c < controls.cControls; c++ )  // c should equal 0 (1control) but you never know
            {
               // MUX - MULTIPLE SELECT
               if ( MIXERCONTROL_CONTROLTYPE_MUX == ( MIXERCONTROL_CONTROLTYPE_MUX & control[ c ].dwControlType ) )
               {
                  cd.cbStruct = sizeof( MIXERCONTROLDETAILS );
                  cd.dwControlID = control->dwControlID;
                  cd.cChannels = 1;
                  cd.cMultipleItems = control->cMultipleItems;

                  //cd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
                  cd.cbDetails = sizeof( MIXERCONTROLDETAILS_LISTTEXT );

                  MIXERCONTROLDETAILS_BOOLEAN * lv = ( MIXERCONTROLDETAILS_BOOLEAN* ) LocalAlloc ( LPTR, cd.cChannels * cd.cMultipleItems * cd.cbDetails );
                  MIXERCONTROLDETAILS_LISTTEXT * lt = ( MIXERCONTROLDETAILS_LISTTEXT* ) LocalAlloc ( LPTR, cd.cChannels * cd.cMultipleItems * cd.cbDetails );
                  if ( NULL != lv )
                  {
                     // get the text names
                     cd.paDetails = lt;
                     error = mixerGetControlDetails( ( HMIXEROBJ ) mix, &cd, MIXER_GETCONTROLDETAILSF_LISTTEXT );
                     // get the state values
                     cd.paDetails = lv;
                     error = mixerGetControlDetails( ( HMIXEROBJ ) mix, &cd, MIXER_GETCONTROLDETAILSF_VALUE );
                     // find out which mux input is selected
                     for ( unsigned int i = 0; i < cd.cMultipleItems; i++ )
                     {
                        //String temp;
                        //temp.Format("%-30s [%c]\n", lt[i].szName, lv[i].fValue?'X':' ');
                        //OutputDebugString(temp.c_str());
                        if ( lv[ i ].fValue )
                        {
                           retval = true;
                           muxId = lt[ i ].dwParam1;
                           sInputName = lt[ i ].szName;
                        }
                     }

                     LocalFree( lv );
                     LocalFree( lt );
                  }
               }
            }
         }
      }
   }

   return retval;
}

// Set the active Mux input select and return previous selection
int VKMixer::SetMuxInput( std::string sInputName )
{
   HMIXER mix = hMixer;
   int retval = -1;
   MMRESULT error;
   MIXERLINE mxl;
   mxl.cbStruct = sizeof( mxl );
   MIXERLINECONTROLS controls;
   MIXERCONTROL * control;
   MIXERCONTROLDETAILS cd;
   HRESULT hr;

   memset( &mxl, 0, sizeof( mxl ) );
   mxl.cbStruct = sizeof( mxl );

   mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
   hr = mixerGetLineInfo( ( HMIXEROBJ ) mix, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE );
   hr = mixerGetLineInfo( ( HMIXEROBJ ) mix, &mxl, MIXER_GETLINEINFOF_DESTINATION );

   if ( hr == MMSYSERR_NOERROR )
   {
      // should always be the first, but should iterate to make sure
      //for (int dest=0; dest<mxl.dwDestination; dest++)
      {
         controls.cbStruct = sizeof( MIXERLINECONTROLS );
         controls.dwLineID = mxl.dwLineID;  // from MIXERLINE
         controls.cControls = mxl.cControls;
         controls.cbmxctrl = sizeof( MIXERCONTROL );

         control = ( MIXERCONTROL * ) LocalAlloc( LPTR, sizeof( MIXERCONTROL ) * controls.cControls );
         controls.pamxctrl = control;

         controls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUX;

         error = mixerGetLineControls( ( HMIXEROBJ ) mix, &controls, MIXER_GETLINECONTROLSF_ONEBYTYPE );
         if ( MMSYSERR_NOERROR == error )
         {
            //            if (target > control->cMultipleItems)
            //               return -1; // value out of range

            for ( unsigned int c = 0; c < controls.cControls; c++ )  // c should equal 0 (1 control) but you never know
            {
               // MUX - MULTIPLE SELECT

               if ( MIXERCONTROL_CONTROLTYPE_MUX == ( MIXERCONTROL_CONTROLTYPE_MUX & control[ c ].dwControlType ) )
               {
                  cd.cbStruct = sizeof( MIXERCONTROLDETAILS );
                  cd.dwControlID = control->dwControlID;
                  cd.cChannels = 1;
                  cd.cMultipleItems = control->cMultipleItems;

                  //                  cd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
                  cd.cbDetails = sizeof( MIXERCONTROLDETAILS_LISTTEXT );

                  MIXERCONTROLDETAILS_BOOLEAN * lv = ( MIXERCONTROLDETAILS_BOOLEAN* ) LocalAlloc ( LPTR, cd.cChannels * cd.cMultipleItems * cd.cbDetails );
                  MIXERCONTROLDETAILS_LISTTEXT * lt = ( MIXERCONTROLDETAILS_LISTTEXT* ) LocalAlloc ( LPTR, cd.cChannels * cd.cMultipleItems * cd.cbDetails );

                  if ( NULL != lv )
                  {
                     // get the text names
                     cd.paDetails = lt;
                     error = mixerGetControlDetails( ( HMIXEROBJ ) mix, &cd, MIXER_GETCONTROLDETAILSF_LISTTEXT );

                     // get the state values
                     cd.paDetails = lv;
                     error = mixerGetControlDetails( ( HMIXEROBJ ) mix, &cd, MIXER_GETCONTROLDETAILSF_VALUE );

                     // find out which mux input is selected
                     for ( unsigned int i = 0; i < cd.cMultipleItems; i++ )
                     {
                        if ( lv[ i ].fValue )
                        {
                           // if the select flag is on turn it off
                           retval = i; // save it for return value
                           lv[ i ].fValue = 0;
                        }
                        if ( stricmp( lt[ i ].szName, sInputName.c_str() ) == 0 )
                        {
                           lv[ i ].fValue = 1;
                        }
                     }
                     // set the select flag for the target item
                     error = mixerSetControlDetails( ( HMIXEROBJ ) mix, &cd, MIXER_SETCONTROLDETAILSF_VALUE );

                     LocalFree( lt );
                     LocalFree( lv );
                  }
               }
            }
         }
      }
   }

   return retval; // return previously selected item
}
#endif 
//---------------------------------------------------------------------------
/*
   May be worth looking for MIXERCONTROL_CONTROLTYPE_MUTE to do settings
   except for volumes
*/
bool VKMixer::GetMicRecVolume( DWORD &micVol )
{
   std::string s;
   DWORD mic;
   DWORD micVolCtl;
   if ( GetMuxInput( mic, s ) && mic != 0 )
   {
      if ( FindLineControl( MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
                            mic,
                            MIXERCONTROL_CONTROLTYPE_VOLUME, micVolCtl ) )
      {
         return GetVolume( micVolCtl, micVol );
      }
   }
   else
   {
      DWORD micVolCtl;
      if ( Find( MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
                 MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,
                 MIXERCONTROL_CONTROLTYPE_VOLUME, micVolCtl ) )
      {
         return GetVolume( micVolCtl, micVol );
      }

   }
   return false;
}
void VKMixer::SetMicRecVolume( DWORD vol )      // use zero to drive mute if present
{
   std::string s;
   DWORD mic;
   if ( GetMuxInput( mic, s ) && mic != 0 )
   {
      DWORD micVol;
      if ( FindLineControl( MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
                            mic,
                            MIXERCONTROL_CONTROLTYPE_VOLUME, micVol ) )
         if ( s.size() )
         {
            SetVolume( micVol, vol );
         }
   }
   else
   {
      DWORD micVol;
      if ( Find( MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
                 MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,
                 MIXERCONTROL_CONTROLTYPE_VOLUME, micVol ) )
         SetVolume( micVol, vol );

   }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool VKMixer::GetMasterVolume( DWORD &vol )
{
   DWORD mv;
   if ( Find( MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,
              0,
              MIXERCONTROL_CONTROLTYPE_VOLUME, mv ) )
   {
      return GetVolume( mv, vol );
   }
   return false;
}
void VKMixer::SetMasterVolume( DWORD vol )
{
   DWORD mv;
   if ( Find( MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,
              0,
              MIXERCONTROL_CONTROLTYPE_VOLUME, mv ) )
   {
      SetVolume( mv, vol );
   }
}

//---------------------------------------------------------------------------

//==============================================================================
/*
The MIXERCONTROLDETAILS structure refers to control-detail structures,
retrieving or setting state information of an audio mixer control.
All members of this structure must be initialized before calling the
mixerGetControlDetails and mixerSetControlDetails functions.

typedef struct {
    DWORD cbStruct;
    DWORD dwControlID;
    DWORD cChannels;
    union {
        HWND  hwndOwner;
        DWORD cMultipleItems;
    };
    DWORD  cbDetails;
    LPVOID paDetails;
} MIXERCONTROLDETAILS;
 
 
Members
 
cbStruct
 
Size, in bytes, of the MIXERCONTROLDETAILS structure. The size must be large
enough to contain the base MIXERCONTROLDETAILS structure. When
mixerGetControlDetails returns, this member contains the actual size of the
information returned. The returned information will not exceed the requested
size, nor will it be smaller than the base MIXERCONTROLDETAILS structure.
 
dwControlID
 
Control identifier on which to get or set properties.
 
cChannels
 
Number of channels on which to get or set control properties.
The following values are defined:
 
0
 
Use this value when the control is a MIXERCONTROL_CONTROLTYPE_CUSTOM control.
 
1
 
Use this value when the control is a MIXERCONTROL_CONTROLF_UNIFORM control
or when an application needs to get and set all channels as if they were uniform.
 
MIXERLINE.cChannels
 
Use this value when the properties for the control are expected on all
channels for a line.
 
An application cannot specify a value that falls between 1 and the number
of channels for the audio line. For example, specifying 2 or 3 for a
four-channel line is not valid. This member cannot be 0 for noncustom control types.
 
hwndOwner
 
Handle of the window that owns a custom dialog box for a mixer control.
This member is used when the MIXER_SETCONTROLDETAILSF_CUSTOM flag is
specified in the mixerSetControlDetails function.
 
cMultipleItems
 
Number of multiple items per channel on which to get or set properties.
The following values are defined:
 
0
 
Use this value for all controls except for a MIXERCONTROL_CONTROLF_MULTIPLE
or a MIXERCONTROL_CONTROLTYPE_CUSTOM control.
 
MIXERCONTROL cMultipleItems member.
 
Use this value when the control class is MIXERCONTROL_CONTROLF_MULTIPLE.
 
MIXERCONTROLDETAILS hwndOwner member.
 
Use this value when the control is a MIXERCONTROL_CONTROLTYPE_CUSTOM control
and the MIXER_SETCONTROLDETAILSF_CUSTOM flag is specified for the
mixerSetControlDetails function. In this case, the hwndOwner member overlaps
with cMultipleItems, providing the value of the window handle.
 
When using a MIXERCONTROL_CONTROLTYPE_CUSTOM control without the
MIXERCONTROL_CONTROLTYPE_CUSTOM flag, specify zero for this member.
An application cannot specify any value other than the value specified
in the cMultipleItems member of the MIXERCONTROL structure for a
MIXERCONTROL_CONTROLF_MULTIPLE control.
 
cbDetails
 
Size, in bytes, of one of the following details structures being used:
 
MIXERCONTROLDETAILS_BOOLEAN
 
Boolean value for an audio line control.
 
MIXERCONTROLDETAILS_LISTTEXT
 
List text buffer for an audio line control. For information about the
appropriate details structure for a specific control, see Control Types.
 
MIXERCONTROLDETAILS_SIGNED
 
Signed value for an audio line control.
 
MIXERCONTROLDETAILS_UNSIGNED
 
Unsigned value for an audio line control.
 
If the control is a MIXERCONTROL_CONTROLTYPE_CUSTOM control, this member must
be equal to the cbCustomData member of the MIXERCONTROL structure.
 
paDetails
 
Address of an array of one or more structures in which properties for the
specified control are retrieved or set. For MIXERCONTROL_CONTROLF_MULTIPLE
controls, the size of this buffer should be the product of the cChannels,
cMultipleItems and cbDetails members of the MIXERCONTROLDETAILS structure.
For controls other than MIXERCONTROL_CONTROLF_MULTIPLE types, the size of this
buffer is the product of the cChannels and cbDetails members of the
MIXERCONTROLDETAILS structure.
 
For controls that are MIXERCONTROL_CONTROLF_MULTIPLE types, the array can be
treated as a two-dimensional array that is channel major. That is, all
multiple items for the left channel are given, then all multiple items
for the right channel, and so on.
For controls other than MIXERCONTROL_CONTROLF_MULTIPLE types, each element
index is equivalent to the zero-based channel that it affects. That is,
paDetails[0] is for the left channel and paDetails[1] is for the right channel.
 
If the control is a MIXERCONTROL_CONTROLTYPE_CUSTOM control, this member must
point to a buffer that is at least large enough to contain the size, in bytes,
specified by the cbCustomData member of the MIXERCONTROL structure.
 
 
 
See Also
 
MIXERCONTROL, mixerGetControlDetails, mixerSetControlDetails
*/
MixerControlDetails::MixerControlDetails( HMIXEROBJ h, MIXERCONTROL *m, int c ) :
      mc( m ), cChannels( c ), hMixer( h )
{
   memset( &mcd, 0, sizeof( MIXERCONTROLDETAILS ) );
   mcd.cbStruct = sizeof( mcd );
   mcd.dwControlID = mc->dwControlID;
   mcd.cChannels = ( mc->fdwControl & MIXERCONTROL_CONTROLF_UNIFORM ) ? 1 : cChannels;
   mcd.cMultipleItems = ( mc->fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE ) ? mc->cMultipleItems : 0;

   //Size, in bytes, of one of the following details structures being used:
   //MIXERCONTROLDETAILS_BOOLEAN
   //Boolean value for an audio line control.
   //MIXERCONTROLDETAILS_LISTTEXT
   // List text buffer for an audio line control. For information about the
   // appropriate details structure for a specific control, see Control Types.
   //MIXERCONTROLDETAILS_SIGNED
   //Signed value for an audio line control.
   //MIXERCONTROLDETAILS_UNSIGNED
   //Unsigned value for an audio line control.
   //If the control is a MIXERCONTROL_CONTROLTYPE_CUSTOM control, this member
   // must be equal to the cbCustomData member of the MIXERCONTROL structure.

   DWORD CT_Units = mc->dwControlType & MIXERCONTROL_CT_UNITS_MASK;
   switch ( CT_Units )
   {
      case MIXERCONTROL_CT_UNITS_CUSTOM:     //        0x00000000L
         mcd.cbDetails = mc->Metrics.cbCustomData;
         break;

      case MIXERCONTROL_CT_UNITS_BOOLEAN:     //       0x00010000L
         mcd.cbDetails = sizeof( MIXERCONTROLDETAILS_BOOLEAN );
         break;

      case MIXERCONTROL_CT_UNITS_SIGNED:     //        0x00020000L
         mcd.cbDetails = sizeof( MIXERCONTROLDETAILS_SIGNED );
         break;

      case MIXERCONTROL_CT_UNITS_UNSIGNED:     //      0x00030000L
         mcd.cbDetails = sizeof( MIXERCONTROLDETAILS_UNSIGNED );
         break;

      case MIXERCONTROL_CT_UNITS_DECIBELS:     //      0x00040000L /* in 10ths */
         mcd.cbDetails = sizeof( MIXERCONTROLDETAILS_SIGNED );
         break;

      case MIXERCONTROL_CT_UNITS_PERCENT:     //       0x00050000L /* in 10ths */
         mcd.cbDetails = sizeof( MIXERCONTROLDETAILS_UNSIGNED );
         break;

      default:
         mcd.cbDetails = 0;
         break;
   }
   /*
   mc->fdwControl

   Status and support flags for the audio line control. The following values are defined:

   MIXERCONTROL_CONTROLF_DISABLED
   The control is disabled, perhaps due to other settings for the mixer hardware, and cannot be used. An application can read current settings from a disabled control, but it cannot apply settings.

   MIXERCONTROL_CONTROLF_MULTIPLE
   The control has two or more settings per channel. An equalizer, for example, requires this flag because each frequency band can be set to a different value. An equalizer that affects both channels of a stereo line in a uniform fashion will also specify the MIXERCONTROL_CONTROLF_UNIFORM flag.

   MIXERCONTROL_CONTROLF_UNIFORM
   The control acts on all channels of a multichannel line in a uniform fashion. For example, a control that mutes both channels of a stereo line would set this flag. Most MIXERCONTROL_CONTROLTYPE_MUX and MIXERCONTROL_CONTROLTYPE_MIXER controls also specify the MIXERCONTROL_CONTROLF_UNIFORM flag.
   */

   // Pointer to an array of one or more structures in which properties for the
   // specified control are retrieved or set. For MIXERCONTROL_CONTROLF_MULTIPLE
   // controls, the size of this buffer should be the product of the cChannels,
   // cMultipleItems and cbDetails members of the MIXERCONTROLDETAILS structure.
   // For controls other than MIXERCONTROL_CONTROLF_MULTIPLE types, the size of
   // this buffer is the product of the cChannels and cbDetails members of the MIXERCONTROLDETAILS structure.
   int detsize = mcd.cbDetails;

   if ( mc->fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
   {
      detsize *= mc->cMultipleItems;
   }
   detsize *= cChannels;
   if ( detsize )
      mcd.paDetails = ( LPVOID ) ( new char[ detsize ] );
   else
      mcd.paDetails = 0;
}
MixerControlDetails::~MixerControlDetails()
{
   delete [] mcd.paDetails;
}

void MixerControlDetails::SetCustomDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element
   // We dump as a string of bytes, as we don't know any better

   if ( mcd.paDetails )
   {
      bool dataOK = false;
      for ( TiXmlNode * g = e->FirstChild(); g; g = g->NextSibling() )
      {
         if ( TiXmlText * t = g->ToText() )
         {
            std::string data = t->Value();
            const char *d = data.c_str();
            unsigned char *dets = ( unsigned char * ) ( mcd.paDetails );
            for ( int i = 0; i < ndets; i++ )
            {
               const char *p = d;
               unsigned long ul = strtoul( d, &const_cast<char *>( p ), 0 );
               dets[ i ] = ( ul & 0xFF );
               d = p;
            }
            dataOK = true;
            break;
         }
      }
      if ( dataOK )
         mixerSetControlDetails( hMixer, &mcd, MIXER_OBJECTF_HMIXER );
      else
         doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszControlErr, 0, 0 );
   }
}
void MixerControlDetails::SetBoolDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element
   bool dataOK = false;
   for ( TiXmlNode * g = e->FirstChild(); g; g = g->NextSibling() )
   {
      if ( TiXmlText * t = g->ToText() )
      {
         std::string data = t->Value();
         const char *d = data.c_str();
         unsigned char *dets = ( unsigned char * ) ( mcd.paDetails );
         for ( int i = 0; i < ndets; i++ )
         {
            const char *p = d;
            unsigned long ul = strtoul( d, &const_cast<char *>( p ), 0 );
            dets[ i ] = ul ? true : false;
            d = p;
         }
         dataOK = true;
         break;
      }
   }
   if ( dataOK )
      mixerSetControlDetails( hMixer, &mcd, MIXER_OBJECTF_HMIXER );
   else
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszControlErr, 0, 0 );

}
void MixerControlDetails::SetSignedDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element

   bool dataOK = false;
   for ( TiXmlNode * g = e->FirstChild(); g; g = g->NextSibling() )
   {
      if ( TiXmlText * t = g->ToText() )
      {
         std::string data = t->Value();
         const char *d = data.c_str();
         int *dets = ( int * ) ( mcd.paDetails );
         for ( int i = 0; i < ndets; i++ )
         {
            const char *p = d;
            unsigned long ul = strtoul( d, &const_cast<char *>( p ), 0 );
            dets[ i ] = ( int ) ul;
            d = p;
         }
         dataOK = true;
         break;
      }
   }
   if ( dataOK )
      mixerSetControlDetails( hMixer, &mcd, MIXER_OBJECTF_HMIXER );
   else
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszControlErr, 0, 0 );

}
void MixerControlDetails::SetUnsignedDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element

   bool dataOK = false;
   for ( TiXmlNode * g = e->FirstChild(); g; g = g->NextSibling() )
   {
      if ( TiXmlText * t = g->ToText() )
      {
         std::string data = t->Value();
         const char *d = data.c_str();
         unsigned int *dets = ( unsigned int * ) ( mcd.paDetails );
         for ( int i = 0; i < ndets; i++ )
         {
            const char *p = d;
            unsigned long ul = strtoul( d, &const_cast<char *>( p ), 0 );
            dets[ i ] = ul;
            d = p;
         }
         dataOK = true;
         break;
      }
   }
   if ( dataOK )
      mixerSetControlDetails( hMixer, &mcd, MIXER_OBJECTF_HMIXER );
   else
      doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszControlErr, 0, 0 );

}
void MixerControlDetails::SetControlDetails( TiXmlElement *e )
{
   if ( mcd.paDetails )
   {
      // now build the MIXERCONTROLDETAILS from the control
      MMRESULT res = mixerGetControlDetails( hMixer, &mcd, MIXER_GETCONTROLDETAILSF_VALUE );
      if ( res != MMSYSERR_NOERROR )      // 11 is invalid param - e.g. for mute... (UNIFORM)
      {
         doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszParamErr, res );
         return ;
      }

      int ndets = 1;

      if ( mc->fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
      {
         ndets *= mcd.cMultipleItems;
      }
      ndets *= mcd.cChannels;

      DWORD CT_Units = mc->dwControlType & MIXERCONTROL_CT_UNITS_MASK;
      switch ( CT_Units )
      {
         case MIXERCONTROL_CT_UNITS_CUSTOM:     //        0x00000000L
            SetCustomDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_BOOLEAN:     //       0x00010000L
            SetBoolDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_SIGNED:     //        0x00020000L
            SetSignedDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_UNSIGNED:     //      0x00030000L
            SetUnsignedDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_DECIBELS:     //      0x00040000L /* in 10ths */
            SetSignedDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_PERCENT:     //       0x00050000L /* in 10ths */
            SetUnsignedDetails( ndets, e );
            break;

         default:
            break;
      }
   }
}

void MixerControlDetails::DumpCustomDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element
   // We dump as a string of bytes, as we don't know any better

   std::string data;
   unsigned char *dets = ( unsigned char * ) ( mcd.paDetails );
   if ( dets )
   {
      for ( unsigned int i = 0; i < ndets * mcd.cbDetails; i++ )
      {
         data += hexVal( dets[ i ] );
         if ( i < ndets * mcd.cbDetails - 1 )
            data += " ";
      }
      TiXmlText tt( data );
      e->InsertEndChild( tt );
   }
}
void MixerControlDetails::DumpBoolDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element

   std::string data;
   unsigned char *dets = ( unsigned char * ) ( mcd.paDetails );
   for ( int i = 0; i < ndets; i++ )
   {
      data += boolVal( dets[ i ] );
      if ( i < ndets - 1 )
         data += " ";
   }
   TiXmlText tt( data );
   e->InsertEndChild( tt );
}
void MixerControlDetails::DumpSignedDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element

   std::string data;
   int *dets = ( int * ) ( mcd.paDetails );
   for ( int i = 0; i < ndets; i++ )
   {
      data += intVal( dets[ i ] );
      if ( i < ndets - 1 )
         data += " ";
   }
   TiXmlText tt( data );
   e->InsertEndChild( tt );
}
void MixerControlDetails::DumpUnsignedDetails( int ndets, TiXmlElement *e )
{
   // mcd.cbDetails is the number of bytes per element

   std::string data;
   unsigned int *dets = ( unsigned int * ) ( mcd.paDetails );
   for ( int i = 0; i < ndets; i++ )
   {
      data += uintVal( dets[ i ] );
      if ( i < ndets - 1 )
         data += " ";
   }
   TiXmlText tt( data );
   e->InsertEndChild( tt );
}

void MixerControlDetails::GetControlDetails( TiXmlElement *e )
{
   if ( mcd.paDetails )
   {
      // now build the MIXERCONTROLDETAILS from the control
      MMRESULT res = mixerGetControlDetails( hMixer, &mcd, MIXER_GETCONTROLDETAILSF_VALUE );
      if ( res != MMSYSERR_NOERROR )      // 11 is invalid param - e.g. for mute... (UNIFORM)
      {
         doMsgBox( 0, MB_OK | MB_ICONEXCLAMATION, gszParamErr, res );
         return ;
      }

      int ndets = 1;

      if ( mc->fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
      {
         ndets *= mcd.cMultipleItems;
      }
      ndets *= mcd.cChannels;

      DWORD CT_Units = mc->dwControlType & MIXERCONTROL_CT_UNITS_MASK;
      switch ( CT_Units )
      {
         case MIXERCONTROL_CT_UNITS_CUSTOM:     //        0x00000000L
            DumpCustomDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_BOOLEAN:     //       0x00010000L
            DumpBoolDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_SIGNED:     //        0x00020000L
            DumpSignedDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_UNSIGNED:     //      0x00030000L
            DumpUnsignedDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_DECIBELS:     //      0x00040000L /* in 10ths */
            DumpSignedDetails( ndets, e );
            break;

         case MIXERCONTROL_CT_UNITS_PERCENT:     //       0x00050000L /* in 10ths */
            DumpUnsignedDetails( ndets, e );
            break;

         default:
            break;
      }
   }
}
//==============================================================================
void VKMixerControls::SaveMixerSettings( TiXmlElement &xe )
{
   if ( mixerLineControls.pamxctrl )
   {
      for ( unsigned int i = 0; i < mixerLineControls.cControls; i++ )
      {
         TiXmlElement controlNode( "Control" );
         controlNode.SetAttribute( "Name", mixerLineControls.pamxctrl[ i ].szName );
         controlNode.SetAttribute( "dwControlID", mixerLineControls.pamxctrl[ i ].dwControlID );
         controlNode.SetAttribute( "dwControlType", hexVal( mixerLineControls.pamxctrl[ i ].dwControlType ) );
         controlNode.SetAttribute( "fdwControl", hexVal( mixerLineControls.pamxctrl[ i ].fdwControl ) );
         controlNode.SetAttribute( "cMultipleItems", mixerLineControls.pamxctrl[ i ].cMultipleItems );
         // these two are just convenient parts of a union
         controlNode.SetAttribute( "dwMinimum", hexVal( mixerLineControls.pamxctrl[ i ].Bounds.dwMinimum ) );
         controlNode.SetAttribute( "dwMaximum", hexVal( mixerLineControls.pamxctrl[ i ].Bounds.dwMaximum ) );

         controlNode.SetAttribute( "cSteps", mixerLineControls.pamxctrl[ i ].Metrics.cSteps );

         MixerControlDetails mcd( hMixer, &mixerLineControls.pamxctrl[ i ], line->lineinfo.cChannels );
         mcd.GetControlDetails( &controlNode );

         xe.InsertEndChild( controlNode );
      }
   }
}
void VKSourceLine::SaveMixerSettings( TiXmlElement &xe )
{
   // set up this line

   TiXmlElement lineNode( "SourceLine" );
   lineNode.SetAttribute( "Name", lineinfo.szName );
   lineNode.SetAttribute( "dwDestination", lineinfo.dwDestination );
   lineNode.SetAttribute( "dwSource", lineinfo.dwSource );
   lineNode.SetAttribute( "dwLineID", hexVal( lineinfo.dwLineID ) );
   lineNode.SetAttribute( "fdwLine", hexVal( lineinfo.fdwLine ) );
   lineNode.SetAttribute( "dwComponentType", hexVal( lineinfo.dwComponentType ) );
   lineNode.SetAttribute( "cChannels", lineinfo.cChannels );
   lineNode.SetAttribute( "cConnections", lineinfo.cConnections );
   lineNode.SetAttribute( "cControls", lineinfo.cControls );

   controls.SaveMixerSettings( lineNode );

   xe.InsertEndChild( lineNode );
}
void VKDestinationLine::SaveMixerSettings( TiXmlElement &xe )
{
   TiXmlElement lineNode( "DestinationLine" );
   lineNode.SetAttribute( "Name", lineinfo.szName );
   lineNode.SetAttribute( "dwDestination", lineinfo.dwDestination );
   lineNode.SetAttribute( "dwSource", lineinfo.dwSource );
   lineNode.SetAttribute( "dwLineID", hexVal( lineinfo.dwLineID ) );
   lineNode.SetAttribute( "fdwLine", hexVal( lineinfo.fdwLine ) );
   lineNode.SetAttribute( "dwComponentType", hexVal( lineinfo.dwComponentType ) );
   lineNode.SetAttribute( "cChannels", lineinfo.cChannels );
   lineNode.SetAttribute( "cConnections", lineinfo.cConnections );
   lineNode.SetAttribute( "cControls", lineinfo.cControls );

   controls.SaveMixerSettings( lineNode );

   for ( unsigned int i = 0; i < sourcelines.size(); i++ )
   {
      sourcelines[ i ] ->SaveMixerSettings( lineNode );
   }

   xe.InsertEndChild( lineNode );
}
void VKMixer::SaveMixerSettings( TiXmlDocument &doc )
{
   TiXmlElement rootNode( "MixerSettings" );
   rootNode.SetAttribute( "Name", getComponentType() );


   for ( unsigned int i = 0; i < destlines.size(); i++ )
   {
      destlines[ i ] ->SaveMixerSettings( rootNode );
   }
   doc.InsertEndChild( rootNode );
}
bool VKMixer::LoadMixerSettings( TiXmlDocument &doc, std::string &err )
{
   if ( !hMixer )
   {
      err = "No current mixer";
      return false;
   }
   // have to walk the DOM, and apply saved settings
   // to the relevant controls
   TiXmlElement *root = doc.RootElement();
   if ( !root )
   {
	  err = "No XML root element (does file exist?)";
      return false;
   }
   if ( !checkElementName( root, "MixerSettings" ) )
   {
      err = "Invalid XML root element";
      return false;
   }
   const char * mixname = root->Attribute( "Name" );
   if ( !mixname )
   {
      err = "No XML root element name";
      return false;
   }
   if ( stricmp( mixname, getComponentType() ) )
   {
      err = "File not for current mixer type";
      return false;
   }
   for ( TiXmlElement * e = root->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      // declaration, destination lines
      if ( checkElementName( e, "DestinationLine" ) )
      {
         int destinationid = -1;
         if ( GetIntAttribute( e, "dwDestination", destinationid ) != TIXML_SUCCESS )
            continue;
         int sourceid = -1;
         int dcChannels = -1;
         if ( GetIntAttribute( e, "cChannels", dcChannels ) != TIXML_SUCCESS )
            continue;
         for ( TiXmlElement * f = e->FirstChildElement(); f; f = f->NextSiblingElement() )
         {
            //destination line controls, source lines
            if ( checkElementName( f, "SourceLine" ) )
            {
               int cChannels = -1;
               if ( GetIntAttribute( f, "dwSource", sourceid ) != TIXML_SUCCESS )
				  continue;
               if ( GetIntAttribute( f, "cChannels", cChannels ) != TIXML_SUCCESS )
                  continue;
               for ( TiXmlElement * g = f->FirstChildElement(); g; g = g->NextSiblingElement() )
               {
                  if ( checkElementName( g, "Control" ) )
                  {
                     int controlid = -1;
                     if ( GetIntAttribute( g, "dwControlID", controlid ) != TIXML_SUCCESS )
                        continue;
                     //source line controls
                     MIXERCONTROL *mc = FindId( destinationid, sourceid, controlid );
                     if ( !mc )
                        continue;
                     MixerControlDetails mcd( hMixer, mc, cChannels );
                     mcd.SetControlDetails( g );
                  }
               }
            }
            else
               if ( checkElementName( f, "Control" ) )
               {
                  // destination control
                  int controlid = -1;
                  if ( GetIntAttribute( f, "dwControlID", controlid ) != TIXML_SUCCESS )
                     continue;
                  //source line controls
                  MIXERCONTROL *mc = FindId( destinationid, sourceid, controlid );
                  if ( !mc )
                     continue;
                  MixerControlDetails mcd( hMixer, mc, dcChannels );
                  mcd.SetControlDetails( f );
               }
         }
      }
   }
   err = "Mixer load successful";
   return true;

}
//==============================================================================
void VKMixer::SaveMixerSettings( const std::string &fname )
{
   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument mixdoc;

   TiXmlDeclaration declaration( "1.0", "ISO-8859-1", "no" );
   mixdoc.InsertEndChild( declaration );
   TiXmlComment comment;
   comment.SetValue( "Produced by G0GJV mixer utility V1.0" );
   mixdoc.InsertEndChild( comment );

   SaveMixerSettings( mixdoc );

   mixdoc.SaveFile( fname );
}
bool VKMixer::LoadMixerSettings( const std::string &fname, std::string &err )
{
   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument mixdoc;
   mixdoc.LoadFile( fname );

   return LoadMixerSettings( mixdoc, err );

}
void _DLL_FUNCTION SaveMixerSettings( const char *fname )
{
   const char * defname = "mixersettings.xml";
   if ( !fname )
      fname = defname;

   // Call this whenever we close(?) or on an explicit
   // save request from the user.

   VKMixer *inMix = VKMixer::GetInputVKMixer();
   if ( inMix )
   {
      inMix ->SaveMixerSettings( std::string( fname ) );
   }
   VKMixer *outMix = VKMixer::GetOutputVKMixer();
   if ( outMix )
   {
      outMix ->SaveMixerSettings( std::string( fname ) );
   }

   // save all the settings for all the MixerSets
}
bool _DLL_FUNCTION LoadMixerSettings( const char *fname, std::string &/*err*/ )
{
   const char * defname = "mixersettings.xml";
   if ( !fname )
      fname = defname;
   //   return VKMixer::GetVKMixer() ->LoadMixerSettings( std::string(fname), err );

   return false;
}

//---------------------------------------------------------------------------
std::vector<boost::shared_ptr<VKMixer> > VKMixer::getInputMixers(HWND callbackHwnd )
{
   if ( inputMixers.size() == 0 )
   {
	  int numMixers = ::waveInGetNumDevs();

	  for ( int i = 0; i < numMixers; i++ )
	  {
		 UINT mixerID;
		 MMRESULT res = mixerGetID((HMIXEROBJ)i, &mixerID, MIXER_OBJECTF_WAVEIN);
		 if (res == MMSYSERR_NOERROR)
		 {
			 VKMixer *mixer = new VKMixer();
			 mixer->inputOpen( mixerID, i, callbackHwnd );
			 boost::shared_ptr<VKMixer> mix = boost::shared_ptr<VKMixer>( mixer );
			 inputMixers.push_back( mix );
		 }
	  }
   }
   return inputMixers;
}
std::vector<boost::shared_ptr<VKMixer> > VKMixer::getOutputMixers(HWND callbackHwnd )
{
   if ( outputMixers.size() == 0 )
   {
	  int numMixers = ::waveOutGetNumDevs();

	  for ( int i = 0; i < numMixers; i++ )
	  {
		 UINT mixerID;
		 MMRESULT res = mixerGetID((HMIXEROBJ)i, &mixerID, MIXER_OBJECTF_WAVEOUT);
		 if (res == MMSYSERR_NOERROR)
		 {
			 VKMixer *mixer = new VKMixer();
			 mixer->outputOpen( mixerID, i, callbackHwnd );
			 boost::shared_ptr<VKMixer> mix = boost::shared_ptr<VKMixer>( mixer );
			 outputMixers.push_back( mix );
		 }
	  }
   }
   return outputMixers;
}
/*static*/ VKMixer *VKMixer::getInputMixer( const std::string &name )
{
   for ( std::vector<boost::shared_ptr<VKMixer> >::iterator i = inputMixers.begin(); i != inputMixers.end(); i++ )
   {
	  if ( ( *i ) ->getMixerName() == name )
	  {
		 return ( *i ).get();
	  }
   }
   return 0;
}
/*static*/ VKMixer *VKMixer::getOutputMixer( const std::string &name )
{
   for ( std::vector<boost::shared_ptr<VKMixer> >::iterator i = outputMixers.begin(); i != outputMixers.end(); i++ )
   {
	  if ( ( *i ) ->getMixerName() == name )
	  {
		 return ( *i ).get();
	  }
   }
   return 0;
}

/*static*/ VKMixer * VKMixer::setInputMixer( const std::string &name )
{
   if ( name.size() > 0 )
   {
	  inputMixer = getInputMixer( name );
   }
   return inputMixer;
}
/*static*/ VKMixer * VKMixer::setOutputMixer( const std::string &name )
{
   if ( name.size() > 0 )
   {
	  outputMixer = getOutputMixer( name );
   }
   return outputMixer;
}

void VKMixer::setInputMixerLine( const std::string &name )
{
   inputLine = name;
   // And we need to find that line and select it in the MUX
   SetMuxInput( name );
}
void VKMixer::setOutputMixerLine( const std::string &name )
{
   outputLine = name;
}

