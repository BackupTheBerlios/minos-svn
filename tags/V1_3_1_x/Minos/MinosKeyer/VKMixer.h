/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef VKMixerH
#define VKMixerH
#pragma pack(push, 1)

#if defined (__DLL__)
#define _DLL_FUNCTION __declspec(dllexport) FAR PASCAL
#define _DLL_CLASS _export
#else
#define _DLL_FUNCTION __declspec(dllimport) FAR PASCAL
#define _DLL_CLASS
#endif

class TiXmlDocument;
class TiXmlElement;
//==============================================================================
/*
Line assignments, elements
 
<LineSetting>
   <eLineTypes>
      <line/> (eLineTypes)       - repeat per line
      // set one mixer line to each line type
   </eLineTypes>
   <eMixerSets>
      <MixerSet>                 - repeat per mixer set
         <linesetting>           - repeat per line type
            <Volume/>            - NB stereo, two settings
            <Mute/>  (or asume zero volume implies mute if available)
         </linesetting>          - repeat per line type
      </MixerSet>
   </eMixerSets>
</LineSetting>
 
We need to match the sliders and meters to the use set (tab and line)
 
BUT we also need to be able to track/set as required in an intuitive way
 
We may also need to "know" that in some modes some things are muted?
Although allowing complicated setups is what allows funny things,
like sidetone recording?
*/ 
//==============================================================================
// Volume settings required in different situations.
// To be applied to the relevant MixerSet for the current
// situation

enum eMixerSets {emsUnloaded, emsPassThroughNoPTT, emsPassThroughPTT,
                 emsReplay, emsReplayPip, emsReplayT1, emsReplayT2,
                 emsVoiceRecord,
                 emsCWTransmit, emsCWPassThrough,
                 emsMaxMixerSet};
class _DLL_CLASS MixerSet
{
   public:
      MixerSet( bool MicOutMute, bool MasterMute )
            : MicOutMute( MicOutMute ), MasterMute( MasterMute )
      {}
      // we need a line setting, stereo volume + mute
      bool MicOutMute;
      bool MasterMute;
};

// All the line setting/type stuff should be in a differnt module.
// Mapping from VK functionality to mixer lines and controls
extern MixerSet * _DLL_FUNCTION GetMixerSets();
extern eMixerSets _DLL_FUNCTION GetCurrentMixerSet();
extern void _DLL_FUNCTION SetCurrentMixerSet( eMixerSets cms );
extern void _DLL_FUNCTION SetBaseMixerLevels();

//==============================================================================
class _DLL_CLASS MixerLineSetting
{
      // Control IDs for the various controls, and the route to them
      int destline;
      int srcline;
      int volumeControl;
      int muteControl;
};
//==============================================================================
class MixerControlDetails
{
      HMIXEROBJ hMixer;
      MIXERCONTROL *mc;
      MIXERCONTROLDETAILS mcd;
      int cChannels;
      void DumpCustomDetails( int ndets, TiXmlElement *e );
      void DumpBoolDetails( int ndets, TiXmlElement *e );
      void DumpSignedDetails( int ndets, TiXmlElement *e );
      void DumpUnsignedDetails( int ndets, TiXmlElement *e );

      void SetCustomDetails( int ndets, TiXmlElement *e );
      void SetBoolDetails( int ndets, TiXmlElement *e );
      void SetSignedDetails( int ndets, TiXmlElement *e );
      void SetUnsignedDetails( int ndets, TiXmlElement *e );
   public:
      MixerControlDetails( HMIXEROBJ hMixer, MIXERCONTROL *m, int cChannels );
      ~MixerControlDetails();
      void SetControlDetails( TiXmlElement *e );
      void GetControlDetails( TiXmlElement *e );
};
//==============================================================================
class _DLL_CLASS VKMixerLine;
class _DLL_CLASS VKMixerControls
{
      VKMixerLine *line;
      HMIXEROBJ hMixer;
   public:
      MIXERLINECONTROLS mixerLineControls;
      VKMixerControls();
      ~VKMixerControls();
      void initialise( VKMixerLine *d, HMIXEROBJ hMixer );
      MIXERCONTROL * FindId( unsigned int controlid );
      bool Find( unsigned int controltype, DWORD & );
      const char * getComponentType( unsigned int );
      virtual void SaveMixerSettings( TiXmlElement & );
};
class _DLL_CLASS VKMixerLine
{
   public:
      VKMixerLine();
      virtual ~VKMixerLine();
      MIXERLINE lineinfo;
      VKMixerControls controls;
      MIXERCONTROL * FindId( unsigned int controlid );
      bool Find( unsigned int controltype, DWORD & );
      const char * getComponentType();
      virtual void SaveMixerSettings( TiXmlElement & ) = 0;
};
class _DLL_CLASS VKDestinationLine;
class _DLL_CLASS VKSourceLine : public VKMixerLine
{
   public:
      VKSourceLine();
      ~VKSourceLine();
      void initialise( VKDestinationLine *d, HMIXEROBJ hMixer, unsigned int u );
      virtual void SaveMixerSettings( TiXmlElement & );
};
class _DLL_CLASS VKDestinationLine : public VKMixerLine
{
   public:
      std::vector < VKSourceLine *> sourcelines;
      VKDestinationLine();
      ~VKDestinationLine();
      void VKDestinationLine::initialise( HMIXEROBJ hMixer, unsigned int u );
      MIXERCONTROL * FindId( unsigned int sourceid, unsigned int controlid );
      bool FindLineControl( DWORD srclineid, unsigned int controltype, DWORD &id );
      bool Find( unsigned int srclinetype, unsigned int controltype, DWORD & );
      virtual void SaveMixerSettings( TiXmlElement & );
};
class _DLL_CLASS VKMixer
{
      unsigned int mixerID;
      HMIXER	hMixer;
      MIXERCAPS	mixercaps;

      bool GetVolume( unsigned int, DWORD & );
      void SetVolume( unsigned int, DWORD );
      bool GetBool( unsigned int controlID );
      void SetBool( unsigned int controlID, bool setting );

      MIXERCONTROL * FindId( unsigned int destid, unsigned int sourceid, unsigned int controlid );
      bool FindLineControl( unsigned int destlinetype, DWORD srclineid, unsigned int controltype, DWORD &id );
      bool Find( unsigned int destlinetype, unsigned int srclinetype, unsigned int controltype, DWORD & );

      static std::vector<boost::shared_ptr<VKMixer> > mixers;
      static VKMixer *inputMixer;
      static VKMixer *outputMixer;
      static std::string inputLine;
      static std::string outputLine;
      static int mixerGetCount;

   public:
      std::vector < VKDestinationLine *> destlines;
      std::string getMixerName();
      static std::vector<boost::shared_ptr<VKMixer> > VKMixer::getMixers( HWND notWnd );
      static VKMixer *VKMixer::getMixer( const std::string &name );

      //static void GetPreferredMixers(std::string &recMixer, std::string &s, std::string &pbMixer, std::string &d);
      //static void SetPreferredMixers(std::string recMixer, std::string s, std::string pbMixer, std::string d);

      static VKMixer * setInputMixer( const std::string &name );
      static VKMixer * setOutputMixer( const std::string &name );

      void setInputMixerLine( const std::string &name );
      void setOutputMixerLine( const std::string &name );

      std::vector<std::string> getInputLines();
      std::vector<std::string> getOutputLines();


      VKMixer();
      bool Open( unsigned int, HWND );
      void Close();
      const char * getComponentType();

      // we shouldn't need these - they are mixersettings for a linetype
      bool GetMicRecVolume( DWORD & );
      void SetMicRecVolume( DWORD );  // use zero to drive mute if present?

      bool GetRecVolume( DWORD & );
      void SetRecVolume( DWORD );

      bool GetMicOutVolume( DWORD & );
      void SetMicOutVolume( DWORD );
      bool GetMicOutMute( bool & );
      void SetMicOutMute( bool );

      int SetMuxInput( std::string sInputName );
      bool GetMuxInput( DWORD &, std::string &sInputName );

      bool GetMasterVolume( DWORD & );
      void SetMasterVolume( DWORD );
      bool GetMasterMute( bool & );
      void SetMasterMute( bool );

      bool GetWaveOutVolume( DWORD & );
      void SetWaveOutVolume( DWORD );
      bool GetWaveOutMute( bool & );
      void SetWaveOutMute( bool );

      ~VKMixer();
      virtual void SaveMixerSettings( TiXmlDocument & );
      virtual void SaveMixerSettings( const std::string &fname );

      virtual bool LoadMixerSettings( TiXmlDocument &, std::string &err );
      virtual bool LoadMixerSettings( const std::string &fname, std::string &err );

      static VKMixer *GetInputVKMixer();
      static int getInputVKMixerID();
      static VKMixer *GetOutputVKMixer();
      static int getOutputVKMixerID();
      static void closeMixer();
};
//---------------------------------------------------------------------------
//==============================================================================

bool _DLL_FUNCTION LoadMixerSettings( const char *fname, std::string &err );
void _DLL_FUNCTION SaveMixerSettings( const char *fname = 0 );
//==============================================================================
//==============================================================================
#pragma pack(pop)
#endif
