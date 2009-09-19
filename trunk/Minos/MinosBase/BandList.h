/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BandListH
#define BandListH

//---------------------------------------------------------------------------

class BandInfo
{
   public:
      std::string type;
      double flow;
      double fhigh;
      std::string wlen;
      std::string uk;
      std::string cabrillo;
      std::string adif;
      std::string reg1test;
};
class TiXmlElement;
class BandList
{
      bool parseBand(TiXmlElement * e);
   public:
      BandList();
      ~BandList();
      std::vector<BandInfo> bandList;
      bool parseFile(const std::string &bandFile);
      bool findBand(int freq, BandInfo &);
      bool findBand(const std::string &cb, BandInfo &bi);

      static BandList &getBandList();

};
#endif
