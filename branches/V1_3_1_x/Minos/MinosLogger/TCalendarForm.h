/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef TCalendarFormH
#define TCalendarFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
//---------------------------------------------------------------------------
class TCalendarForm : public TForm
{
   __published: 	// IDE-managed Components
      TStringGrid *CalendarGrid;
      TPanel *Panel1;
   TButton *CloseButton;
      TButton *SelectButton;
      TButton *GetCalendarButton;
      TIdHTTP *IdHTTP1;
   TLabel *CalendarVersionLabel;
   TEdit *YearEdit;
   TButton *YearDownButton;
   TButton *YearUpButton;
      void __fastcall CloseButtonClick( TObject *Sender );
      void __fastcall FormShow( TObject *Sender );
      void __fastcall GetCalendarButtonClick( TObject *Sender );
      void __fastcall SelectButtonClick( TObject *Sender );
      void __fastcall CalendarGridDblClick( TObject *Sender );
   void __fastcall YearDownButtonClick(TObject *Sender);
   void __fastcall YearUpButtonClick(TObject *Sender);
   private: 	// User declarations
      bool downloadCalendar(String URL, String dest);
   public: 		// User declarations
      __fastcall TCalendarForm( TComponent* Owner );
      Calendar vhf;
      IndividualContest ic;
      String description;
};
//---------------------------------------------------------------------------
#endif
