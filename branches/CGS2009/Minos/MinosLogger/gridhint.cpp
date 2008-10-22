/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c)M. J. Goodey G0GJV, 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#include "logger_pch.h"
#pragma hdrstop

#include "gridhint.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

// This is a derivative of THintWindow
// and is used to show our own fly by hints

/*procedure THintWindow.NCPaint(DC: HDC);
var
  R: TRect;
begin
  R := Rect(0, 0, Width, Height);
  Windows.DrawEdge(DC, R, BDR_RAISEDOUTER, BF_RECT);
end;
*/
void __fastcall TGridHint::NCPaint( HDC DC )
{
   TRect R( 0, 0, Width, Height );
   DrawEdge( DC, &R, BDR_RAISEDOUTER, BF_RECT );
}
void TGridHint::SetShowing( bool s )
{
   if ( s )
      Appear();
   else
      Disappear();
}
bool TGridHint::GetShowing( void )
{
   return FShowing;
}

/*Windows::*/TRect __fastcall TGridHint::CalcHintRect(  int /*MaxWidth*/,
      const System::String AHint,
      void * /*AData*/ )
{
   String temp = AHint;
   RECT r = Bounds( 0, 0, HintControl->Width, 0 );
   // need a TCustomControl to get a Canvas
   DrawText( ( HDC ) Canvas->Handle, temp.t_str(), -1, &r,
             DT_CALCRECT | DT_LEFT | DT_WORDBREAK | DT_NOPREFIX );
   OffsetRect( &r, XPos + 10, YPos + 10 );
   r.right += 6;
   r.bottom += 2;
   return r;
}
void TGridHint::Appear( void )
{
   String Hint;
   TRect HintRect;

   //   Hint = GetShortHint(HintControl->Hint);   // this splits on | character
   Hint = HintControl->Hint;
   if ( Hint.Length() )
   {
      HintRect = CalcHintRect( -1, Hint, 0 );
      ActivateHint( HintRect, Hint );
      FShowing = true;
   }
}
void TGridHint::Disappear( void )
{
   ReleaseHandle();
   FShowing = false;
}

__fastcall TGridHint::TGridHint( TComponent *f ) : THintWindow( f ),
      FShowing( false ), HintControl( 0 )
{
   Color = TColor( ( 225 << 16 ) + ( 255 << 8 ) + 255 );
}

__fastcall TGridHint::~TGridHint()
{}

