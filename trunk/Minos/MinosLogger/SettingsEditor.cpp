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

#include "gridhint.h"
#include "SettingsEditor.h"
#include "enqdlg.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsEditDlg *SettingsEditDlg;
//---------------------------------------------------------------------------
__fastcall TSettingsEditDlg::TSettingsEditDlg( TComponent* Owner, SettingsBundle *b )
      : TForm( Owner ), bundle( b ), GridHintWindow( 0 ), oldX( 0 ), oldY( 0 ),
         currSectionOnly(false)

{
   initialSection = bundle->getSection();
   GridHintWindow = new TGridHint( this );
   GridHintWindow->SetHintControl( SectionGrid );
}
//---------------------------------------------------------------------------
void TSettingsEditDlg::ShowCurrentSectionOnly()
{
   currSectionOnly = true;
}
//---------------------------------------------------------------------------
void TSettingsEditDlg::showSections()
{
   SectionsList->Items->Clear();
   std::vector<std::string> sections;

   if (currSectionOnly)
   {
      sections.push_back(initialSection);
   }
   else
   {
      sections = bundle->getSections( );
   }

   Caption = Caption + ( " - " + bundle->getBundle() + " for " + bundle->getSection() ).c_str();

   int offset = 0;
   for ( unsigned int i = 0; i < sections.size(); i++ )
   {
      if ( sections[ i ] == bundle->getSection() )
         offset = i;
      SectionsList->Items->Add( sections[ i ].c_str() );
   }
   SectionsList->ItemIndex = offset;
   showSection( );
}
//---------------------------------------------------------------------------
void TSettingsEditDlg::showSection()
{
   // Select this section to display on tree
   int offset = SectionsList->ItemIndex;
   if (currSectionOnly)
   {
      bundle->openSection( initialSection );
      SectionGrid->Visible = true;
      showDetails();
   }
   else if ( offset > 0 )
   {
      std::vector<std::string> sections = bundle->getSections( );
      std::string sect = sections[ offset ];
      bundle->openSection( sect );
      SectionGrid->Visible = true;
      showDetails();
   }
   else
   {
      bundle->openSection( noneBundle );
      SectionGrid->Visible = false;
   }

   NewSectionButton->Enabled = !currSectionOnly;
   DeleteButton->Enabled = currSectionOnly?false:( offset > 0 );
   CopyButton->Enabled = currSectionOnly?false:( offset > 0 );
}
//---------------------------------------------------------------------------
void __fastcall TSettingsEditDlg::OKButtonClick( TObject */*Sender*/ )
{
   getDetails();
   bundle->flushProfile();
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TSettingsEditDlg::DeleteButtonClick( TObject */*Sender*/ )
{
   //   getDetails();  // we are about to delete it - don't save it first!

   // delete the section - don't allow "<None>" to be deleted!
   // First, put up an "Are you sure?" message

   int offset = SectionsList->ItemIndex;
   if ( offset == 0 )
      MinosParameters::getMinosParameters() ->mshowMessage( "You cannot delete the empty section!", this );
   else
      if ( MinosParameters::getMinosParameters() ->yesNoMessage( this, "Are you sure you want to delete the current section?" ) )
      {
         bundle->clearProfileSection();
         showSections();
         showDetails();
      }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsEditDlg::SectionsListClick( TObject */*Sender*/ )
{
   getDetails();  // save what is set already
   showSection();
}
//---------------------------------------------------------------------------
void TSettingsEditDlg::showDetails()
{
   int offset = SectionsList->ItemIndex;
   if ( offset > 0 || currSectionOnly)
   {
      std::vector<int> entries = bundle->getBundleEntries();
      SectionGrid->RowCount = entries.size() + 1;
      SectionGrid->ColCount = 2;
      //                    col row
      SectionGrid->Cells[ 0 ][ 0 ] = "Field";
      SectionGrid->Cells[ 1 ][ 0 ] = "Value";

      for ( unsigned int r = 0; r < entries.size(); r++ )
      {
         //                    col row
         SectionGrid->Cells[ 0 ][ r + 1 ] = bundle->nameOf( entries[ r ] ).c_str();
         std::string val;
         bundle->getStringProfile( entries[ r ], val );
         //                    col row
         SectionGrid->Cells[ 1 ][ r + 1 ] = val.c_str();
      }

   }
}
//---------------------------------------------------------------------------
void TSettingsEditDlg::getDetails()
{
   int offset = SectionsList->ItemIndex;
   if ( offset > 0 || currSectionOnly)
   {
      std::vector<int> entries = bundle->getBundleEntries();
      for ( unsigned int r = 0; r < entries.size(); r++ )
      {
         // col row
         std::string val = SectionGrid->Cells[ 1 ][ r + 1 ].c_str();
         bundle->setStringProfile( entries[ r ], val.c_str() );
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TSettingsEditDlg::CopyButtonClick( TObject */*Sender*/ )
{
   getDetails();  // save what is set already

   int offset = SectionsList->ItemIndex;
   if ( offset > 0 && !currSectionOnly)
   {
      std::string Value = "new section";
      if ( enquireDialog( this, "Please give a name for the new section", Value ) )
      {
         getDetails();  // save old section

         if ( bundle->dupSection( Value ) )
         {
            bundle->openSection( Value );
            showSections();
            showDetails();
         }
         else
         {
            MinosParameters::getMinosParameters() ->mshowMessage( "Section already exists", this );
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsEditDlg::FormShow( TObject */*Sender*/ )
{
   MinosParameters::getMinosParameters() ->applyFontMultiplier(this);
   showSections();
   showDetails();
}
//---------------------------------------------------------------------------

void __fastcall TSettingsEditDlg::CancelButtonClick( TObject */*Sender*/ )
{
   bundle->openSection( initialSection );
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsEditDlg::NewSectionButtonClick( TObject */*Sender*/ )
{
   getDetails();  // save what is set already
   std::string Value = "new section";
   if ( enquireDialog( this, "Please give a name for the new section", Value ) )
   {
      bundle->newSection( Value );
      bundle->openSection( Value );
      showSections();
      showDetails();
   }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsEditDlg::GridHintTimerTimer( TObject */*Sender*/ )
{
   // timer used to time out the grid hint - but we don't
   GridHintTimer->Enabled = false;
   if ( !GridHintWindow )
      return ;
   if ( GridHintWindow->Showing || !Application->Active )
   {
      GridHintWindow->Showing = false;
   }
   else
   {
      TStringGrid *QGrid = dynamic_cast<TStringGrid *>( GridHintWindow->GetHintControl() );
      if ( QGrid )
      {
         POINT mpos, mpos2;
         ::GetCursorPos( &mpos );

         mpos2 = QGrid->ScreenToClient( mpos );

         if ( PtInRect( &( QGrid->ClientRect ), mpos2 ) )
         {
            std::vector<std::string>hints = bundle->getBundleHints( );
            if (HintRow > 0 && HintRow <= (int)hints.size())
            {
               QGrid->Hint = hints[ HintRow - 1 ].c_str();
               GridHintWindow->SetXY( mpos.x, mpos.y );
               GridHintWindow->Showing = true;
            }
         }
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TSettingsEditDlg::SectionGridMouseMove( TObject */*Sender*/,
      TShiftState /*Shift*/, int X, int Y )
{
   GridHintWindow->SetHintControl( SectionGrid );
   if ( ( X != oldX ) || ( Y != oldY ) )
   {
      GridHintWindow->Showing = false;
      GridHintTimer->Enabled = false;

      oldX = X;
      oldY = Y;


      int offset = SectionsList->ItemIndex;
      if ( offset > 0 || currSectionOnly)
      {
         try
         {
            SectionGrid->MouseToCell( X, Y, HintCol, HintRow );

            if ( ( HintCol >= 0 ) && ( HintRow >= 1 ) )
            {
               // and we want to set the hint position to the current mouse position
               GridHintTimer->Enabled = true;
               GridHintTimer->Interval = 500;
            }
         }
         catch ( ... )
         {}
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsEditDlg::FormMouseDown( TObject */*Sender*/,
      TMouseButton /*Button*/, TShiftState /*Shift*/, int /*X*/, int /*Y*/ )
{
   if ( GridHintWindow->Showing )
   {
      GridHintWindow->Showing = false;
   }
   GridHintTimer->Enabled = false;
}
//---------------------------------------------------------------------------

