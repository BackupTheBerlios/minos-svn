/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
#ifndef KeyerAboutH
#define KeyerAboutH 
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
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
   __published:
      TPanel *Panel1;
      TImage *ProgramIcon;
      TLabel *ProductName;
      TLabel *Version;
      TLabel *Copyright;
      TLabel *Comments;
      TLabel *WebLabel;
      TPanel *Panel2;
      TButton *OKButton;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall WebLabelClick( TObject *Sender );
   private:
   public:
      virtual __fastcall TAboutBox( TComponent *Owner );
};
//----------------------------------------------------------------------------
#endif
