/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MonitoredStationH
#define MonitoredStationH 
//---------------------------------------------------------------------------
class MonitoredStation
{
   private:

      // Plus we want the control (keyer) state, frequency, ?bandmap etc

   public:
      std::string stationName;
      PublishState state;
      std::vector<MonitoredLog *> slotList;
      
      MonitoredStation()
      {}
      ~MonitoredStation()
      {}   // need to delete slots...
};
#endif
