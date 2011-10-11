/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef ProfileEnumsH
#define ProfileEnumsH
//----------------------------------------------------------------------------
enum LOGGERPROFILE {
   elpPreloadFile, elpPreloadSection,
   elpDisplayFile, elpDisplaySection,
   elpOperatorFile, elpOperatorSection,
   elpListDirectory, elpLogDirectory,
   elpEntryFile,
   elpStationFile, elpQTHFile,
   elpFontName, elpFontSize, elpFontBold, elpFontItalic,
   elpAutoFill
};
enum PRELOADPROFILE {eppCurrent, epp1 , epp2 , epp3 , epp4 , epp5 , epp6 , epp7 , epp8 , epp9 , epp10};

enum DISPLAYPROFILE {edpTop, edpLeft, edpWidth, edpHeight,
                     edpShowContinentEU, edpShowContinentAS,
                     edpShowContinentAF, edpShowContinentOC,
                     edpShowContinentSA, edpShowContinentNA,
                     edpShowWorked, edpShowUnworked,
                     edpNextContactDetailsOnLeft,
                     edpScrollingContestTabs,
                     edpShowOperators,
                     edpEditor, edpStatisticsPeriod1, edpStatisticsPeriod2,
                     edpAutoBandMapTune, edpAutoBandMapTuneAmount,
                     edpAutoBandMapTime, edpAutoBandMapTimeLapse,
                    };
enum ENTRYPROFILE {eepCall, eepEntrant, eepMyName, eepMyCall,
                   eepMyAddress1, eepMyAddress2, eepMyCity, eepMyPostCode, eepMyCountry,
                   eepMyPhone, eepMyEmail};
enum QTHPROFILE {eqpLocator, eqpDistrict, eqpLocation, eqpStationQTH1, eqpStationQTH2, eqpASL};
enum STATIONPROFILE {espPower, espTransmitter, espReceiver, espAntenna, espAGL, espOffset};

enum PROFILES {epLOGGERPROFILE, epPRELOADPROFILE, epDISPLAYPROFILE,
			   epENTRYPROFILE, epQTHPROFILE, epSTATIONPROFILE};
#endif
