//---------------------------------------------------------------------------

#ifndef OptionsFrameH
#define OptionsFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TOptionFrame : public TFrame
{
__published:	// IDE-managed Components
   TLabel *OptionLabel;
   TEdit *OptionEdit;
   TPanel *Panel1;
private:	// User declarations
public:		// User declarations
   __fastcall TOptionFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionFrame *OptionFrame;
//---------------------------------------------------------------------------
#endif
