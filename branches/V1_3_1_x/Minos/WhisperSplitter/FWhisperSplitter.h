//---------------------------------------------------------------------------

#ifndef FWhisperSplitterH
#define FWhisperSplitterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "UWhisperSplitter.hpp"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmWhisperSplitterDemo : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlLeft;
	TSplitter *SplitterLeft;
	TPanel *pnlCenter;
	TPanel *pnlRight;
	TSplitter *SplitterRight;
	TPanel *pnlTop;
	TPanel *pnlBottom;
	TSplitter *SplitterTop;
	TSplitter *SplitterBottom;
	TImage *Image_DotInset;
	TButton *btnRestoreAll;
	TButton *btnMinimizeAll;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnRestoreAllClick(TObject *Sender);
	void __fastcall btnMinimizeAllClick(TObject *Sender);
private:	// User declarations
	TWhisperSplitter *fSplitterLeft;
	TWhisperSplitter *fSplitterRight;
	TWhisperSplitter *fSplitterTop;
	TWhisperSplitter *fSplitterBottom;
public:		// User declarations
	__fastcall TfrmWhisperSplitterDemo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWhisperSplitterDemo *frmWhisperSplitterDemo;
//---------------------------------------------------------------------------
#endif
