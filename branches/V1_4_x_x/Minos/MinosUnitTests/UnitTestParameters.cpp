/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <string>

#define _MSC_VER 2300
#include "sorted_vector.h"
#include "validators.h"
#include "MinosParameters.h"
#include "UnitTestParameters.h"

class UnitTestParameters : public MinosParametersAdapter
{
      int p1;
      int p2;
   public:
      UnitTestParameters() : p1( 10 ), p2( 60 )
      {}
      ~UnitTestParameters()
      {}
      virtual int getStatsPeriod1();
      virtual int getStatsPeriod2();
      virtual void setStatsPeriod1( int );
      virtual void setStatsPeriod2( int );
};
static UnitTestParameters mp;

int UnitTestParameters::getStatsPeriod1()
{
   return p1;
}
int UnitTestParameters::getStatsPeriod2()
{
   return p2;
}
void UnitTestParameters::setStatsPeriod1( int p )
{
   p1 = p;
}
void UnitTestParameters::setStatsPeriod2( int p )
{
   p2 = p;
}

