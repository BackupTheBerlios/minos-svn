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

extern const double pi = ( double ) 3.141592653 ;  /* pi */
extern const double dr = pi / 180.0;      // degree to radian conversion factor

/******************************************************/
/*                                                    */
/* convert Maidenhead locator to longtitude and       */
/* latitude                                           */
/*                                                    */
/* returns 0 if OK, else 1 if source locator in error */
/*                                                    */
/* Latitude is NS, Longtitude is EW                   */
/*                                                    */
/******************************************************/
char lonlat( const std::string &inqra, double &lon, double &lat )
{
   int l ;                       /* length of qra string */
   double ft[ 8 ] = {0.0, 0.0, 5.0, 5.0, 11.0, 11.0, 5.0, 5.0} ; /* work values for locator positions */
   // defaulted to square centres - LL55LL55
   double wfe = 0.0 ;                  /* work values for longtitude */
   double wfe1 = 0.0 ;
   double wfe2 = 0.0 ;
   double wfn = 0.0 ;                  /* work values for latitude */
   double wfn1 = 0.0 ;
   double wfn2 = 0.0 ;
   const char *wp1 ;                   /* work pointers in strings */
   const char *wp2 ;
   const char *wp3 ;
   double *fp1 ;

   for ( l = inqra.length() - 1; l >= 0; l-- )
   {
      if ( inqra[ l ] != ' ' )
         break;
   }
   if ( l <= 0 )
      return ( ERR_NOLOC );

   char locres = LOC_OK;

   l++;

   /* Process Maidenhead locator */
   if ( l <= 8 && l >= 2 )      /* maidenhead locator */
   {
      char * lowval = "AA00AA00";   // make sure this is all after the
      char *hival = "RR99XX99";    // length check

      TEMPBUFF( qra, 10 );
      strncpy( qra, inqra.c_str(), l );	// make sure it doesn't overrun
      qra[ l ] = 0;
      strupr( qra );

      wp1 = qra ;     /* set to front of source */

      /* set up for conversion */
      wp2 = lowval ;                   /* front of lowest value validation */
      wp3 = hival ;                    /* front for highest value validation */
      fp1 = ft ;                       /* set to front of converted table */
      /* validate */
      int i = 0;
      while ( *wp1 && ( i++ < l ) )                     /* check whole locator */
      {
         if ( *wp1 == ' ' )
         {
            if ( i <= 2 )
               return ERR_LOC_RANGE;
            locres = LOC_PARTIAL;

            fp1++;	// already set to centre
            wp1++;
            wp2++;
            wp3++;
         }
         else
         {
            if ( *wp1 < *wp2 || *wp1 > *( wp3++ ) )     /* out of range */
               return ( ERR_LOC_RANGE );                 /* invalid locator */
            else
               *( fp1++ ) = ( double ) ( *( wp1++ ) - ( double ) * ( wp2++ ) ); /* convert to double */
         }

      }

      // NB for short locs we have already set up ft

      /* calculate longtitude and latitude from maidenhead locator*/
      wfe1 = ft[ 0 ] * ( double ) 20.0 + ft[ 2 ] * ( double ) 2.0 - ( double ) 180.0 ;
      wfe2 = ft[ 4 ] / ( double ) 12.0 + ft[ 6 ] / 120.0;
      wfe = wfe1 + wfe2 /*+ (double )1.0 / (double )240.0 */;

      wfn1 = ft[ 1 ] * ( double ) 10.0 + ft[ 3 ] - ( double ) 90.0 ;
      wfn2 = ft[ 5 ] / ( double ) 24.0 + ft[ 7 ] / 240.0;
      wfn = wfn1 + wfn2 /*+ (double )1.0 / (double )480.0 */;
      lat = wfn ;                  /* set up latitude */
      lon = wfe ;                  /* set up longtitude */
   }
   else                                /* invalid locator */
      return ( ERR_LOC_RANGE );                       /* so bug out */

   /* clean exit */
   lon = lon * dr ;                /* hold ready for calculation */
   lat = lat * dr ;

   bool allowLoc4 = MinosParameters::getMinosParameters() ->getAllowLoc4();
   bool allowLoc8 = MinosParameters::getMinosParameters() ->getAllowLoc8();

   if ( l == 4 && allowLoc4 )
   {
      return locres;
   }
   if ( l == 8 && allowLoc8 )
   {
      return locres;
   }
   if ( l == 6 )
   {
      return locres;
   }
   return LOC_PARTIAL;
}

std::string WtodbW( const std::string &W )
{
   std::string res;
   double watts = atof( W.c_str() );
   if ( watts != 0 )
   {
      double dbWatts = 10.0 * log10( watts );
      res = ( boost::format( "%.2f%" ) % dbWatts ).str();
   }
   return res;
}

std::string dbWtoW( const std::string &dbW )
{
   double dbWatts = atof( dbW.c_str() );
   double watts = pow( 10.0, dbWatts / 10.0 ) + 0.05;
   std::string res = ( boost::format( "%.2f%" ) % watts ).str();
   return res;
}

int normBrg( int mbear )
{
   if ( mbear < 0 )
      mbear += 360;
   if ( mbear > 360 )
      mbear -= 360;
   return mbear;
}
int varBrg( int mbear )
{
   mbear += MinosParameters::getMinosParameters() ->getMagneticVariation();
   return normBrg( mbear );
}

