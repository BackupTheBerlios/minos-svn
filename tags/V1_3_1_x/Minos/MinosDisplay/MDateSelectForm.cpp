/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "display_pch.h"
#pragma hdrstop

#include "MDateSelectForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDateSelectForm *DateSelectForm;

/* Don't have any leap year code available.  Seemed like a straigthforward
   translation, but should be checked.
*/
bool TDateSelectForm::IsLeapYear(int AYear)
{
  return  ((AYear %   4) == 0) &&
		 (((AYear % 100) != 0) ||
		  ((AYear % 400) == 0));
};


int TDateSelectForm::DaysPerMonth(int AYear, int AMonth)
{
  int result;
  const int DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  result = DaysInMonth[AMonth-1];
  if ((AMonth == 2) && IsLeapYear(AYear))
	++result; // leap-year Feb is special

  return result;
};

int TDateSelectForm::DaysThisMonth()
{
  Word  AYear, AMonth, ADay;
  DecodeDate(CalendarDate, AYear, AMonth, ADay);
  return DaysPerMonth(AYear, AMonth);
};


String TDateSelectForm::GetCellText(int ACol, int ARow)
{
  int DayNum;
  System::String  result;

  if (!ARow)
    // day names at tops of columns
    //
	result = ShortDayNames[(FStartOfWeek + ACol) % 7];
  else
  {
	 DayNum = FMonthOffset + ACol + (ARow - 1) * 7;
    if ((DayNum < 1) || (DayNum > DaysThisMonth()))
      result = "";
    else
      result = IntToStr(DayNum);
  }

  return result;
};

// Given a value of 1 or -1, moves to Next or Prev month accordingly.
//
void TDateSelectForm::ChangeMonth(int Delta)
{
  Word  AYear, AMonth, ADay;
  TDateTime NewDate;
  int CurDay;

  DecodeDate(CalendarDate, AYear, AMonth, ADay);
  CurDay = ADay;
  if (Delta > 0)
	ADay = DaysPerMonth(AYear, AMonth);
  else
	ADay = 1;

  NewDate = EncodeDate(AYear, AMonth, ADay);
  NewDate = NewDate + Delta;
  DecodeDate(NewDate, AYear, AMonth, ADay);
  if (DaysPerMonth(AYear, AMonth) > CurDay)
    ADay = CurDay;
  else
    ADay = DaysPerMonth(AYear, AMonth);

  CalendarDate = EncodeDate(AYear, AMonth, ADay);
}

void TDateSelectForm::PrevMonth()
{
  ChangeMonth(-1);
  UpdateCalendar();
}

void TDateSelectForm::NextMonth()
{
  ChangeMonth(1);
  UpdateCalendar();
}

void TDateSelectForm::NextYear()
{
  Word  AYear, AMonth, ADay;
  DecodeDate(CalendarDate, AYear, AMonth, ADay);

  if (IsLeapYear(AYear) && (AMonth == 2) && (ADay == 29))
	 ADay = 28;
  ++AYear;
  CalendarDate = EncodeDate(AYear, AMonth, ADay);
  UpdateCalendar();
}

void TDateSelectForm::PrevYear()
{
  Word  AYear, AMonth, ADay;
  DecodeDate(CalendarDate, AYear, AMonth, ADay);
  if (IsLeapYear(AYear) && (AMonth == 2) && (ADay == 29))
	 ADay = 28;
  --AYear;
  CalendarDate = EncodeDate(AYear, AMonth, ADay);
  UpdateCalendar();
}

void TDateSelectForm::UpdateCalendar()
{
  Word  AYear, AMonth, ADay;
  TDateTime FirstDate;

  DecodeDate(CalendarDate, AYear, AMonth, ADay);
  FirstDate = EncodeDate(AYear, AMonth, 1);

  // Day of week for 1st of month.
  //

  FMonthOffset = 2 - ((DayOfWeek(FirstDate) - FStartOfWeek + 7) % 7);
  if (FMonthOffset == 2)
	FMonthOffset = -5;

  CalendarGrid->Col = (ADay - FMonthOffset) % 7;
  CalendarGrid->Row = (ADay - FMonthOffset) / 7 + 1;

  CalendarGrid->Repaint();

	MonthDownButton->Caption = "<< " + ShortMonthNames[(AMonth - 1 + 11) % 12];
	YearDownButton->Caption = "<< " + String(AYear - 1);
	MonthUpButton->Caption = ShortMonthNames[(AMonth - 1 + 1) % 12] + " >>";
	YearUpButton->Caption = String(AYear + 1) + " >>";
	DateLabel->Caption = CalendarDate.FormatString("dd/mm/yyyy");

};
//---------------------------------------------------------------------------
__fastcall TDateSelectForm::TDateSelectForm(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::FormShow(TObject */*Sender*/)
{
	UpdateCalendar();
}
//---------------------------------------------------------------------------
void __fastcall TDateSelectForm::MonthDownButtonClick(TObject */*Sender*/)
{
	PrevMonth();
}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::YearDownButtonClick(TObject */*Sender*/)
{
	PrevYear();
}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::MonthUpButtonClick(TObject */*Sender*/)
{
	NextMonth();
}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::YearUpButtonClick(TObject */*Sender*/)
{
	NextYear();
}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::SelectButtonClick(TObject */*Sender*/)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::CancelButtonClick(TObject */*Sender*/)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::CalendarGridClick(TObject */*Sender*/)
{
	if (CalendarGrid->Row > 0)
	{
	  String TheCellText;
	  Word  AYear, AMonth, ADay;
	  DecodeDate(CalendarDate, AYear, AMonth, ADay);

	  TheCellText = GetCellText(CalendarGrid->Col, CalendarGrid->Row);
	  if(!TheCellText.IsEmpty())
		ADay = TheCellText.ToInt();

	  CalendarDate = EncodeDate(AYear, AMonth, ADay);
	  UpdateCalendar();
	}
}
//---------------------------------------------------------------------------

void __fastcall TDateSelectForm::CalendarGridDrawCell(TObject */*Sender*/, int ACol, int ARow,
		  TRect &Rect, TGridDrawState /*State*/)
{
  const System::String TheText = GetCellText(ACol, ARow);
  CalendarGrid->Canvas->TextRect(
	Rect,
	Rect.Left + (Rect.Right - Rect.Left - CalendarGrid->Canvas->TextWidth(TheText)) / 2,
	Rect.Top + (Rect.Bottom - Rect.Top - CalendarGrid->Canvas->TextHeight(TheText)) / 2,
	TheText);
}
//---------------------------------------------------------------------------

