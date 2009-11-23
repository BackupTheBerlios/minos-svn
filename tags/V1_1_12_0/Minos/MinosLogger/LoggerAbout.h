/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef LoggerAboutH
#define LoggerAboutH 
//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include "RCVersion.h"
#include "ConfigMainFrame.h"
#include <ComCtrls.hpp> 
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
   __published:
      TPageControl *PageControl1;
      TTabSheet *AboutTabSheet;
      TPanel *Panel1;
      TImage *ProgramIcon;
      TLabel *ProductName;
      TLabel *Version;
      TLabel *Copyright;
      TLabel *Comments;
      TLabel *WebLabel;
      TTabSheet *AutoStartTabSheet;
      TTConfigFrame *TConfigFrame1;
      TPanel *Panel2;
      TButton *OKButton;
      TButton *LoggerOnlyButton;
      TTabSheet *TabSheet1;
      TMemo *MinosMemo;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall WebLabelClick( TObject *Sender );
      void __fastcall TConfigFrame1StartButtonClick( TObject *Sender );
      void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
      void __fastcall TConfigFrame1StopButtonClick( TObject *Sender );
      void __fastcall LoggerOnlyButtonClick( TObject *Sender );
      void __fastcall TConfigFrame1CancelButtonClick( TObject *Sender );
   private:
      bool doStartup;
   public:
      virtual __fastcall TAboutBox( TComponent *Owner, bool onStartup );
      static bool TAboutBox::ShowAboutBox( TComponent *Owner, bool onStartup );
};
//----------------------------------------------------------------------------
#endif
