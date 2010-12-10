#pragma link "CompDisp"
/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "display_pch.h"
#pragma hdrstop
#include "MinosVer.h"   // updated by SubWCRev

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "StatsDisp"
#pragma resource "*.dfm"
const CONTINENTS = 6;
struct ContList
{
   char continent[ 3 ];
   bool allow;
};

extern ContList contlist[ CONTINENTS ];
ContList contlist[ CONTINENTS ] =
   {
      {"EU", true},
      {"AS", false},
      {"AF", false},
      {"OC", false},
      {"SA", false},
      {"NA", false},
   };
bool showWorked = false;
bool showUnworked = false;
void TMultDispFrame::initFilters()
{
   filterClickEnabled = false;

   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowContinentEU, contlist[ 0 ].allow );
   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowContinentAS, contlist[ 1 ].allow );
   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowContinentAF, contlist[ 2 ].allow );
   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowContinentOC, contlist[ 3 ].allow );
   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowContinentSA, contlist[ 4 ].allow );
   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowContinentNA, contlist[ 5 ].allow );

   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowWorked, showWorked );
   MinosParameters::getMinosParameters() ->getBoolDisplayProfile( edpShowUnworked, showUnworked );

   ContEU->Checked = contlist[ 0 ].allow;
   ContAS->Checked = contlist[ 1 ].allow;
   ContAF->Checked = contlist[ 2 ].allow;
   ContOC->Checked = contlist[ 3 ].allow;
   ContSA->Checked = contlist[ 4 ].allow;
   ContNA->Checked = contlist[ 5 ].allow;
   WorkedCB->Checked = showWorked;
   UnworkedCB->Checked = showUnworked;

   filterClickEnabled = true;
}
void TMultDispFrame::saveFilters()
{
   contlist[ 0 ].allow = ContEU->Checked;
   contlist[ 1 ].allow = ContAS->Checked;
   contlist[ 2 ].allow = ContAF->Checked;
   contlist[ 3 ].allow = ContOC->Checked;
   contlist[ 4 ].allow = ContSA->Checked;
   contlist[ 5 ].allow = ContNA->Checked;
   showWorked = WorkedCB->Checked;
   showUnworked = UnworkedCB->Checked;

   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowContinentEU, contlist[ 0 ].allow );
   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowContinentAS, contlist[ 1 ].allow );
   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowContinentAF, contlist[ 2 ].allow );
   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowContinentOC, contlist[ 3 ].allow );
   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowContinentSA, contlist[ 4 ].allow );
   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowContinentNA, contlist[ 5 ].allow );

   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowWorked, showWorked );
   MinosParameters::getMinosParameters() ->setBoolDisplayProfile( edpShowUnworked, showUnworked );
   MinosParameters::getMinosParameters() ->flushDisplayProfile();
}
//---------------------------------------------------------------------------
__fastcall TMultDispFrame::TMultDispFrame( TComponent* Owner )
      : TFrame( Owner ), filterClickEnabled( false ), ct( 0 ),
      EL_ShowErrorList ( EN_ShowErrorList, & ShowErrorList_Event ),
      EL_ValidateError ( EN_ValidateError, & ValidateError_Event )
{
   Parent = ( TWinControl * ) Owner;               // This makes the JEDI splitter work!
   //   CountryMultTree->Canvas->Font->Assign(Parent->Font);
   //   DistrictMultTree->Canvas->Font->Assign(Parent->Font);


	WFilterSplitter = new TWhisperSplitter(FilterSplitter, FilterPanel);
	WFilterSplitter->Bitmap = Splitter_Image->Picture->Bitmap;
	WFilterSplitter->HighlightColor = clSkyBlue;

   WFilterSplitter->Minimize();

#ifndef BETA_VERSION
   CompSheet->TabVisible = false;
#endif
}
void TMultDispFrame::setContest( BaseContestLog *pct )
{
   ct = pct;
   StatsDispFrame->setContest( ct );
   CompFrame->setContest( ct );
   initFilters();
   reInitialiseCountries();
   reInitialiseDistricts();
   reInitialiseLocators();
   reInitialiseStats();
   reInitialiseComp();
}
//---------------------------------------------------------------------------
static GridColumn CountryTreeColumns[ ectMultMaxCol ] =
   {
      GridColumn( ectCall, "XXXXXX", "Call", taLeftJustify ),
      GridColumn( ectWorked, "Wk CtX", "Wkd", taCenter ),
      GridColumn( ectLocator, "MM00MM00", "Locator", taLeftJustify ),
      GridColumn( ectBearing, "BRGXXX", "brg", taCenter ),
      GridColumn( ectName, "This is a very long country", "Country", taLeftJustify ),
      GridColumn( ectOtherCalls, "This is a very very very very long country name", "Other calls", taLeftJustify /*taRightJustify*/ )
   };
void TMultDispFrame::reInitialiseCountries()
{
   CountryMultTree->RootNodeCount = 0;
   if ( !ct )
   {
      // clear down
      return ;
   }
   CountryMultTree->BeginUpdate();

   CountryMultTree->RootNodeCount = MultLists::getMultLists() ->getCtryListSize();

   CountryMultTree->Header->Columns->Clear();
   CountryMultTree->Margin = 0;
   CountryMultTree->TextMargin = 4;
   for ( int i = 0; i < ectMultMaxCol; i++ )
   {
      TVirtualTreeColumn *NewColumn = CountryMultTree->Header->Columns->Add();
      NewColumn->Alignment = CountryTreeColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      //      int temp = -1;
      //      String key = "CountryColumn" + String( i );
      //      MinosParameters::getMinosParameters() ->getIntProfile( key.c_str(), temp, -1 );

      //      if ( temp > 0 )
      //         NewColumn->Width = temp;
      //      else
      NewColumn->Width = CountryMultTree->Canvas->TextWidth( CountryTreeColumns[ i ].width );

      NewColumn->Text = CountryTreeColumns[ i ].title;
   }
   CountryMultTree->EndUpdate();
   CountryMultTree->Header->Options = ( CountryMultTree->Header->Options << hoVisible );


   CountryMultTree->ValidateNode( 0, true );
}
static GridColumn DistrictTreeColumns[ ectMultMaxCol - 1 ] =
   {
      GridColumn( ectCall, "XXXXXXX", "Code", taLeftJustify ),
      GridColumn( ectWorked, "Wk CtX", "Wkd", taCenter ),
      GridColumn( ectLocator, "MM00MM00", "Locator", taLeftJustify ),
      GridColumn( ectBearing, "BRGXXX", "brg", taCenter ),
      GridColumn( ectName, "This is a Very Very long District", "District", taLeftJustify ),
   };
void TMultDispFrame::reInitialiseDistricts()
{
   DistrictMultTree->RootNodeCount = 0;
   if ( !ct )
   {
      // clear down
      return ;
   }
//   DistrictMultTree->RootNodeCount = MultLists::getMultLists() ->getDistListSize();

   DistrictMultTree->BeginUpdate();

   DistrictMultTree->RootNodeCount = MultLists::getMultLists() ->getDistListSize();

   DistrictMultTree->Header->Columns->Clear();
   DistrictMultTree->Margin = 0;
   DistrictMultTree->TextMargin = 4;
   for ( int i = 0; i < ectMultMaxCol - 1; i++ )
   {
      TVirtualTreeColumn *NewColumn = DistrictMultTree->Header->Columns->Add();
      NewColumn->Alignment = DistrictTreeColumns[ i ].alignment;
      NewColumn->Margin = 0;
      NewColumn->Spacing = 0;

      //      int temp = -1;
      //      String key = "CountryColumn" + String( i );
      //      MinosParameters::getMinosParameters() ->getIntProfile( key.c_str(), temp, -1 );

      //      if ( temp > 0 )
      //         NewColumn->Width = temp;
      //      else
      NewColumn->Width = DistrictMultTree->Canvas->TextWidth( DistrictTreeColumns[ i ].width );

      NewColumn->Text = DistrictTreeColumns[ i ].title;
   }
   DistrictMultTree->EndUpdate();
   DistrictMultTree->Header->Options = ( DistrictMultTree->Header->Options << hoVisible );


   DistrictMultTree->ValidateNode( 0, true );
}
void TMultDispFrame::reInitialiseLocators()
{
   LocatorMultTree->RootNodeCount = 0;
   if ( !ct )
   {
      // clear down
      return ;
   }
   LocatorMultTree->RootNodeCount = ct->locs.llist.size();
   LocatorMultTree->ValidateNode( 0, true );
   LocatorMultTree->FullExpand();
}
//---------------------------------------------------------------------------
void TMultDispFrame::reInitialiseStats()
{
   StatsDispFrame->reInitialise();
}
//---------------------------------------------------------------------------
void TMultDispFrame::reInitialiseComp()
{
   CompFrame->reInitialise();
}
//---------------------------------------------------------------------------
void TMultDispFrame::refreshMults()
{
   if ( ct )
   {
      int c = -1;
      int d = -1;
      if ( CountryMultTree->FocusedNode )
      {
         c = CountryMultTree->FocusedNode->Index;
      }
      if ( DistrictMultTree->FocusedNode )
      {
         d = DistrictMultTree->FocusedNode->Index;
      }

      reInitialiseCountries();
      reInitialiseDistricts();
      if ( c >= 0 )
      {
         scrollToCountry( c, false );
      }
      if ( d >= 0 )
      {
         scrollToDistrict( d, false );
      }
      reInitialiseLocators();

      Repaint();
      LocatorMultTree->Repaint();
   }
}
//---------------------------------------------------------------------------
void __fastcall TMultDispFrame::CountryMultTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::CountryMultTreeGetText(
   TBaseVirtualTree */*Sender*/, PVirtualNode Node, TColumnIndex Column,
   TVSTTextType /*TextType*/, WideString &CellText )
{
   if ( Column >= 0 )
   {
      std::string disp = MultLists::getMultLists() ->getCtryListText( Node->Index, CountryTreeColumns[ Column ].fieldId, ct );
      CellText = disp.c_str();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::CountryMultTreeInitNode(
   TBaseVirtualTree */*Sender*/, PVirtualNode /*ParentNode*/, PVirtualNode Node,
   TVirtualNodeInitStates &/*InitialStates*/ )
{
   // No action
   CountryEntry * ce = MultLists::getMultLists() ->getCtryListAt( Node->Index );
   bool makeVisible = false;
   for ( int i = 0; i < CONTINENTS; i++ )
   {
      if ( ce->continent == contlist[ i ].continent )
      {
         makeVisible = contlist[ i ].allow;
         break;
      }
   }
   int worked = ct->getCountriesWorked( Node->Index );
   if ( worked && showUnworked && !showWorked )
   {
      makeVisible = false;
   }
   else
      if ( !worked && !showUnworked && showWorked )
      {
         makeVisible = false;
      }

   CountryMultTree->IsVisible[ Node ] = makeVisible;
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::CountryMultTreeInitChildren(
   TBaseVirtualTree */*Sender*/, PVirtualNode /*Node*/, DWORD &/*ChildCount*/ )
{
   //   ChildCount = 6;
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::DistrictMultTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::DistrictMultTreeGetText(
   TBaseVirtualTree */*Sender*/, PVirtualNode Node, TColumnIndex Column,
   TVSTTextType /*TextType*/, WideString &CellText )
{
   if ( Column >= 0 )
   {
      std::string disp = MultLists::getMultLists() ->getDistListText( Node->Index, DistrictTreeColumns[ Column ].fieldId, ct );
      CellText = disp.c_str();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::DistrictMultTreeInitNode(
   TBaseVirtualTree */*Sender*/, PVirtualNode /*ParentNode*/, PVirtualNode Node,
   TVirtualNodeInitStates &/*InitialStates*/ )
{
   bool makeVisible = true;
   int worked = ct ->getDistrictsWorked( Node->Index );
   if ( worked && showUnworked && !showWorked )
   {
      makeVisible = false;
   }
   else
      if ( !worked && !showUnworked && showWorked )
      {
         makeVisible = false;
      }

   DistrictMultTree->IsVisible[ Node ] = makeVisible;
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::DistrictMultTreeInitChildren(
   TBaseVirtualTree */*Sender*/, PVirtualNode/* Node*/, DWORD &/*ChildCount*/ )
{
   //   ChildCount = 6;
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::LocatorMultTreeGetNodeDataSize(
   TBaseVirtualTree */*Sender*/, int &NodeDataSize )
{
   NodeDataSize = 0;
}
//---------------------------------------------------------------------------


void __fastcall TMultDispFrame::LocatorMultTreeInitNode(
   TBaseVirtualTree *Sender, PVirtualNode ParentNode, PVirtualNode Node,
   TVirtualNodeInitStates &InitialStates )
{
   if ( !ParentNode )
   {
      InitialStates << ivsHasChildren;
   }
   else
   {
      bool loconline = false;
      for (int i = 0; i < 10; i++)
      {
         unsigned char *lc = ct->locs.llist[ ParentNode->Index ] ->map( Node->Index * 10 + i );
         if ( lc && *lc != 0 )
            loconline = true;

      }
      if ( !loconline )
      {
         Sender->IsVisible[ Node ] = false;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::LocatorMultTreeInitChildren(
   TBaseVirtualTree */*Sender*/, PVirtualNode /*Node*/, DWORD &ChildCount )
{
   ChildCount = 10;
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::LocatorMultTreeGetText(
   TBaseVirtualTree */*Sender*/, PVirtualNode Node, TColumnIndex /*Column*/,
   TVSTTextType /*TextType*/, WideString &CellText )
{
   if ( Node->States.Contains( vsHasChildren ) )
      CellText = ct->locs.llist[ Node->Index ] ->loc;
   else
   {
      String dispLine;
      for (int i = 0; ct->locs.llist.size() && i < 10; i++)
      {
         unsigned char *lc = ct->locs.llist[ Node->Parent->Index ] ->map( Node->Index * 10 + i );
         AnsiString disp;
         disp.printf( "%2.2d", Node->Index * 10 + i );
         if ( lc && *lc )
            dispLine += disp + " (" + *lc + ") ";
      }
      CellText = dispLine;
   }
}
//---------------------------------------------------------------------------

void TMultDispFrame::scrollToDistrict( unsigned int district_ind, bool makeVisible )
{
   reInitialiseDistricts();
   PVirtualNode l = DistrictMultTree->GetFirstChild( DistrictMultTree->RootNode );
   while ( district_ind && l )
   {
      if ( l->Index == district_ind )
      {
         if ( makeVisible )
         {
            DistrictMultTree->IsVisible[ l ] = true;
         }
         if ( DistrictMultTree->IsVisible[ l ] == true )
         {
            DistrictMultTree->FocusedNode = l;
            DistrictMultTree->Selected[ l ] = true;
         }
         break;
      }
      l = DistrictMultTree->GetNextSibling( l );
   }
}
void TMultDispFrame::scrollToCountry( unsigned int ctry_ind, bool makeVisible )
{
   reInitialiseCountries();
   PVirtualNode l = CountryMultTree->GetFirstChild( CountryMultTree->RootNode );
   while ( ctry_ind && l )
   {
      if ( l->Index == ctry_ind )
      {
         if ( makeVisible )
         {
            CountryMultTree->IsVisible[ l ] = true;
         }
         if ( CountryMultTree->IsVisible[ l ] == true )
         {
            CountryMultTree->FocusedNode = l;
            CountryMultTree->Selected[ l ] = true;
         }
         break;
      }
      l = CountryMultTree->GetNextSibling( l );
   }
}
//---------------------------------------------------------------------------


void __fastcall TMultDispFrame::FilterClick( TObject */*Sender*/ )
{
   if ( filterClickEnabled )
   {
      saveFilters();
      reInitialiseCountries();
      reInitialiseDistricts();
      reInitialiseLocators();
      reInitialiseStats();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultDispFrame::FilterTimerTimer( TObject */*Sender*/ )
{
   // Use a timer so that it can all get constructed and sorted out
//   FilterSplitter->Maximized = true;
   FilterTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void TMultDispFrame::ValidateError_Event ( MinosEventBase & Event )
{
#warning this really needs the log pointer for context
		ActionEvent1<int, EN_ValidateError> & S = dynamic_cast<ActionEvent1<int, EN_ValidateError> &> ( Event );
      int mess_no = S.getData();
      if ( mess_no == -1 )
      {
         errs.clear();
         return ;
      }

      // add the message into the error list
      errs.insert( &errDefs[ mess_no ] );
}
void TMultDispFrame::ShowErrorList_Event ( MinosEventBase & /*Event*/ )
{
   showErrorList(errs);
}
void TMultDispFrame::showErrorList( ErrorList &errs )
{
   ErrList->Clear();
   for ( ErrorIterator i = errs.begin(); i != errs.end(); i++ )
   {
      ErrList->Items->Add( ( *i ) ->errStr.c_str() );
   }
   ErrList->ItemIndex = 0;
}


void __fastcall TMultDispFrame::FilterSplitterMoved(TObject */*Sender*/)
{
//   
}
//---------------------------------------------------------------------------

