/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "base_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------

MinosParameters *MinosParameters::mp = 0;
MinosParameters *MinosParameters::getMinosParameters()
{
   if ( !mp )
   {
      ShowMessage( "MinosParameters not initialised" );
   }
   return mp;
}

bool MinosParametersAdapter::insertContest( BaseContestLog */*p*/, unsigned int /*sno*/ )
{
   return true;
}
int MinosParametersAdapter::getMagneticVariation()
{
   return 0;
}
void MinosParametersAdapter::getDisplayColumnWidth( const std::string &/*key*/, int &val, int def )
{
   val = def;
}
void MinosParametersAdapter::setDisplayColumnWidth( const std::string &/*key*/, int /*val*/ )
{
}
void MinosParametersAdapter::getBoolDisplayProfile( int /*enumkey*/, bool &value )
{
   value = true;
}
void MinosParametersAdapter::setBoolDisplayProfile( int /*enumkey*/, bool /*value*/ )
{
}
void MinosParametersAdapter::flushDisplayProfile( void )
{
}
double MinosParametersAdapter::getBigClockCorrection()
{
   return 0.0;
}
int MinosParametersAdapter::getStatsPeriod1()
{
   return 0;
}
int MinosParametersAdapter::getStatsPeriod2()
{
   return 0;
}
void MinosParametersAdapter::setStatsPeriod1( int /*p*/ )
{
}
void MinosParametersAdapter::setStatsPeriod2( int /*p*/ )
{
}
void MinosParametersAdapter::addOperator( const std::string &/*curop*/ )
{}
void MinosParametersAdapter::setError( int /*err*/ )
{
}
bool MinosParametersAdapter::getAllowLoc4()
{
   return false;
}
bool MinosParametersAdapter::getAllowLoc8()
{
   return false;
}
bool MinosParametersAdapter::yesNoMessage( TComponent* /*Owner*/, String /*mess*/ )
{
   return false;
}
void MinosParametersAdapter::mshowMessage( String /*mess*/, TComponent* /*Owner*/ )
{
}
void MinosParametersAdapter::showContestScore( const std::string &/*score*/ )
{
}
BaseContestLog * MinosParametersAdapter::getCurrentContest()
{
   return 0;
}
bool MinosParametersAdapter::insertList( ContactList */*p*/, unsigned int /*sno*/ )
{
   return false;
}
bool MinosParametersAdapter::isContestOpen( const std::string /*fn*/ )
{
   return false;
}
bool MinosParametersAdapter::isListOpen( const std::string /*fn*/ )
{
   return false;
}
void MinosParametersAdapter::showErrorList( )
{
}
ErrorList &MinosParametersAdapter::getErrorList()
{
   return errs;
}
void MinosParametersAdapter::clearErrorList()
{
}
void MinosParametersAdapter::valtrace( int /*mess_no*/, bool /*flag*/ )
{
}
bool MinosParametersAdapter::isErrSet( int /*mess_no*/ )
{
   return false;
}

