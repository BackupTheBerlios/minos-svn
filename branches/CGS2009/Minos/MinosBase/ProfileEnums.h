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
   elpEntryFile, elpContestFile,
   elpStationFile, elpQTHFile
};
enum PRELOADPROFILE {eppCurrent, epp1 , epp2 , epp3 , epp4 , epp5 , epp6 , epp7 , epp8 , epp9 , epp10};

enum DISPLAYPROFILE {edpFontName, edpFontSize,
                     edpTop, edpLeft, edpWidth, edpHeight,
                     edpShowContinentEU, edpShowContinentAS,
                     edpShowContinentAF, edpShowContinentOC,
                     edpShowContinentSA, edpShowContinentNA,
                     edpShowWorked, edpShowUnworked,
                     edpNextContactDetailsOnLeft,
                     edpEditor, edpStatisticsPeriod1, edpStatisticsPeriod2,
                     edpAutoBandMapTune, edpAutoBandMapTuneAmount,
                     edpAutoBandMapTime, edpAutoBandMapTimeLapse,
                    };
enum ENTRYPROFILE {eepCall, eepEntrant, eepMyName, eepMyCall,
                   eepMyAddress1, eepMyAddress2, eepMyCity, eepMyPostCode, eepMyCountry,
                   eepMyPhone, eepMyEmail};
enum CONTESTPROFILE {ecpName, ecpBands, ecpSections, ecpScoreKmQSO,
                     ecpDistrictMult, ecpDXCCMult, ecpLocMult, ecpQTHReq,
                     ecpAllowLoc4, ecpAllowLoc8,
                     ecpStart, ecpEnd};
enum QTHPROFILE {eqpLocator, eqpDistrict, eqpLocation, eqpStationQTH1, eqpStationQTH2, eqpASL};
enum STATIONPROFILE {espPower, espTransmitter, espReceiver, espAntenna, espAGL};

enum PROFILES {epLOGGERPROFILE, epPRELOADPROFILE, epDISPLAYPROFILE,
               epENTRYPROFILE, epCONTESTPROFILE, epQTHPROFILE, epSTATIONPROFILE};
#endif
