/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef portconfH
#define portconfH

class commonController;
class PortConfig
{
   public:
      enum PortType {eptWindows, eptSerial, eptK8055, eptUBW};
      std::string name;
      PortType portType;
      std::string portName;

      PortConfig()
      {}
      PortConfig( const std::string &name, PortType portType, const std::string &portName ) :
            name( name ), portType( portType ), portName( portName )
      {}
}
;

class LineConfig
{
   public:
      std::string name;
      bool lineIn;
      std::string portName;
      std::string portLineName;

      LineConfig()
      {}
      LineConfig( const std::string &name, const std::string &lineIn, const std::string &portName, const std::string &portLineName )
            : name( name ), lineIn( lineIn == "in" ), portName( portName ), portLineName( portLineName )
      {}
}
;

extern bool configurePorts( commonController & );

class TiXmlElement;

class PortConfigure
{
   private:
      std::map <std::string, PortConfig> portmap;
      std::map <std::string, LineConfig> linemap;

      void SetPorts( TiXmlElement *e );
      void SetLines( TiXmlElement *e );
      void SetEnable( TiXmlElement *e, commonController &monitor );
   public:
      bool configurePorts( commonController & );
};
#endif
