/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2007 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef VHFListH
#define VHFListH 
//---------------------------------------------------------------------------
class MultType
{
   public:
      std::string name;
      std::string description;
      std::string longDescription;
      std::string scoringDescription;
      std::string exchange;
};
class SpecialRule
{
   public:
      std::string name;
      std::string description;
      std::string longDescription;
};
class MonthList
{
   public:
      std::string month;
};
class Section
{
   public:
      Section() : singleAntenna( false ), overall( false )
      {}
      std::vector<MonthList> monthList;

      std::string name;
      std::string description;
      std::string longDescription;
      std::string power;
      std::string height;
      bool singleAntenna;
      bool overall;
};
class StartDateList
{
   public:
      std::string date;

};
class TimeList
{
   public:
      std::vector<MonthList> monthList;
      std::vector<StartDateList> startDateList;
      std::string startDay;
      std::string startWeek;
      std::string startTime;
      std::string timeType;
      std::string duration;
      std::string mult;
};
class ContestBand
{
   public:
      std::string name;
      std::string reg1band;
      std::string wavelength;
      std::vector<TimeList> timeList;
};
class ContestList
{
   public:
      std::string name;

};
class ContestSeries
{
   public:
      std::string name;
      std::string description;
      std::vector<ContestList>contestList;
      std::vector<std::string>awardList;
};
class SectionList
{
   public:
      std::string name;

};
class CalendarBandList
{
   public:
      std::string name;

};
class SpecialRuleList
{
   public:
      std::string name;

};
class Contest
{
   public:
      Contest() : scoring( perkms ), iaru( false )
      {}

      std::string name;
      std::string description;
      std::string shortDescription;
      enum ScoreType{oneppq, perkms} scoring;
      bool iaru;
      std::string mult;
      std::string power;
      std::string mode;
      std::string entryDate;

      std::vector<TimeList> timeList;
      std::vector<CalendarBandList> bandList;
      std::vector<SectionList> sectionList;
      std::vector<SpecialRuleList> specialRulesList;
      std::vector<std::string> awardList;

};
class IndividualContest
{
   public:
      TDateTime start;
      TDateTime finish;
      std::string duration;

      std::string description;
      std::string shortDescription;
      std::string bands;
      std::string reg1band;
      bool ppKmScoring;
      std::string sections;
      std::string mults;
      std::string specialRules;
      std::string power;
      std::string antenna;

      bool operator<( const IndividualContest& rhs ) const;
};
class TiXmlElement;
class Calendar
{
   public:
      Calendar(int year);
      ~Calendar();
      bool parseFile( const std::string &fname );
      bool parseMultType( TiXmlElement * e );
      bool parseSpecialRule( TiXmlElement * e );
      bool parseSection( TiXmlElement * e );
      bool parseBand( TiXmlElement * e );
      bool parseContestSeries( TiXmlElement * e );
      bool parseContest( TiXmlElement * e );

      bool parseTimeList( TiXmlElement * e, std::vector<TimeList> &timeList );

      std::string version;
      std::map<std::string, MultType> mults;
      std::map<std::string, SpecialRule> specialRules;
      std::map<std::string, Section> sections;
      std::map<std::string, ContestBand> bands;
      std::map<std::string, ContestSeries> contestSeries;
      std::map<std::string, Contest> contests;

      std::vector<IndividualContest> calendar;
};
extern TDateTime localToUTC( TDateTime t );
extern void setYear( int y );
extern int getMonth( const std::string &m );
extern int getDate( int month, int day, int week );
extern int getDayOfWeek( TDateTime dtg );

#endif
