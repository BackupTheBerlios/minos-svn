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

typedef struct transformelement
{
   int ( * tfproc ) ( Location *, Location * );
   gridstyle tfelement[ 2 ][ 2 ];
}
transformelement;

static int nulinput( Location * );
static int nuloutput( Location * );
static int geoinput( Location * );
static int geooutput( Location * );
static int owninput ( Location * );
static int ownoutput( Location * );
static int ngrinput ( Location * );
static int ngroutput( Location * );
static int locinput ( Location * );
static int locoutput( Location * );

static int identitytransform ( Location *, Location * );
static int tfnull ( Location *, Location * );
static int geotointernal ( Location *, Location * );
static int internaltogeo ( Location *, Location * );
static int internaltongr ( Location *, Location * );
static int ngrtointernal ( Location *, Location * );
static int internaltoloc ( Location *, Location * );
static int loctointernal ( Location *, Location * );

static void nulcentre( double, double * );
static void geocentre( double, double * );
static void owncentre( double, double * );
static void ukoscentre( double, double * );
static void loccentre ( double, double * );
static void setprojectionconstants( struct projectionparams * );

static int tfele( int, int, Location *, Location *, int * );

static int getgridcoord( char *, int, int, double *, double * );
static double north( double );
static double dnorth( double );
static int transngrletters( char, int *, int * );


static int ( * datatransform[ LOC + 1 ][ 2 ] ) ( Location * ) = {
         {nulinput, nuloutput},
         {geoinput, geooutput},
         {owninput, ownoutput},
         {ngrinput, ngroutput},
         {locinput, locoutput}
      };

static struct transformelement tfmatrix[ LOC + 1 ][ LOC + 1 ] =
   {
      {		/* dummy! */
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
      }
      , {      /* geog to ... */
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {identitytransform, {{NUL, NUL}, {NUL, NUL}} },
         {geotointernal, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{GEO, OWN}, {OWN, NGR}} },
         {identitytransform, {{NUL, NUL}, {NUL, NUL}} },
      }, {    /* internal to ... */
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {internaltogeo, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{OWN, GEO}, {GEO, OWN}} },
         {internaltongr, {{NUL, NUL}, {NUL, NUL}} },
         {internaltoloc, {{NUL, NUL}, {NUL, NUL}} },
      }, {    /* ngr to ... */
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{NGR, OWN}, {OWN, GEO}} },
         {ngrtointernal, {{NUL, NUL}, {NUL, NUL}} },
         {identitytransform, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{NGR, GEO}, {GEO, LOC}} },
      }, {    /* loc to ... */
         {tfnull, {{NUL, NUL}, {NUL, NUL}} },
         {identitytransform, {{NUL, NUL}, {NUL, NUL}} },
         {loctointernal, {{NUL, NUL}, {NUL, NUL}} },
         {tfnull, {{LOC, GEO}, {GEO, NGR}} },
         {identitytransform, {{NUL, NUL}, {NUL, NUL}} },
      }
   };

static Location interlocs[ 8 ] = {
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                    {NUL, "DUMMY", 0, 0, 0.0, 0.0, 0.0},
                                 };
static Location p_ingrid, p_outgrid;

static double incentre = 0.0, outcentre = 0.0;

static gridstyle instyle, outstyle;

struct spheroidparams
{
   double a0, b0;
};

struct projectionparams
{
   struct spheroidparams *spheroid;
   double f0;
};

static struct spheroidparams
         nullsphere =
   {
      6377563.396, 6356256.910
   },
   airy = {6377563.396, 6356256.910};
//static struct spheroidparams
//		international = {6378388.0, 6356912.0},
//		krasovsky     = {6378245.0, 6356863.0};

static struct projectionparams grids[ LOC + 1 ] =
   {
      {
         &nullsphere, 1.0
      },
      {&nullsphere, 1.0},
      {&nullsphere, 1.0},
      {&airy, 0.9996012717},
      {&nullsphere, 1.0},
   };

static void ( * setprojcentre[ LOC + 1 ] ) ( double, double * ) =
   {nulcentre, geocentre, owncentre, ukoscentre, loccentre};

static int coordele[ 10 ];

static double a = 0.0, b = 0.0, e2 = 0.0, n1 = 0.0, n2 = 0.0, n3 = 0.0, ex = 0.0;

extern const double pi;
/***************************************************************************/


static void setprojectionconstants( projectionparams *proj )
{
   double apb = 0.0, amb = 0.0;

   a = proj->spheroid->a0 * proj->f0;
   b = proj->spheroid->b0 * proj->f0;
   apb = a + b;
   amb = a - b;
   e2 = ( a * a - b * b ) / ( a * a );
   n1 = amb / apb;
   n2 = ( amb * amb ) / ( apb * apb );
   n3 = ( amb * amb * amb ) / ( apb * apb * apb );
   ex = ( a * a - b * b ) / ( b * b );

}
/**********************************************************************/
static int tfnull( Location * /*l1*/, Location * /*l2*/ )
{
   return ( INVALIDGREF );
}

static int identitytransform( Location *l1, Location *l2 )
{
   l2->northing = l1->northing;
   l2->easting = l1->easting;
   l2->centremeridian = l1->centremeridian;
   return ( GRIDOK );
}
/**********************************************************************/

static int tfele( int i1, int i2, Location *ingrid, Location *outgrid, int *freelocation )
{
   int status;
   transformelement *tf = &tfmatrix[ i1 ][ i2 ];
   if ( tf->tfproc != tfnull )
   {
      status = ( * tf->tfproc ) ( ingrid, outgrid );
      return ( status );
   }
   ( *freelocation ) ++;
   if ( *freelocation > 8 )
      return ( INVALIDGREF );

   status = tfele( tf->tfelement[ 0 ][ 0 ], tf->tfelement[ 0 ][ 1 ],
                   ingrid,
                   &interlocs[ *freelocation - 1 ],
                   freelocation );

   if ( status == GRIDOK )
      status = tfele( tf->tfelement[ 1 ][ 0 ], tf->tfelement[ 1 ][ 1 ],
                      &interlocs[ *freelocation - 1 ],
                      outgrid,
                      freelocation );

   ( *freelocation ) --;

   return ( status );
}
/**********************************************************************/

int transform( Location *fingrid, Location *foutgrid )
{
   int status;
   int frl = 0;
   Location *ig, * og;

   ig = &p_ingrid;
   memmove( ig, fingrid, sizeof( Location ) );

   og = &p_outgrid;
   memmove( og, foutgrid, sizeof( Location ) );

   incentre = p_ingrid.centremeridian;
   outcentre = p_outgrid.centremeridian;
   instyle = p_ingrid.gridstyle;
   outstyle = p_outgrid.gridstyle;

   setprojectionconstants( &grids[ instyle ] );

   status = ( * datatransform[ instyle ][ 0 ] ) ( &p_ingrid );

   if ( status == GRIDOK )
      status = tfele( instyle, outstyle, &p_ingrid, &p_outgrid, &frl );

   if ( status == GRIDOK )
      status = ( * datatransform[ outstyle ][ 1 ] ) ( &p_outgrid );

   og = &p_outgrid;

   memmove( foutgrid, og, sizeof( Location ) );
   return ( status );
}
/**********************************************************************/
static void ccor( double &y )
{
   while ( ( y < -180.0 ) || ( y > 180.0 ) )
      y += ( y < 0.0 ) ? 360.0 : -360.0;
}
static double raddeg( double x )
{
   double y = x * 180.0 / pi;

   ccor( y );
   return ( y );

}

double degrad( double x )
{
   ccor( x );

   return ( x * pi / 180.0 );
}
/**********************************************************************/

static int getgridcoord( char *gridref, int offset, int dlength, double *e, double *n )
{
   int i = offset;
   int j = offset;
   TEMPBUFF( tbuff, 50 );
   double x = 0.0, y = 0.0;

   if ( dlength >= 50 )
      return ( INVALIDGREF );

   while ( ( i < 20 ) && gridref[ i ] )
   {
      if ( gridref[ i ] != ' ' )
         tbuff[ j++ ] = gridref[ i ];
      i++;
   }
   tbuff[ j ] = 0;
   i = 0;

   while ( ( ( i + offset ) < dlength ) && ( isdigit( tbuff[ i + offset ] ) )
           && ( i < 10 ) )
   {
      coordele[ i ] = tbuff[ i + offset ] - '0';
      i++;
   }

   if ( i % 2 == 1 )
      return ( INVALIDGREF );

   for ( j = 0; j < ( i / 2 ); j++ )
      x = x * 10.0 + coordele[ j ];

   for ( ; j < i; j++ )
      y = y * 10.0 + coordele[ j ];

   for ( j = i + 2; j <= 10; j += 2 )
   {
      x = x * 10.0;
      y = y * 10.0;
   }

   *e = x;
   *n = y;
   return ( GRIDOK );
}
/**********************************************************************/

static double north( double phi )
{
   double res = 0.0;

   res = b * phi + b * ( n1 * ( 1.0 + 5.0 * n1 * ( 1.0 + n1 ) / 4.0 ) * phi
                         - 3.0 * n1 * ( 1.0 + n1 * ( 1.0 + 7.0 * n1 / 8.0 ) ) * sin( phi ) * cos( phi )
                         + ( 15.0 * n1 * ( n1 + n2 ) / 8.0 ) * sin( 2.0 * phi ) * cos( 2.0 * phi )
                         - ( 35.0 * n3 / 24.0 ) * sin( 3.0 * phi ) * cos( 3.0 * phi ) );

   return ( res );
}

static double dnorth( double phi )
{
   double res = 0.0;

   res = b * ( ( 1.0 + n1 + 5.0 * ( n2 + n3 ) / 4.0 ) - 3.0 * ( n1 + n2 + 7.0 * n3 / 8.0 ) * cos( 2.0 * phi )
               + ( 15.0 * ( n2 + n3 ) / 4.0 ) * cos( 4 * phi ) - ( 35.0 * n3 / 8.0 ) * cos( 6.0 * phi ) );
   return ( res );
}
/**********************************************************************/
static void nulcentre ( double /*longi*/, double *centre )
{
   *centre = 0.0;
}
static void geocentre ( double /*longi*/, double *centre )
{
   *centre = 0.0;
}

static void owncentre ( double /*longi*/, double * /*centre*/ )
{
   /* Null for internal output */
}
/**********************************************************************/

static int internaltogeo( Location *internal, Location *ll )
{
   double northing = internal->northing;
   double easting = internal->easting;
   double *lat = &ll->northing;
   double *longi = &ll->easting;

   double nu = 0.0, phid = 0.0, phid2 = 0.0, t2 = 0.0, t = 0.0, q2 = 0.0, c = 0.0, s = 0.0;
   double nu2 = 0.0, nudivrho = 0.0, invnurho = 0.0;

   /* Evaluate M term: latitude of the northing on the centre meridian */

   phid = northing / ( b * ( 1.0 + n1 + 5.0 * ( n2 + n3 ) / 4.0 ) ) - 1.0;
   phid2 = phid + 1.0;

   while ( fabs( phid2 - phid ) > 1E-6 )
   {
      phid = phid2;
      phid2 = phid - ( north( phid ) - northing ) / dnorth( phid );
   }

   c = cos( phid );
   s = sin( phid );
   t = tan( phid );

   t2 = t * t;

   q2 = easting * easting;

   /* nu is evaluated by a taylor series approximation to
   	an inverse square root */

   nu = a + a * e2 * s * s / 2.0 + 3.0 * a * e2 * e2 * s * s * s * s / 4.0;

   nu2 = ( a * a ) / ( 1.0 - e2 * s * s );

   nudivrho = a * a * c * c / ( b * b ) - c * c + 1.0;

   invnurho = ( ( 1.0 - e2 * s * s ) * ( 1.0 - e2 * s * s ) ) / ( a * a * ( 1.0 - e2 ) );

   *lat = phid - t * q2 * invnurho / 2.0;

   *longi = easting / ( c * nu )
            - easting * q2 * ( ( nudivrho + 2.0 * t2 ) / ( 6.0 * nu2 ) ) / ( c * nu )
            + internal->centremeridian;

   ll->centremeridian = 0.0;

   return ( GRIDOK );

}
/**********************************************************************/

static int geotointernal( Location *ll, Location *internal )
{

   double * northing = &internal->northing;
   double *easting = &internal->easting;
   double *centremeridian = &internal->centremeridian;
   double lat = ll->northing;
   double longi = ll->easting;

   double nu = 0.0, eta2 = 0.0, nudivrho = 0.0, t = 0.0, c = 0.0, s = 0.0, t2 = 0.0, p = 0.0;
   double longlim = ( 15 * 2 * pi ) / 360;

   setprojectionconstants( &grids[ outstyle ] );

   *centremeridian = outcentre;

   if ( *centremeridian == 0.0 )
      ( *setprojcentre[ outstyle ] ) ( ll->easting, centremeridian );

   p = longi - *centremeridian;

   if ( fabs( p ) > longlim )
      return ( INVALIDGREF );

   c = cos( lat );
   s = sin( lat );
   t = tan( lat );

   t2 = t * t;

   eta2 = ex * s * s;

   /* nu is evaluated by a taylor series approximation to
   	an inverse square root */

   nu = a + a * e2 * s * s / 2.0 + 3.0 * a * e2 * e2 * s * s * s * s / 4.0;

   nudivrho = a * a * c * c / ( b * b ) - c * c + 1.0;

   *northing = north( lat )
               + p * p * c * s * nu / 2.0
               + p * p * p * p * c * c * c * s * nu * ( 5.0 - t2 ) / 24.0
               + p * p * p * p * c * c * c * s * nu * 9.0 * eta2 / 24.0;
   *easting = p * c * nu
              + p * p * p * c * c * c * nu * ( ( nudivrho - t2 ) / 6.0 );

   return ( GRIDOK );
}
/**********************************************************************/

static int transngrletters( char letter, int *e, int *n )
{
   int work;

   if ( ( !isalpha( letter ) ) || ( ( letter = toupper( letter ) ) == 'I' ) )
      return ( INVALIDGREF );

   work = letter - 'A';
   if ( work > 8 )
      work--;

   *e = work % 5;
   *n = 4 - work / 5;

   return ( GRIDOK );
}

static int ngrinput( Location *ingrid )
{

   int ea, eb, na, nb;
   char l1, l2;
   double ge = 0.0, gn = 0.0;

   char *gridref = ingrid->datastring;
   int offset = ingrid->dataoffset;
   int dlength = ingrid->datalength;

   ingrid->centremeridian = degrad( -2 );

   strupr( gridref );

   l1 = gridref[ offset++ ];
   l2 = gridref[ offset++ ];

   if ( getgridcoord( gridref, offset, dlength, &ge, &gn ) != GRIDOK )
      return ( INVALIDGREF );

   if ( ( transngrletters( l1, &ea, &na ) != GRIDOK )
        || ( transngrletters( l2, &eb, &nb ) != GRIDOK ) )
      return ( INVALIDGREF );

   ingrid->easting = 500000.0 * ( ea - 2.0 ) + 100000.0 * eb + ge;
   ingrid->northing = 500000.0 * ( na - 1.0 ) + 100000.0 * nb + gn;
   return ( GRIDOK );
}

static int ngrtointernal( Location *ngrgrid, Location *internal )
{
   internal->centremeridian = degrad( -2 );
   internal->easting = ngrgrid->easting - 400000.0;
   internal->northing = north( degrad( 49.0 ) ) + ngrgrid->northing
                        + 100000.0;
   return ( GRIDOK );
}

static int internaltongr( Location *ingrid, Location *outgrid )
{
   outgrid->northing = ingrid->northing - north( degrad( 49.0 ) )
                       - 100000.0;

   outgrid->easting = ingrid->easting + 400000.0;

   return ( GRIDOK );
}

static int ngroutput( Location *outgrid )
{

   int ea, eb, na, nb;
   int gri = outgrid->dataoffset;
   double ger = 0.0, gnr = 0.0;
   double n1 = 0.0, e = 0.0;
   char *gridref = outgrid->datastring;

   e = outgrid->easting + 1000000.0;
   n1 = outgrid->northing + 500000.0;

   if ( ( e < 0.0 ) || ( n1 < 0.0 ) || ( e > 2500000.0 ) || ( n1 > 2500000.0 ) )
      return ( INVALIDGREF );

   ea = ( int ) ( e / 500000.0 );
   na = ( int ) ( n1 / 500000.0 );

   eb = ( int ) ( ( e - ( 500000.0 * ea ) ) / 100000.0 );
   nb = ( int ) ( ( n1 - ( 500000.0 * na ) ) / 100000.0 );

   ger = e - ( 500000.0 * ea + 100000.0 * eb );
   gnr = n1 - ( 500000.0 * na + 100000.0 * nb );

   gridref[ gri ] = ( char ) ( ( 4 - na ) * 5 + ea + 'A' );

   if ( gridref[ gri ] >= 'I' )
      gridref[ gri ] ++;

   gridref[ ++gri ] = ( char ) ( ( 4 - nb ) * 5 + eb + 'A' );

   if ( gridref[ gri ] >= 'I' )
      gridref[ gri ] ++;

   std::string pt1 = ( boost::format( "%05.5lu" ) % ( long ) ger ).str();
   std::string pt2 = ( boost::format( "%05.5lu" ) % ( long ) gnr ).str();
   std::string gr = ( boost::format( " %6.6s %6.6s" ) % pt1 % pt2 ).str();

   strcpy( &gridref[ ++gri ], gr.c_str() );
   //   sprintf( &gridref[ ++gri ], "%06.6s%06.6s",
   //            pt1.c_str(), pt2.c_str() );

   return ( GRIDOK );
}
static void ukoscentre ( double /*longi*/, double *centre )
{
   *centre = degrad( -2.0 );
}
/**********************************************************************/

//extern char lonlat(const char *qra, double &lon, double &lat);	// from cdlg.h
//#define LOC_OK 0	// from mwin.h

static int locinput( Location *ingrid )
{
   int gri = ingrid->dataoffset;
   char *gridref = &ingrid->datastring[ gri ];

   if ( lonlat( gridref, ingrid->easting, ingrid->northing ) == LOC_OK )
      return GRIDOK;
   return ( INVALIDGREF );
}

static int loctointernal( Location *ingrid, Location *outgrid )
{
   return ( geotointernal( ingrid, outgrid ) );
}

static int internaltoloc( Location *ingrid, Location *outgrid )
{
   return ( internaltogeo( ingrid, outgrid ) );
}

static void txgeoloc( double *n, double *e, int f, char t, char **gp )
{
   *e = f * ( *e - ( ( int ) * e ) );
   *n = f * ( *n - ( ( int ) * n ) );

   *( ( *gp ) ++ ) = ( char ) ( ( int ) ( *e ) + t );
   *( ( *gp ) ++ ) = ( char ) ( ( int ) ( *n ) + t );
}

int geotoloc( double lat, double longi, char *&gridref )
{
   // lat, longi to be in degrees, -ve for W or S

   longi = longi / 360 + 0.5;
   lat = lat / 180 + 0.5;

   char *grid = gridref;

   txgeoloc( &lat, &longi, 18, 'A', &grid );
   txgeoloc( &lat, &longi, 10, '0', &grid );
   txgeoloc( &lat, &longi, 24, 'A', &grid );
   txgeoloc( &lat, &longi, 10, '0', &grid );

   *grid = 0;
   return ( GRIDOK );

}
static int locoutput( Location *outgrid )
{
   double lat = 0.0, longi = 0.0;

   int gri = outgrid->dataoffset;
   char *gridref = &outgrid->datastring[ gri ];

   longi = raddeg( outgrid->easting );
   lat = raddeg( outgrid->northing );

   return geotoloc( lat, longi, gridref );
}
static void loccentre( double /*longi*/, double *centre )
{
   *centre = 0.0;
}
/**********************************************************************/
static int geoinput( Location *ingrid )
{
   TEMPBUFF( longbuff, 50 );
   TEMPBUFF( latbuff, 50 );

   int nsign = 1;
   int esign = 1;
   int deg, min;
   float secs;
   int i, j;

   //		fprintf(outfile, "\nLatitude (int int f): ");
   //		getline();

   strupr( ingrid->datastring );

   i = strcspn( ingrid->datastring, "NSEW" );
   if ( i < ingrid->datalength )
      j = strcspn( &ingrid->datastring[ i + 1 ], "NSEW" );
   else
      j = 0;

   char *b1 = 0;
   char *b2 = 0;

   char dsi = ingrid->datastring[ i ];
   char dsj = ingrid->datastring[ i + 1 + j ];

   if ( ( dsi == 'N' ) || ( dsi == 'S' ) )
      b1 = latbuff;
   else
      if ( ( dsi == 'E' ) || ( dsi == 'W' ) )
         b1 = longbuff;

   if ( ( dsj == 'N' ) || ( dsj == 'S' ) )
      b2 = latbuff;
   else
      if ( ( dsj == 'E' ) || ( dsj == 'W' ) )
         b2 = longbuff;

   if ( ( b1 == 0 ) || ( b2 == 0 ) || ( b1 == b2 ) )
      return ( INVALIDGREF );

   if ( ( dsi == 'S' ) || ( dsj == 'S' ) )
      nsign = -1;
   if ( ( dsi == 'W' ) || ( dsj == 'W' ) )
      esign = -1;

   strncpy( b1, ingrid->datastring, i );
   b1[ i ] = 0;
   strncpy( b2, &ingrid->datastring[ i + 1 ], j );
   b2[ i ] = 0;

   deg = 0;
   min = 0;
   secs = 0;

   sscanf( latbuff, "%d %d %f", &deg, &min, &secs );

   if ( deg < 0 )
   {
      nsign = -1;
      deg = -deg;
   }

   ingrid->northing = nsign *
                      degrad( ( secs / 60.0 + min ) / 60.0 + deg );

   //		fprintf(outfile, "\nLongitude (int int f): ");
   //		getline();
   deg = 0;
   min = 0;
   secs = 0;

   sscanf( longbuff, "%d %d %f", &deg, &min, &secs );

   if ( deg < 0 )
   {
      esign = -1;
      deg = -deg;
   }

   ingrid->easting = esign *
                     degrad( ( secs / 60.0 + min ) / 60.0 + deg );

   ingrid->centremeridian = 0;

   if ( abs( ingrid->northing ) > ( pi / 2 + 0.1 ) )
      return ( INVALIDGREF );

   if ( abs( ingrid->easting ) > ( pi + 0.1 ) )
      return ( INVALIDGREF );

   return ( GRIDOK );
}

static int geooutput( Location *outgrid )
{
   int dlng, dlat, mlng, mlat;
   char lngs = 'E', lats = 'N';
   double slng = 0.0, slat = 0.0;

   int gri = outgrid->dataoffset;
   char *gridref = &outgrid->datastring[ gri ];

   if ( outgrid->datalength < 16 )
      return ( INVALIDGREF );
   dms( raddeg( outgrid->northing ), &dlat, &mlat, &slat );
   dms( raddeg( outgrid->easting ), &dlng, &mlng, &slng );
   if ( outgrid->northing < 0 )
   {
      dlat = -dlat;
      lats = 'S';
   }
   if ( outgrid->easting < 0 )
   {
      dlng = -dlng;
      lngs = 'W';
   }
   sprintf( gridref,
            "%2.2d %2.2d %2.4f %c " 		// northing
            "%3.3d %2.2d %2.4f %c",       // easting
            dlat, mlat, slat, lats,
            dlng, mlng, slng, lngs
          );
   return ( GRIDOK );

}
/**********************************************************************/

static int owninput( Location * /*ingrid*/ )
{
   return ( INVALIDGREF );
}

static int ownoutput( Location * /*outgrid*/ )
{
   return ( INVALIDGREF );
}
/**********************************************************************/
static int nulinput( Location * /*ingrid*/ )
{
   return ( INVALIDGREF );

}
static int nuloutput( Location * /*outgrid*/ )
{
   return ( INVALIDGREF );
}

/**********************************************************************/
void dms( double deg, int *d, int *m, double *s )
{
   int sign = 1;

   if ( deg < 0.0 )
   {
      sign = -1;
      deg = -deg;
   }
   *d = ( int ) floor( deg );
   *m = ( int ) floor( ( deg - *d ) * 60.0 );
   *s = 60.0 * ( ( deg - *d ) * 60.0 - *m );

   if ( *s >= 59.00009 )   	// near enough
   {
      *s = 0;
      *m += 1;
   }
   *d *= sign;
}

