/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef LatlongH
#define LatlongH 
//----------------------------------------------------------------------------
#define GRIDOK 10000
#define INVALIDGREF 10001

typedef enum gridstyle {NUL, GEO, OWN, NGR, LOC} gridstyle;

struct Location
{
   gridstyle gridstyle;
   char * datastring;
   int dataoffset, datalength;
   double northing, easting, centremeridian;
};

double raddeg( double );
double degrad( double );
void dms( double, int *, int *, double * );
int transform( Location *, Location * );
#endif
