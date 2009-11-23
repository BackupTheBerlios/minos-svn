/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef keyconfH
#define keyconfH

class PortConfig
{
   public:
      enum PortType {eptWindows, eptMinosControl};
      std::string name;
      PortType portType;

      PortConfig()
      {}
      PortConfig( const std::string &name, PortType portType ) :
            name( name ), portType( portType )
      {}
}
;

class KeyerConfig
{
   public:
      enum KeyerType {ektVoiceKeyer};
      std::string name;
      KeyerType type;
      int pipTone;
      bool enablePip;
      int startDelay;
      int autoRepeatDelay;
      bool enableAutoRepeat;
      int pipStartDelay;
      int playPTTDelay;
      int voxHangTime;
      int pipVolume;
      int pipLength;
      int clipRecord;

      KeyerConfig()
      {}
      KeyerConfig( const std::string &name, int pipTone, int pipVolume, int pipLength, bool enablePip, int startDelay,
                   int autoRepeatDelay, bool enableAutoRepeat, int pipStartDelay, int playPTTDelay, int voxHangTime, int clipRecord )
            : name( name ), type( ektVoiceKeyer ), pipTone( pipTone ), enablePip( enablePip ), startDelay( startDelay ),
            autoRepeatDelay( autoRepeatDelay ), enableAutoRepeat( enableAutoRepeat ), pipStartDelay( pipStartDelay ), playPTTDelay( playPTTDelay ),
            voxHangTime( voxHangTime ), pipVolume( pipVolume ), clipRecord( clipRecord ), pipLength( pipLength )
      {}
}
;

extern bool configureKeyers();

class commonKeyer;
class TiXmlElement;

class KeyerConfigure
{
   private:
      std::map <std::string, PortConfig> portmap;
      std::map <std::string, KeyerConfig> keyermap;

      commonKeyer *createKeyer( const KeyerConfig &keyer, const PortConfig &port );
      void SetTrace( TiXmlElement *e );
      void SetTune( TiXmlElement *e );
      void SetPorts( TiXmlElement *e );
      void SetKeyers( TiXmlElement *e );
      void SetEnable( TiXmlElement *e );
      void SetCW( TiXmlElement *e );
      void SetMorseCode( TiXmlElement *e );
      void SetCWMessages( TiXmlElement *e );
   public:
      bool configureKeyers();
};
#endif
