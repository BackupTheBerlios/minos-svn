/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop
#include "LoggerContest.h"

#include "RCVersion.h"
#include "PrintFile.h"

enum PrintFile_order
{
   TName, TdDate, PCall, PWWLo, PExch, PAdr1, PAdr2, PSect, PBand,
   PClub, RName, RCall, RAdr1, RAdr2, RPoCo, RCity, RCoun, RPhon,
   RHBBS, MOpe1, MOpe2, STXEq, SPowe, SRXEq, SAnte, SAntH, CVQSO, CQSOs,
   CQSOP, CWWLs, /*CWWLB,*/ CExcs, /*CExcB,*/ CDXCs, /*CDXCB,*/ CToSc, CODXC,
   LineCount
};
// We want to export as a human readable file
PrintFileLine::PrintFileLine( const std::string &pre, const std::string &dat ) :
      prefix( pre ), data( dat )
{}
PrintFileLine::PrintFileLine( void )
{}
PrintFileLine::~PrintFileLine()
{}
PrintFileLine &PrintFileLine::operator = ( const PrintFileLine &l )
{
   prefix = l.prefix;

   data = l.data;
   return *this;
}

PrintFile::PrintFile( LoggerContestLog * const ct )
      : ct( ct )
{}
PrintFile::~PrintFile()
{}

bool PrintFile::exportTest( HANDLE expfd )
{
   PrintFileLine linelist[ ( int ) LineCount ];
   // export current contest as PrintFile
   writer wr( expfd );
   char lbuff[ 1024 ];

   // First the header info
   // calc mult count
   int ltot = 0;
   int nctry = 0;
   int ndistrict = 0;
   int nlocs = 0;
   int nvalid = 0;

   if ( ct->countryMult.getValue() )
   {
      ltot += ct->nctry ;
      nctry = ct->nctry;
   }
   if ( ct->districtMult.getValue() )
   {
      ltot += ct->ndistrict;
      ndistrict = ct->ndistrict;
   }
   if ( ct->locMult.getValue() )
   {
      ltot += ct->nlocs;
      nlocs = ct->nlocs;
   }

   if ( ltot == 0 )
      ltot = 1;

   std::vector <std::string> remarks;
   remarks.push_back( ct->entCondx1.getValue() );
   remarks.push_back( ct->entCondx2.getValue() );
   remarks.push_back( ct->entCondx3.getValue() );
   remarks.push_back( ct->entCondx4.getValue() );

   for ( unsigned int i = 0; i < ct->ctList.size(); i++ )
   {
      BaseContact *dct = ct->ctList[ i ];
      ContestContact *cct = dynamic_cast<ContestContact *>( dct );

      if ( cct->contactScore.getValue() > 0 )
         nvalid++;
   }

   // get the best DX contact
   BaseContact *bestdx = ct->getBestDX();

   linelist[ ( int ) TName ] = PrintFileLine( "Contest Name                            ", ct->name.getValue()  /*, "Contest Name"*/ ),

   linelist[ ( int ) TdDate ] = PrintFileLine("Start Date;End Date                     ", ct->dateRange( DTGFULL )  /*, "Start Date;End Date"*/ );
   linelist[ ( int ) PCall ] = PrintFileLine( "Callsign Used                           ", ct->mycall.fullCall.getValue()  /*, "Callsign Used"*/ );
   linelist[ ( int ) PWWLo ] = PrintFileLine( "Locator Used                            ", ct->myloc.loc.getValue()  /*, "Locator Used"*/ );
   linelist[ ( int ) PExch ] = PrintFileLine( "Exchange Used                           ", ct->location.getValue()  /*, "Exchange Used"*/ );
   linelist[ ( int ) PAdr1 ] = PrintFileLine( "Address line 1/2                        ", ct->sqth1.getValue()  /*, "Address line 1/2 of station"*/ );
   linelist[ ( int ) PAdr2 ] = PrintFileLine( "Address line 2/2                        ", ct->sqth2.getValue()  /*, "Address line 2/2 of station"*/ );
   linelist[ ( int ) PSect ] = PrintFileLine( "Section Entered                         ", ct->entSect.getValue()  /*, "Section Entered"*/ );
   linelist[ ( int ) PBand ] = PrintFileLine( "Band Used                               ", ct->band.getValue()  /*, "Band Used"*/ );

   linelist[ ( int ) PClub ] = PrintFileLine( "Name of club/group                      ", ct->entrant.getValue()  /*, "Name of club/group"*/ );
   linelist[ ( int ) RName ] = PrintFileLine( "Name of responsible operator            ", ct->entName.getValue()  /*, "Name of responsible operator"*/ );
   linelist[ ( int ) RCall ] = PrintFileLine( "Callsign of responsible operator        ", ct->entCall.getValue()  /*, "Callsign of responsible operator"*/ );

   linelist[ ( int ) RAdr1 ] = PrintFileLine( "Address line 1/2 of responsible operator", ct->entAddr1.getValue()  /*, "Address line 1/2 of responsible operator"*/ );
   linelist[ ( int ) RAdr2 ] = PrintFileLine( "Address line 2/2 of responsible operator", ct->entAddr2.getValue()  /*, "Address line 2/2 of responsible operator"*/ );
   linelist[ ( int ) RCity ] = PrintFileLine( "City of responsible operator            ", ct->entCity.getValue()  /*, "City of responsible operator"*/ );
   linelist[ ( int ) RPoCo ] = PrintFileLine( "Post Code of responsible operator       ", ct->entPostCode.getValue()  /*, "Post Code of responsible operator"*/ );
   linelist[ ( int ) RCoun ] = PrintFileLine( "Country of responsible operator         ", ct->entCountry.getValue()  /*, "Country of responsible operator"*/ );
   linelist[ ( int ) RPhon ] = PrintFileLine( "Phone no. of responsible operator       ", ct->entPhone.getValue()  /*, "Phone no. of responsible operator"*/ );

   linelist[ ( int ) RHBBS ] = PrintFileLine( "EMAIL address of responsible operator   ", ct->entEMail.getValue()  /*, "EMAIL address of responsible operator"*/ );
   linelist[ ( int ) MOpe1 ] = PrintFileLine( "Operators line 1/2                      ", ct->ops1.getValue()  /*, "Operators line 1/2"*/ );
   linelist[ ( int ) MOpe2 ] = PrintFileLine( "Operators line 2/2                      ", ct->ops2.getValue()  /*, "Operators line 2/2"*/ );
   linelist[ ( int ) STXEq ] = PrintFileLine( "TX Equipment                            ", ct->entTx.getValue()  /*, "TX Equipment"*/ );
   linelist[ ( int ) SPowe ] = PrintFileLine( "TX Power (W)                            ", ct->power.getValue()  /*, "TX Power (W)"*/ );
   linelist[ ( int ) SRXEq ] = PrintFileLine( "RX Equipment                            ", ct->entRx.getValue()  /*, "RX Equipment"*/ );
   linelist[ ( int ) SAnte ] = PrintFileLine( "Antenna                                 ", ct->entAnt.getValue()  /*, "Antenna"*/ );
   linelist[ ( int ) SAntH ] = PrintFileLine( "Antenna Height agl;height asl           ", ct->entAGL.getValue() + ";" + ct->entASL.getValue()  /*, "Antenna Height agl;height asl"*/ );

   linelist[ ( int ) CVQSO ] = PrintFileLine( "no. of valid QSOs                       ", makeStr( nvalid ) ); /*, "Claimed no. of QSO points"*/
   linelist[ ( int ) CQSOP ] = PrintFileLine( "Claimed no. of QSO points               ", makeStr( ct->contestScore ) ); /*, "Claimed no. of QSO points"*/
   linelist[ ( int ) CWWLs ] = PrintFileLine( "Claimed no. of WWLs                     ", makeStr( nlocs ) ); /*, "Claimed no. of WWLs;Bonus per new WWL;WWL multiplier"*/
   linelist[ ( int ) CExcs ] = PrintFileLine( "Claimed no. of exchanges                ", makeStr( ndistrict )); /*, "Claimed no. of exchanges; Bonus for each new exchange; Exchange Multiplier"*/
   linelist[ ( int ) CDXCs ] = PrintFileLine( "Claimed no. of DXCCs                    ", makeStr( nctry )); /*, "Claimed no. of DXCCs; Bonus for each new DXCC;DXCC multiplier"*/
   linelist[ ( int ) CToSc ] = PrintFileLine( "Claimed total score                     ", makeStr( ct->contestScore * ltot ) ); /*, "Claimed total score"*/
   linelist[ ( int ) CODXC ] = PrintFileLine( "(Best DX) Callsign; Locator; Distance   ", bestdx ? ( ( bestdx->cs.fullCall.getValue() + ";" + bestdx->loc.loc.getValue() + ";" + makeStr( bestdx->contactScore.getValue() ).c_str() ).c_str() ) : ";;" ); /*, "(Best DX) Callsign; Locator; Distance"*/

   for ( int i = 0; i < LineCount; i++ )
   {
      if (linelist[ i ].prefix.size())
      {
         sprintf( lbuff, "%s : %s", linelist[ i ].prefix.c_str(), linelist[ i ].data.c_str() );
         wr.lwrite( lbuff );
      }
   }
   wr.lwrite( "" );
   wr.lwrite( "[Remarks]" );

   for ( unsigned int i = 0; i < remarks.size(); i++ )
   {
      wr.lwrite( remarks[ i ].c_str() );
   }

   TMyRCVersion RCVersion;
   RCVersion.initialise();
   String ProductName = RCVersion.ProductName;
   String Version = RCVersion.FileVersion;

   String pver = "Produced by " + ProductName + " version " + Version;
   wr.lwrite( pver.c_str() );
   wr.lwrite( "" );

   // and then the contact list

   for ( unsigned int i = 0; i < ct->ctList.size(); i++ )
   {
      BaseContact *dct = ct->ctList[ i ];
      ContestContact *cct = dynamic_cast<ContestContact *>( dct );

      if ( cct->contactFlags.getValue() & ( LOCAL_COMMENT | DONT_PRINT ) )
      {
         // should all COMMENT_ONLY records go into remarks?
         // and also comments on individual QSOs?
         continue;
      }

      std::string sbuff;

      cct->getPrintFileText( sbuff, 120 );   // lbuff if "bsize" = 256
      if ( sbuff.length() != 0 )
      {
         sbuff = trimr( sbuff );			// No need to pad to 250!!
         wr.lwrite( sbuff.c_str() );
      }
   }

   return true;

}
//====================================================================

