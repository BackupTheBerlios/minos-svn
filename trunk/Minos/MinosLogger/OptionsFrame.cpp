//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OptionsFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionFrame *OptionFrame;
//---------------------------------------------------------------------------
__fastcall TOptionFrame::TOptionFrame(TComponent* Owner)
   : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
