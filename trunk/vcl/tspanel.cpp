/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#pragma package(smart_init)
#include <vcl\mask.hpp>

#include "tspanel.h"
#include <vcl\comctrls.hpp>
class PACKAGE CompDetail
{
	public:
      int Left;
      int Right;
      int Top;
      int Bottom;
      CompDetail():Left(0), Right(0), Top(0), Bottom(0){}
      ~CompDetail(){}
};
class PACKAGE SetInAlign
{
	private:
		static inAlign;
	public:
		SetInAlign(){inAlign++;}
		~SetInAlign(){inAlign--;}

        static bool IsInAlign(){return inAlign > 0;}
};
int SetInAlign::inAlign = 0;
//---------------------------------------------------------------------------
//static inline TSPanel *ValidCtrCheck()
//{
//	return new TSPanel(NULL);
//}
//---------------------------------------------------------------------------
__fastcall TSPanel::TSPanel(TComponent* Owner)
	: Extctrls::TCustomPanel(Owner), compdetails(0), panelpos(0), ccount(0),
   fGrowButtons(true)
{

}
__fastcall TSPanel::TSPanel(HWND ParentWindow) : Extctrls::TCustomPanel(ParentWindow)
, compdetails(0), panelpos(0), ccount(0),fGrowButtons(true)
{
}

__fastcall TSPanel::~TSPanel(void)
{
	if (compdetails)
    	delete [] compdetails;
    if (panelpos)
    	delete panelpos;
}
//---------------------------------------------------------------------------
namespace Tspanel
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSPanel)};
		RegisterComponents("MJGAddons", classes, 0);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TSPanel::GetDesignDrag()
{
	return fDesignDrag;
}
//---------------------------------------------------------------------------
void __fastcall TSPanel::SetDesignDrag(bool s)
{
	fDesignDrag = s;
    GetCompDetails();
}
//---------------------------------------------------------------------------
void __fastcall TSPanel::GetCompDetails(void)
{
    if (ComponentState.Contains(csDesigning) && (!DesignDrag || ComponentState.Contains(csLoading)))
	{
    	return;
    }
	if (compdetails)
    	delete [] compdetails;
    if (panelpos)
    	delete panelpos;

    panelpos = new TRect;
    panelpos->Top = Top;
    panelpos->Left = Left;
    panelpos->Right = Width;
    panelpos->Bottom = Height;

    compdetails = new CompDetail[ControlCount];
    ccount = ControlCount;

    for (int i = 0; i < ControlCount; i++)
    {
      TControl *tc = Controls[i];
      compdetails[i].Top = tc->Top;
      compdetails[i].Left = tc->Left;
      compdetails[i].Right = tc->Width;
      compdetails[i].Bottom = tc->Height;
    }

}
void __fastcall TSPanel::Loaded(void)
{
	// we have been loaded; we can now look at our children
    inherited::Loaded();		// call back to our parent

    GetCompDetails();
}
//---------------------------------------------------------------------------
void __fastcall TSPanel::Resize(void)
{
	SetInAlign fred;
	inherited::Resize();

    if (ComponentState.Contains(csDesigning) && (!DesignDrag || ComponentState.Contains(csLoading)))
	{
    	return;
    }

	if (panelpos && compdetails && panelpos->Bottom != 0 && panelpos->Right != 0)
    {
        double xscale = 0;
        double yscale = 0;
        try
        {
        		xscale = (double)Width / panelpos->Right;
        		yscale = (double)Height / panelpos->Bottom;
        }
        catch ( EMathError & e )
        {
//				String ExceptionName = e.ClassName();
//				Application->MessageBox(e.Message.c_str(),e.ClassName().c_str(),MB_OK | MB_APPLMODAL | MB_ICONERROR);
        }

        if (!ComponentState.Contains(csDesigning) && fLimitSize)
        {
           if (xscale < 1.0)
           {
               xscale = 1.0;
           }
           if (yscale < 1.0)
           {
               yscale = 1.0;
           }
		}

        for (int i = 0; i < ControlCount; i++)
        {
            TControl *tc = Controls[i];

            if (tc->Align == alNone)
            {
            	int l = compdetails[i].Left * xscale;
                int t = compdetails[i].Top * yscale;
                int ow = compdetails[i].Right;
                int oh = compdetails[i].Bottom;
                int w = ow;
                int h = oh;

            	if (!tc->ControlStyle.Contains(csFixedWidth))
                {
	                w = compdetails[i].Right * xscale;
                }
	           	if (!tc->ControlStyle.Contains(csFixedHeight))
                {
	                h = compdetails[i].Bottom * yscale;
				}

                if (TLabel *tcl = dynamic_cast<TLabel *>(tc))
                {
	               	String text = tcl->Caption;
                	if (tcl->WordWrap)
                  {
                  	// word wrap => scale naturally
							tc->SetBounds(l, t, w, h);
                  }
                  else
                  {
                     if (tcl->Alignment == taRightJustify)
                     {
                        tcl->SetBounds(l + w - ow, t, ow, oh);
                     }
                     else if (tcl->Alignment == taLeftJustify)
                     {
                        tcl->SetBounds(l, t, ow, oh);
                     }
                     else if (tcl->Alignment == taCenter)
                        tcl->SetBounds(l + (w - ow)/2, t, ow, oh);
                  }
                  tcl->Caption = text;
                }
                else if (TButton *tb = dynamic_cast<TButton *>(tc))
                {
						if ( GrowButtons )
                  	tb->SetBounds(l, t, w, oh);
                  else
                  	tb->SetBounds(l, t, ow, oh);
                }
                else if (TCustomMaskEdit *tcme = dynamic_cast<TCustomMaskEdit *>(tc))
                {
					tcme->SetBounds(l, t, w, oh);
                }
                else if (TEdit *te = dynamic_cast<TEdit *>(tc))
                {
					te->SetBounds(l, t, w, oh);
                }
                else if (TUpDown *tu = dynamic_cast<TUpDown *>(tc))
                {
					tu->SetBounds(l, t, ow, oh);
                }
                else
                {
					tc->SetBounds(l, t, w, h);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TSPanel::AlignControls(Controls::TControl* AControl, Types::TRect &Rect)
{
	bool al = SetInAlign::IsInAlign();
	SetInAlign fred;

	inherited::AlignControls(AControl, Rect);

	if (al || ComponentState.Contains(csLoading))
    	return;

    if (!AControl && (ccount == ControlCount))  // look if specific control named,
    											// or the number has changed
    	return;

    if (ComponentState.Contains(csDesigning) && DesignDrag)
    {
	    GetCompDetails();
    }
}
//---------------------------------------------------------------------------
void __fastcall TSPanel::WndProc(Messages::TMessage &Message)
{
	inherited::WndProc(Message);

   if (Message.Msg == WM_SETFONT)
   {
       if (ComponentState.Contains(csDesigning) && (!DesignDrag || ComponentState.Contains(csLoading)))
       {
         return;
       }
       Resize();
       GetCompDetails();
   }
}
//---------------------------------------------------------------------------

