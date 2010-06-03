/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BundleFrameH
#define BundleFrameH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Hstcbo.hpp"
#include <Dialogs.hpp>
#include <ExtCtrls.hpp> 
//---------------------------------------------------------------------------
class TSettingBundleFrame : public TFrame
{
   __published:  	// IDE-managed Components
      TLabel *BundleNameLabel;
      THistoryComboBox *BundleSection;
      TButton *BundleEdit;
   TPanel *Panel1;
      void __fastcall BundleSectionChange( TObject *Sender );
      void __fastcall BundleEditClick( TObject *Sender );
   private:  	// User declarations
      SettingsBundle *bundle;
      MinosItem<std::string> *bname;
   public:  		// User declarations
      __fastcall TSettingBundleFrame( TComponent* Owner );
      void initialise();
      void initialise( String cap, SettingsBundle *bundle, MinosItem<std::string> *name );
      bool doEdit( TObject *Sender );
      void enableBundle( bool );
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingBundleFrame *SettingBundleFrame;
//---------------------------------------------------------------------------
#endif
