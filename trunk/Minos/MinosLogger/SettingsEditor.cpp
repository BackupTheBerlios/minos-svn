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

#include "SettingsEditor.h"
#include "enqdlg.h"
#include "optionsframe.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsEditDlg *SettingsEditDlg;
//---------------------------------------------------------------------------
__fastcall TSettingsEditDlg::TSettingsEditDlg( TComponent* Owner, SettingsBundle *b )
      : TForm( Owner ), bundle( b ),
         currSectionOnly(false)

{
   initialSection = bundle->getSection();
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
      OptionsScrollBox->Visible = true;
      showDetails();
   }
   else if ( offset > 0 )
   {
      std::vector<std::string> sections = bundle->getSections( );
      std::string sect = sections[ offset ];
      bundle->openSection( sect );
      OptionsScrollBox->Visible = true;
      showDetails();
   }
   else
   {
      OptionsScrollBox->Visible = false;
      bundle->openSection( noneBundle );
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
   for ( unsigned int i= 0; i < options.size(); i++ )
   {
      delete options[i];
   }
   options.clear();
   int offset = SectionsList->ItemIndex;
   if ( offset > 0 || currSectionOnly)
   {
      std::vector<int> entries = bundle->getBundleEntries();


   int etop = 0;

   std::vector<std::string>hints = bundle->getBundleHints( );

   for ( unsigned int i= 0; i < entries.size(); i++ )
   {
      TOptionFrame *tcf = new TOptionFrame( this );
      options.push_back(tcf);

      tcf->Name = "entryopt" + String(i);
      tcf->Parent = OptionsScrollBox;
      tcf->Top = etop;
      etop += tcf->Height;
      tcf->Align = alTop;
      std::string val;
      bundle->getStringProfile( entries[ i ], val );
      tcf->OptionLabel->Caption = bundle->displayNameOf( entries[ i ] ).c_str();
      tcf->OptionEdit->Text =  val.c_str();
      tcf->Hint = hints[i].c_str();
      bool RO = bundle->isReadOnly(entries[ i ]);
      tcf->OptionEdit->ReadOnly = RO;
      if (RO)
      {
         tcf->OptionEdit->Color = clBtnFace;
      }
   }
   }
}
//---------------------------------------------------------------------------
void TSettingsEditDlg::getDetails()
{
   int offset = SectionsList->ItemIndex;
   if (( offset > 0 || currSectionOnly) && options.size())
   {
      std::vector<int> entries = bundle->getBundleEntries();
      for ( unsigned int r = 0; r < entries.size(); r++ )
      {
      /*
         // col row
         std::string val = SectionGrid->Cells[ 1 ][ r + 1 ].c_str();
      */
         std::string val = options[r]->OptionEdit->Text.c_str();
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
   MinosParameters::getMinosParameters() ->applyFontChange(this);
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



