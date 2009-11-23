//---------------------------------------------------------------------------

#ifndef MDateSelectFormH
#define MDateSelectFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
typedef short TDayOfWeek;

class TDateSelectForm : public TForm
{
__published:	// IDE-managed Components
	TButton *MonthDownButton;
	TButton *YearDownButton;
	TButton *MonthUpButton;
	TButton *YearUpButton;
	TLabel *DateLabel;
	TButton *SelectButton;
	TButton *CancelButton;
	TStringGrid *CalendarGrid;
	void __fastcall MonthDownButtonClick(TObject *Sender);
	void __fastcall YearDownButtonClick(TObject *Sender);
	void __fastcall MonthUpButtonClick(TObject *Sender);
	void __fastcall YearUpButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SelectButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall CalendarGridClick(TObject *Sender);
	void __fastcall CalendarGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
private:	// User declarations
	bool IsLeapYear(int AYear);
	int DaysPerMonth(int AYear, int AMonth);
	int DaysThisMonth();
	String GetCellText(int ACol, int ARow);
	int FMonthOffset;
	TNotifyEvent FOnChange;
	TDayOfWeek FStartOfWeek;

	void UpdateCalendar();
	void ChangeMonth(Integer Delta);
	void NextMonth();
	void NextYear();
	void PrevMonth();
	void PrevYear();

public:		// User declarations
	__fastcall TDateSelectForm(TComponent* Owner);
	TDateTime CalendarDate;
};
//---------------------------------------------------------------------------
extern PACKAGE TDateSelectForm *DateSelectForm;
//---------------------------------------------------------------------------
#endif
