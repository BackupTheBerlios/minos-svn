//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FWhisperSplitter.h"
#include "UWhisperSplitter.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmWhisperSplitterDemo *frmWhisperSplitterDemo;
//---------------------------------------------------------------------------
__fastcall TfrmWhisperSplitterDemo::TfrmWhisperSplitterDemo(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmWhisperSplitterDemo::FormCreate(TObject *Sender)
{
	fSplitterLeft = new TWhisperSplitter(SplitterLeft, pnlLeft);
	fSplitterLeft->Bitmap = Image_DotInset->Picture->Bitmap;

	fSplitterRight = new TWhisperSplitter(SplitterRight, pnlRight);
	fSplitterRight->Bitmap = Image_DotInset->Picture->Bitmap;
	fSplitterRight->AutoRestore = true;

	fSplitterBottom = new TWhisperSplitter(SplitterBottom, pnlBottom);
	fSplitterBottom->Bitmap = Image_DotInset->Picture->Bitmap;
	fSplitterBottom->BitmapInterations = 20;
	fSplitterBottom->AutoRestore = true;
	fSplitterBottom->HighlightColor = clMoneyGreen;

	fSplitterTop = new TWhisperSplitter(SplitterTop, pnlTop);
	fSplitterTop->Bitmap = Image_DotInset->Picture->Bitmap;
	fSplitterTop->HighlightColor = clMoneyGreen;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWhisperSplitterDemo::FormDestroy(TObject *Sender)
{
	fSplitterLeft->Free();
	fSplitterRight->Free();
	fSplitterTop->Free();
	fSplitterBottom->Free();
}
//---------------------------------------------------------------------------
void __fastcall TfrmWhisperSplitterDemo::btnRestoreAllClick(TObject *Sender)
{
	fSplitterLeft->Restore();
	fSplitterRight->Restore();
	fSplitterTop->Restore();
	fSplitterBottom->Restore();
}
//---------------------------------------------------------------------------
void __fastcall TfrmWhisperSplitterDemo::btnMinimizeAllClick(TObject *Sender)
{
	fSplitterLeft->Minimize();
	fSplitterRight->Minimize();
	fSplitterTop->Minimize();
	fSplitterBottom->Minimize();
}
//---------------------------------------------------------------------------
