/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#include "display_pch.h"
#pragma hdrstop

#include "multtestmain.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1( TComponent* Owner )
      : TForm( Owner )
{}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click( TObject * /*Sender*/ )
{
   /*MultLists *m =*/ MultLists::getMultLists();
   /*
   std::ofstream os("c:\\temp\\multlist.txt");

   os << "================== country entries ========================" << std::endl;
   for (MultList < CountryEntry * >::iterator i = m->ctryList.begin(); i != m->ctryList.end(); i++)
   {
      os << (*i)->basePrefix + " " + (*i)->realName << std::endl;
   }
   os << (String("================== country synonyms ") + String(m->ctrySynList.size()).c_str() + "========================").c_str() << std::endl;
   for (MultList < CountrySynonym * >::iterator i = m->ctrySynList.begin(); i != m->ctrySynList.end(); i++)
   {
      std::string temp1 = (*i)->synPrefix;
   //      CountryEntry * country = *((*i)->country);
      CountryEntry * country = (*i)->country;
      std::string temp2 = country->basePrefix;
      os << (temp1 + " : " + temp2) << std::endl;
   }
   os << "================== district entries ========================" << std::endl;
   for (MultList < DistrictEntry * >::iterator i = m->distList.begin(); i != m->distList.end(); i++)
   {
      os << (*i)->districtCode << std::endl;
   }
   os << "================== district synonyms ========================" << std::endl;
   for (MultList < DistrictSynonym * >::iterator i = m->distSynList.begin(); i != m->distSynList.end(); i++)
   {
      os << ((*i)->synonym + " : " + ((*i)->district)->districtCode) << std::endl;
   }
   os << "================== Glist ========================" << std::endl;
   for (MultList < GlistEntry * >::iterator i = m->glist.begin(); i != m->glist.end(); i++)
   {
      os << ((*i)->synPrefix + " : " + (*i)->dupPrefix) << std::endl;
   }
   */

}
//---------------------------------------------------------------------------
class MonitorParameters : public MinosParameters
{
      int p1;
      int p2;
   public:
      MonitorParameters() : p1( 10 ), p2( 60 )
      {}
      ~MonitorParameters()
      {}
      virtual bool insertContest( BaseContestLog *p, unsigned int sno );
      virtual int getMagneticVariation();
      virtual void getDisplayColumnWidth( const std::string &key, int &val, int def );
      virtual void setDisplayColumnWidth( const std::string &key, int val );
      virtual void getBoolDisplayProfile( int enumkey, bool &value );
      virtual void setBoolDisplayProfile( int enumkey, bool value );
      virtual void flushDisplayProfile( void );
      virtual double getBigClockCorrection();
      virtual int getStatsPeriod1();
      virtual int getStatsPeriod2();
      virtual void setStatsPeriod1( int );
      virtual void setStatsPeriod2( int );
      virtual void addOperator( const std::string &curop );
      virtual void setError( int err );
      virtual bool getAllowLoc4();
      virtual bool getAllowLoc8();
      virtual bool yesNoMessage( TComponent* Owner, String mess );
      virtual void mshowMessage( String mess, TComponent* Owner = 0 );
      virtual void showErrorList( ErrorList &errs );
      virtual void showContestScore( const std::string &score );
      virtual BaseContestLog *getCurrentContest();
      virtual bool insertList( ContactList *p, unsigned int sno );
      virtual bool isContestOpen( const std::string fn );
      virtual bool isListOpen( const std::string fn );
};
static MonitorParameters mp;

bool MonitorParameters::insertContest( BaseContestLog */*p*/, unsigned int /*sno*/ )
{
   return true;
}
int MonitorParameters::getMagneticVariation()
{
   return 0;
}
void MonitorParameters::getDisplayColumnWidth( const std::string &/*key*/, int &val, int def )
{
   val = def;
}
void MonitorParameters::setDisplayColumnWidth( const std::string &/*key*/, int /*val*/ )
{
}
void MonitorParameters::getBoolDisplayProfile( int /*enumkey*/, bool &value )
{
   value = true;
}
void MonitorParameters::setBoolDisplayProfile( int /*enumkey*/, bool /*value*/ )
{
}
void MonitorParameters::flushDisplayProfile( void )
{
}
double MonitorParameters::getBigClockCorrection()
{
   return 0.0;
}
int MonitorParameters::getStatsPeriod1()
{
   return p1;
}
int MonitorParameters::getStatsPeriod2()
{
   return p2;
}
void MonitorParameters::setStatsPeriod1( int p )
{
   p1 = p;
}
void MonitorParameters::setStatsPeriod2( int p )
{
   p2 = p;
}
void MonitorParameters::addOperator( const std::string &/*curop*/ )
{}
void MonitorParameters::setError( int /*err*/ )
{
}
bool MonitorParameters::getAllowLoc4()
{
   return false;
}
bool MonitorParameters::getAllowLoc8()
{
   return false;
}
bool MonitorParameters::yesNoMessage( TComponent* Owner, String mess )
{

   return mShowYesNoMessage( Owner, mess );
}
void MonitorParameters::mshowMessage( String mess, TComponent* Owner )
{
   mShowMessage( mess, Owner );
}
void MonitorParameters::showErrorList( ErrorList &/*errs*/ )
{
}
void MonitorParameters::showContestScore( const std::string &/*score*/ )
{}
BaseContestLog * MonitorParameters::getCurrentContest()
{
   return 0;
}
bool MonitorParameters::insertList( ContactList */*p*/, unsigned int /*sno*/ )
{
   return false;
}
bool MonitorParameters::isContestOpen( const std::string /*fn*/ )
{
   return false;
}
bool MonitorParameters::isListOpen( const std::string /*fn*/ )
{
   return false;
}

