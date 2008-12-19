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
static MinosParametersAdapter mp;
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

