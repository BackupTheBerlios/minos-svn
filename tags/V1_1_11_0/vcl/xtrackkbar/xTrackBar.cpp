//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "xTrackBar.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TxTrackBar *)
{
    new TxTrackBar(NULL);
}
//---------------------------------------------------------------------------
__fastcall TxTrackBar::TxTrackBar(TComponent* Owner)
    : TTrackBar(Owner) ,
      m_EnableSelRange(false) // set default value to false
{
}

void __fastcall TxTrackBar::CreateParams(Controls::TCreateParams &Params)
{
    // TTrackBar's CreateParams assumes that the user wants a wide slider.
    // Call base class CreateParams, and then check the private variable
    // to see if the user would rather have a thin slider. If a thin
    // slider is wanted, clear the TBS_ENABLESELRANGE bit that was hardcoded in
    // the base class.

    // Note: The wide slider background facilitates having a selection range
    //       right on the background of the slider. I haven't seen a program
    //       that actually uses these. If you haven't, you can investigate what
    //       this is all about. Place a normal track bar on a form, set SelEnd
    //       to 5, and set SelStart to 0. When you clear the TBS_ENABLESELRANGE
    //       bit, SelStart and SelEnd become impotent (don't do anything).

    TTrackBar::CreateParams(Params);          // call base class
    if (!m_EnableSelRange)                     // is a wide background wanted
        Params.Style &= ~TBS_ENABLESELRANGE;  // if not, nuke the SELRANGE bit
}

void __fastcall TxTrackBar::SetEnableSelRange(bool enabled)
{
    // This function is the write method for the EnableSelRange property
    // and can be called from the IDE or from code at runtime.

    // First, save the new setting into the private variable. Next, recreate
    // the window handle. Why do this? I dunno, Borland does it lots of times
    // Actually this is needed because you cannot use the API to change the
    // TBS_ENABLESELRANGE bit in the track bars style once the control has
    // been created. RecreateWnd is a VCL function whose sole purpose appears
    // to be redrawing controls when a characteristic of a window changes
    // that can only be set when the control is created. RecreateWnd results
    // in a re-call of CreateParams.


    // Don't RecreateWnd unless the style changes
    if(enabled != m_EnableSelRange)
    {
        m_EnableSelRange = enabled;
        if(!ComponentState.Contains(csLoading)) 
            RecreateWnd();
    }
}



//---------------------------------------------------------------------------
namespace Xtrackbar
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TxTrackBar)};
         RegisterComponents("Snd", classes, 0);
    }
}
//---------------------------------------------------------------------------
