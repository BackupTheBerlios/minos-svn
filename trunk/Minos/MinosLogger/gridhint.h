/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c)M. J. Goodey G0GJV, 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
#ifndef gridhintH
 #define gridhintH 
//---------------------------------------------------------------------------

class TGridHint : public THintWindow
{
   private:
      bool FShowing;
      TControl *HintControl;

      void SetShowing( bool );
      bool GetShowing( void );
      int XPos;
      int YPos;
   public:
      virtual void __fastcall TGridHint::NCPaint( HDC DC );

      virtual /*Windows::*/TRect __fastcall CalcHintRect( int MaxWidth, const System::String AHint, void * AData );

      void Appear( void );
      void Disappear( void );
      void SetHintControl( TControl *tc )
      {
         HintControl = tc;
      }
      TControl * GetHintControl( void )
      {
         return HintControl;
      }
      void SetXY( int x, int y )
      {
         XPos = x;
         YPos = y;
      }

      __fastcall TGridHint( TComponent * );
      __fastcall ~TGridHint();
      __property bool Showing = {read = GetShowing, write = SetShowing};
};
#endif
