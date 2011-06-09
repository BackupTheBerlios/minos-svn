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
#include "reg1test.h"

enum reg1test_order
{
   TName, TdDate, PCall, PWWLo, PExch, PAdr1, PAdr2, PSect, PBand,
   PClub, RName, RCall, RAdr1, RAdr2, RPoCo, RCity, RCoun, RPhon,
   RHBBS, MOpe1, MOpe2, STXEq, SPowe, SRXEq, SAnte, SAntH, CQSOs,
   CQSOP, CWWLs, CWWLB, CExcs, CExcB, CDXCs, CDXCB, CToSc, CODXC,
   LineCount
};

reg1testLine::reg1testLine( const std::string &pre, const std::string &dat ) :
      prefix( pre ), data( dat )
{}
reg1testLine::reg1testLine( void )
{}
reg1testLine::~reg1testLine()
{}
reg1testLine &reg1testLine::operator = ( const reg1testLine &l )
{
   prefix = l.prefix;

   data = l.data;
   return *this;
}

reg1test::reg1test( LoggerContestLog * ct, HANDLE hFile )
      : ct( ct ), regContestFile( hFile ), nextBlock( 0 )
{
}
reg1test::reg1test( LoggerContestLog * const ct )
      : ct( ct ), regContestFile( INVALID_HANDLE_VALUE ), nextBlock( 0 )
{}
reg1test::~reg1test()
{}

bool reg1test::exportTest( HANDLE expfd )
{
   // export current contest as REG1TEST
   writer wr( expfd );
   char lbuff[ 1024 ];

   wr.lwrite( "[REG1TEST;1]" );

   // First the header info
   // calc mult count
   int ltot = 0;
   int nctry = 0;
   int ndistrict = 0;
   int nlocs = 0;

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
   int nvalid = 0;      // valid QSOs
   int nlines = 0;      // QSO records
   for ( unsigned int i = 0; i < ct->ctList.size(); i++ )
   {
      BaseContact *dct = ct->ctList[ i ];
      ContestContact *cct = dynamic_cast<ContestContact *>( dct );
      // Extract comments for "Remarks" section
      cct->addReg1TestComment( remarks );

      if ( cct->contactFlags.getValue() & ( LOCAL_COMMENT | COMMENT_ONLY | DONT_PRINT ) )
         continue;

      nlines++;
      if ( cct->contactScore.getValue() > 0 )
         nvalid++;
   }

   // get the best DX contact
   BaseContact *bestdx = ct->getBestDX();

   /*
   // build the list of main operators
   std::set
      <std::string> op1list;

   for ( LogIterator i = ct->ctList.begin(); i != ct->ctList.end(); i++ )
   {
      op1list.insert( ( *i ) ->op1.getValue() );
   }
   */

   reg1testLine linelist[ ( int ) LineCount ];
   linelist[ ( int ) TName ] = reg1testLine( "TName", ct->name.getValue()  /*, "Contest Name"*/ ),

   linelist[ ( int ) TdDate ] = reg1testLine( "TDate", ct->dateRange( DTGFULL )  /*, "Start Date;End Date"*/ );
   linelist[ ( int ) PCall ] = reg1testLine( "PCall", ct->mycall.fullCall.getValue()  /*, "Callsign Used"*/ );
   linelist[ ( int ) PWWLo ] = reg1testLine( "PWWLo", ct->myloc.loc.getValue()  /*, "Locator Used"*/ );
   linelist[ ( int ) PExch ] = reg1testLine( "PExch", ct->location.getValue()  /*, "Exchange Used"*/ );
   linelist[ ( int ) PAdr1 ] = reg1testLine( "PAdr1", ct->sqth1.getValue()  /*, "Address line 1/2 of station"*/ );
   linelist[ ( int ) PAdr2 ] = reg1testLine( "PAdr2", ct->sqth2.getValue()  /*, "Address line 2/2 of station"*/ );
   linelist[ ( int ) PSect ] = reg1testLine( "PSect", ct->entSect.getValue()  /*, "Section Entered"*/ );
   linelist[ ( int ) PBand ] = reg1testLine( "PBand", ct->band.getValue()  /*, "Band Used"*/ );

   linelist[ ( int ) PClub ] = reg1testLine( "PClub", ct->entrant.getValue()  /*, "Name of club/group"*/ );
   linelist[ ( int ) RName ] = reg1testLine( "RName", ct->entName.getValue()  /*, "Name of responsible operator"*/ );
   linelist[ ( int ) RCall ] = reg1testLine( "RCall", ct->entCall.getValue()  /*, "Callsign of responsible operator"*/ );

   linelist[ ( int ) RAdr1 ] = reg1testLine( "RAdr1", ct->entAddr1.getValue()  /*, "Address line 1/2 of responsible operator"*/ );
   linelist[ ( int ) RAdr2 ] = reg1testLine( "RAdr2", ct->entAddr2.getValue()  /*, "Address line 2/2 of responsible operator"*/ );
   linelist[ ( int ) RCity ] = reg1testLine( "RCity", ct->entCity.getValue()  /*, "City of responsible operator"*/ );
   linelist[ ( int ) RPoCo ] = reg1testLine( "RPoCo", ct->entPostCode.getValue()  /*, "Post Code of responsible operator"*/ );
   linelist[ ( int ) RCoun ] = reg1testLine( "RCoun", ct->entCountry.getValue()  /*, "Country of responsible operator"*/ );
   linelist[ ( int ) RPhon ] = reg1testLine( "RPhon", ct->entPhone.getValue()  /*, "Phone no. of responsible operator"*/ );

   linelist[ ( int ) RHBBS ] = reg1testLine( "RHBBS", ct->entEMail.getValue()  /*, "EMAIL address of responsible operator"*/ );
   linelist[ ( int ) MOpe1 ] = reg1testLine( "MOpe1", ct->ops1.getValue()  /*, "Operators line 1/2"*/ );
   linelist[ ( int ) MOpe2 ] = reg1testLine( "MOpe2", ct->ops2.getValue()  /*, "Operators line 2/2"*/ );
   linelist[ ( int ) STXEq ] = reg1testLine( "STXEq", ct->entTx.getValue()  /*, "TX Equipment"*/ );
   linelist[ ( int ) SPowe ] = reg1testLine( "SPowe", ct->power.getValue()  /*, "TX Power (W)"*/ );
   linelist[ ( int ) SRXEq ] = reg1testLine( "SRXEq", ct->entRx.getValue()  /*, "RX Equipment"*/ );
   linelist[ ( int ) SAnte ] = reg1testLine( "SAnte", ct->entAnt.getValue()  /*, "Antenna"*/ );
   linelist[ ( int ) SAntH ] = reg1testLine( "SAntH", ct->entAGL.getValue() + ";" + ct->entASL.getValue()  /*, "Antenna Height agl;height asl"*/ );
   linelist[ ( int ) CQSOs ] = reg1testLine( "CQSOs", makeStr( nvalid ) + ";1" /*, "Claimed no. of valid QSOs;Band multiplier"*/ );

   linelist[ ( int ) CQSOP ] = reg1testLine( "CQSOP", makeStr( ct->contestScore ) ); /*, "Claimed no. of QSO points"*/
   linelist[ ( int ) CWWLs ] = reg1testLine( "CWWLs", makeStr( nlocs ) + ";0;1" ); /*, "Claimed no. of WWLs;Bonus per new WWL;WWL multiplier"*/
   linelist[ ( int ) CWWLB ] = reg1testLine( "CWWLB", "0" ); /*, "Claimed no. of WWL bonus points"*/
   linelist[ ( int ) CExcs ] = reg1testLine( "CExcs", makeStr( ndistrict ) + ";0;1" ); /*, "Claimed no. of exchanges; Bonus for each new exchange; Exchange Multiplier"*/
   linelist[ ( int ) CExcB ] = reg1testLine( "CExcB", "0" ); /*, "Claimed no. of exchange bonus points"*/
   linelist[ ( int ) CDXCs ] = reg1testLine( "CDXCs", makeStr( nctry ) + ";0;1" ); /*, "Claimed no. of DXCCs; Bonus for each new DXCC;DXCC multiplier"*/
   linelist[ ( int ) CDXCB ] = reg1testLine( "CDXCB", "0" ); /*, "Claimed no of DXCC bonus points"*/
   linelist[ ( int ) CToSc ] = reg1testLine( "CToSc", makeStr( ct->contestScore * ltot ) ); /*, "Claimed total score"*/
   linelist[ ( int ) CODXC ] = reg1testLine( "CODXC", bestdx ? ( ( bestdx->cs.fullCall.getValue() + ";" + bestdx->loc.loc.getValue() + ";" + makeStr( bestdx->contactScore.getValue() ).c_str() ).c_str() ) : ";;" ); /*, "(Best DX) Callsign; Locator; Distance"*/

   for ( int i = 0; i < LineCount; i++ )
   {
      sprintf( lbuff, "%s=%s", linelist[ i ].prefix.c_str(), linelist[ i ].data.c_str() );
      wr.lwrite( lbuff );
   }
   /*
      [Remarks]Remarks identifier
      F Remarks lines
   */
   wr.lwrite( "[Remarks]" );

   for ( unsigned int i = 0; i < remarks.size(); i++ )
   {
      wr.lwrite( remarks[ i ].c_str() );
   }

   TMyRCVersion RCVersion;
   RCVersion.initialise();
   String ProductName = RCVersion.ProductName;
   String Version = RCVersion.FileVersion;


   /*
      [QSORecords;Number of QSO records following]QSO records identifier;number of QSO records following
   */
   sprintf( lbuff, "[QSORecords;%d]", nlines );
   wr.lwrite( lbuff );
   // and then the contact list

   for ( unsigned int i = 0; i < ct->ctList.size(); i++ )
   {
      BaseContact *dct = ct->ctList[ i ];
      ContestContact *cct = dynamic_cast<ContestContact *>( dct );

      if ( cct->contactFlags.getValue() & ( LOCAL_COMMENT | COMMENT_ONLY | DONT_PRINT ) )
      {
         // should all COMMENT_ONLY records go into remarks?
         // and also comments on individual QSOs?
         continue;
      }

      std::string sbuff;
      cct->getReg1TestText( sbuff );   // lbuff if "bsize" = 256

      if ( sbuff.length() == 0 )
         continue;

      sbuff = trimr( sbuff );			// No need to pad to 250!!
      wr.lwrite( sbuff.c_str() );
   }
   // [END] isn't a part of the Reg1Test spec, but everyone else seems to have it!
   // Adjudication software copes either way round.
   
   String pver = "[END;" + ProductName + " by G0GJV, version " + Version + "]";
   wr.lwrite( pver.c_str() );

   return true;

}
//====================================================================
bool reg1test::parseHeader( String line )
{
   // break line at semicolons
   char buff[ 1024 ];
   strncpy( buff, line.c_str(), 1023 );
   buff[ 1023 ] = 0;
   bool sep2seen;
   char *a[ 2 ];
   char *b[ 3 ];
   int scnt = parseLine( buff, '=', a, 2, 0, sep2seen );   // BAD - parseLine writes to buffer

   if ( scnt != 1 )
   {
      return false;
   }

   // now work through the possible header strings
   String code = String( a[ 0 ] ).Trim().UpperCase();
   if ( code == "TNAME" )
   {
      //TName=VHFNFD
      ct->name.setValue( a[ 1 ] );
   }
   else
      if ( code == "PCALL" )
      {
         //PCall=G4RFR/P
         ct->mycall = callsign( strupr( a[ 1 ] ) );
         ct->mycall.valRes = CS_NOT_VALIDATED;
         ct->mycall.validate();
      }
      else
         if ( code == "PWWLO" )
         {
            //PWWLo=IO80ST
            ct->myloc.loc.setValue( a[ 1 ] );
            ct->validateLoc();
         }
         else
            if ( code == "PEXCH" )
            {
               //PExch=
               ct->location.setValue( a[ 1 ] );
            }
            else
               if ( code == "PADR1" )
               {
                  //PAdr1=Little Minterne Hill
                  ct->sqth1.setValue( a[ 1 ] );
               }
               else
                  if ( code == "PADR2" )
                  {
                     //PAdr2=Near Cerne Abbas
                     ct->sqth2.setValue( a[ 1 ] );
                  }
                  else
                     if ( code == "PSECT" )
                     {
                        //PSect=OPEN
                        ct->entSect.setValue( a[ 1 ] );
                     }
                     else
                        if ( code == "PBAND" )
                        {
                           //PBand=145 MHz
                           ct->band.setValue( a[ 1 ] );
                        }
                        else
                           if ( code == "PCLUB" )
                           {
                              //PClub=Bracknell and Flight Refuelling
                              ct->entrant.setValue( a[ 1 ] );
                           }
                           else
                              if ( code == "RNAME" )
                              {
                                 //RName=M. J. Goodey
                                 ct->entName.setValue( a[ 1 ] );
                              }
                              else
                                 if ( code == "RCALL" )
                                 {
                                    //RCall=G0GJV
                                    ct->entCall.setValue( a[ 1 ] );
                                 }
                                 else
                                    if ( code == "RADR1" )
                                    {
                                       //RAdr1=62, Rose Hill
                                       ct->entAddr1.setValue( a[ 1 ] );
                                    }
                                    else
                                       if ( code == "RADR2" )
                                       {
                                          //RAdr2=Binfield
                                          ct->entAddr2.setValue( a[ 1 ] );
                                       }
                                       else
                                          if ( code == "RPOCO" )
                                          {
                                             //RPoCo=RG42 5LG
                                             ct->entPostCode.setValue( a[ 1 ] );
                                          }
                                          else
                                             if ( code == "RCITY" )
                                             {
                                                //RCity=Bracknell
                                                ct->entCity.setValue( a[ 1 ] );
                                             }
                                             else
                                                if ( code == "RCOUN" )
                                                {
                                                   //RCoun=UK
                                                   ct->entCountry.setValue( a[ 1 ] );
                                                }
                                                else
                                                   if ( code == "RPHON" )
                                                   {
                                                      //RPhon=01344 428614
                                                      ct->entPhone.setValue( a[ 1 ] );
                                                   }
                                                   else
                                                      if ( code == "RHBBS" )
                                                      {
                                                         //RHBBS=g0gjv@goodey.org.uk
                                                         ct->entEMail.setValue( a[ 1 ] );
                                                      }
                                                      else
                                                         if ( code == "MOPE1" )
                                                         {
                                                            //MOpe1=J.Smith (G0NZO); A.J.Baker (G3PFM); J.Fell (G0API); J. Hart (G4POF)
                                                            ct->ops1.setValue( a[ 1 ] );
                                                         }
                                                         else
                                                            if ( code == "MOPE2" )
                                                            {
                                                               //MOpe2=
                                                               ct->ops2.setValue( a[ 1 ] );
                                                            }
                                                            else
                                                               if ( code == "STXEQ" )
                                                               {
                                                                  //STXEq=IC202 + 2x4CX250 PA
                                                                  ct->entTx.setValue( a[ 1 ] );
                                                               }
                                                               else
                                                                  if ( code == "SPOWE" )
                                                                  {
                                                                     //SPowe=350
                                                                     ct->power.setValue( a[ 1 ] );
                                                                  }
                                                                  else
                                                                     if ( code == "SRXEQ" )
                                                                     {
                                                                        //SRXEq=IC202
                                                                        ct->entRx.setValue( a[ 1 ] );
                                                                     }
                                                                     else
                                                                        if ( code == "SANTE" )
                                                                        {
                                                                           //SAnte=2 X 14 ele
                                                                           ct->entAnt.setValue( a[ 1 ] );
                                                                        }
                                                                        else
                                                                           if ( code == "SANTH" )
                                                                           {
                                                                              //SAntH=16;250
                                                                              scnt = parseLine( a[ 1 ], ';', b, 3, 0, sep2seen );   // BAD - parseLine writes to buffer
                                                                              if ( scnt == 2 )
                                                                              {
                                                                                 ct->entAGL.setValue( b[ 0 ] );
                                                                                 ct->entASL.setValue( b[ 1 ] );
                                                                              }
                                                                           }

   // These could be useful as  across-check?

                                                                           else
                                                                              if ( code == "CQSOS" )
                                                                              {
                                                                                 // Claimed no. of valid QSOs;Band multiplier
                                                                                 //CQSOs=354;1
                                                                              }
                                                                              else
                                                                                 if ( code == "CQSOP" )
                                                                                 {
                                                                                    // Claimed no. of QSO points
                                                                                    //CQSOP=93471
                                                                                 }
                                                                                 else
                                                                                    if ( code == "CWWLS" )
                                                                                    {
                                                                                       // Claimed no. of WWLs;Bonus per new WWL;WWL multiplier
                                                                                       //CWWLs=0;0;1
                                                                                       if ( atoi( b[ 0 ] ) != 0 )
                                                                                       {
                                                                                          ct->locMult.setValue( true );
                                                                                       }
                                                                                    }
                                                                                    else
                                                                                       if ( code == "CWWLB" )
                                                                                       {
                                                                                          // Claimed no. of WWL bonus points
                                                                                          //CWWLB=0
                                                                                       }
                                                                                       else
                                                                                          if ( code == "CEXCS" )
                                                                                          {
                                                                                             // Claimed no. of exchanges; Bonus for each new exchange; Exchange Multiplier
                                                                                             //CExcs=0;0;1
                                                                                             if ( atoi( b[ 0 ] ) != 0 )
                                                                                             {
                                                                                                ct->districtMult.setValue( true );
                                                                                             }
                                                                                          }
                                                                                          else
                                                                                             if ( code == "CEXCB" )
                                                                                             {
                                                                                                // Claimed no. of exchange bonus points
                                                                                                //CExcB=0
                                                                                             }
                                                                                             else
                                                                                                if ( code == "CDXCS" )
                                                                                                {
                                                                                                   // Claimed no. of DXCCs; Bonus for each new DXCC;DXCC multiplier
                                                                                                   //CDXCs=0;0;1
                                                                                                   if ( atoi( b[ 0 ] ) != 0 )
                                                                                                   {
                                                                                                      ct->countryMult.setValue( true );
                                                                                                   }
                                                                                                }
                                                                                                else
                                                                                                   if ( code == "CDXCB" )
                                                                                                   {
                                                                                                      // Claimed no of DXCC bonus points
                                                                                                      //CDXCB=0
                                                                                                   }
                                                                                                   else
                                                                                                      if ( code == "CTOSC" )
                                                                                                      {
                                                                                                         // Claimed total score
                                                                                                         //CToSc=93471
                                                                                                      }
                                                                                                      else
                                                                                                         if ( code == "CODXC" )
                                                                                                         {
                                                                                                            // (Best DX) Callsign; Locator; Distance
                                                                                                            //CODXC=HB9CC;JN47RJ;946
                                                                                                         }
                                                                                                         else
                                                                                                            if ( code == "TDATE" )
                                                                                                            {
                                                                                                               // Start Date;End Date
                                                                                                               // derived
                                                                                                               //TDate=20060701;20060702
                                                                                                            }
                                                                                                            else
                                                                                                            {
                                                                                                               // unknown
                                                                                                               return false;
                                                                                                            }
   return true;
}
//====================================================================
bool reg1test::parseRemark( String line )
{
   // free form lines
   remarks.push_back( line.c_str() );
   return true;
}
//====================================================================
bool reg1test::parseQSO( String line )
{
   // break line at semicolons
   //060701;1408;M3MQR;;59;001;59;001;;IO80OU;24;;;;
   char * a[ 20 ];
   char buff[ 1024 ];
   strncpy( buff, line.c_str(), 1023 );
   buff[ 1023 ] = 0;
   bool sep2seen;
   int scnt = parseLine( buff, ';', a, 20, 0, sep2seen );   // BAD - parseLine writes to buffer
   if ( scnt >= 14 )
   {
      DisplayContestContact * aqso = 0;
      ct->makeContact( false, ( BaseContact * ) aqso );
      /*
       0  Date(6);
       1  Time(4);
       2  Call(3 - 14);
       3  Mode code(1 below);
       4  Sent-RST(0 or 2 or 3);
       5  Sent QSO number(0 or 3 or 4, with leading zeroes);
       6  Received-RST(0 or 2 or 3);
       7  Received QSO number(0 or 3 or 4, with leading zeroes);
       8  Received exchange(0 or 1-6);
       9  Received-WWL(0 or 4 or 6);
       10  QSO-Points(1-6 inc band mult);
       11  New-Exchange-(N)(0 or 1);
       12  New-WWL-(N)(0 or 1);
       13  New-DXCC-(N)(0 or 1);
       14  Duplicate-QSO-(D)(0 or 1)

         (N) says either "N;" or ";"
      */
      aqso->time.setDate( a[ 0 ], DTGReg1Test );
      aqso->time.setTime( a[ 1 ], DTGReg1Test );
      aqso->cs = callsign( strupr( a[ 2 ] ) );
      aqso->cs.valRes = CS_NOT_VALIDATED;
      // a[3] mode
      aqso->reps.setValue( a[ 4 ] );
      aqso->serials.setValue( a[ 5 ] ); // not string, int

      int maxct = atoi( aqso->serials.getValue().c_str() );
      if ( maxct > ct->maxSerial )
         ct->maxSerial = maxct;

      aqso->repr.setValue( a[ 6 ] );
      aqso->serialr.setValue( a[ 7 ] ); // not string, int
      aqso->extraText.setValue( a[ 8 ] );     // or district code if postcode mults
      aqso->loc.loc.setValue( a[ 9 ] );

      aqso->loc.valRes = LOC_NOT_VALIDATED;
      aqso->contactScore.setValue( atoi( a[ 10 ] ) );

      if ( atoi( a[ 10 ] ) == 0 || stricmp( a[ 14 ], "D" ) == 0 )
         aqso->contactFlags.setValue( NON_SCORING );

      nextBlock++;
      aqso->setLogSequence( nextBlock << 16 );
      ct->ctList.insert( aqso );
      return true;
   }
   return false;
}
//====================================================================
bool reg1test::importReg1test()
{
   //Create and Open File Stream
   try
   {
      THandleStream * hs = new THandleStream( ( int ) regContestFile );
      TStringList *sl = new TStringList;
      sl->LoadFromStream( hs );

      if ( !sl->Count )
      {
         return false;
      }

      enum reg1Stage {sStart, sHeader, sRemarks, sQSOs} stage = sStart;
      int nq = 0;
      int qcount = 0;
      // and here wade our way through
      for ( int lineno = 0; lineno < sl->Count; lineno++ )
      {
         String line = sl->Strings[ lineno ].Trim();

         if ( line.Length() == 0 )
         {
            continue;
         }
         if ( stage == sStart && line == "[REG1TEST;1]" )
         {
            stage = sHeader;
            continue;
         }
         if ( stage == sHeader && line == "[Remarks]" )
         {
            stage = sRemarks;
            continue;
         }
         if ( stage == sRemarks && line.SubString( 1, 12 ) == "[QSORecords;" )
         {
            for ( int i = remarks.size(); i < 4; i++ )
            {
               remarks.push_back( "" );
            }
            ct->entCondx1.setValue( remarks[ 0 ] );
            ct->entCondx2.setValue( remarks[ 1 ] );
            ct->entCondx3.setValue( remarks[ 2 ] );
            ct->entCondx4.setValue( remarks[ 3 ] );

            stage = sQSOs;
            nextBlock = 0;
            int endq = line.Pos( "]" );
            if ( endq == 0 )
            {
               return false;
            }
            String snq = line.SubString( 13, endq - 13 );
            nq = snq.ToIntDef( 0 );
            if ( !nq )
            {
               return false;
            }
            continue;
         }
         switch ( stage )
         {
            case sStart:
               {
                  // wait for the start - in case we have an email lead in
               }
               break;

            case sHeader:
               {
                  // parse out the header line and fill in the contest details
                  if ( !parseHeader( line ) )
                     return false;
               }
               break;

            case sRemarks:
               {
                  // read the remarks lines while they still come...
                  if ( !parseRemark( line ) )
                     return false;
               }
               break;

            case sQSOs:
               {
                  // parse out the QSO
                  if ( !parseQSO( line ) )
                  {
                     return false;
                  }
                  qcount++;
                  if ( nq == qcount )
                  {
                     return true;
                  }

               }
               break;

            default:     // there shouldn't be any others
               break;
         }
      }
      delete sl;
   }
   catch ( ... )
   {
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------
/*static*/bool reg1test::doImportReg1test(  LoggerContestLog * c,  HANDLE hFile )
{
   if ( !c )
      return false;

   reg1test rimp( c, hFile );

   return rimp.importReg1test();
}
//====================================================================

